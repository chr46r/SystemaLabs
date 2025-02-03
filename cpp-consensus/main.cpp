#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

// Mutex for thread-safe logging
static std::mutex blockMutex;

// Change this path if you want an absolute path 
static const std::string BLOCK_LOG_PATH = "blocks.log";
static std::ofstream blockLog(BLOCK_LOG_PATH, std::ios::app);

struct Transaction {
    std::string sender;
    std::string receiver;
    std::string amount;
};

std::vector<Transaction> mempool;

int nodeId    = 0;
int localPort = 0;
std::string peerIp = "";
int peerPort = 0;

// Forward declaration
void broadcastTx(const Transaction& tx);

void handleTx(const std::string& txMessage) {
    // parse "Alice,Bob,100"
    size_t pos1 = txMessage.find(',');
    size_t pos2 = txMessage.rfind(',');
    if (pos1 != std::string::npos && pos2 != std::string::npos && pos2 > pos1) {
        Transaction tx;
        tx.sender   = txMessage.substr(0, pos1);
        tx.receiver = txMessage.substr(pos1 + 1, pos2 - (pos1 + 1));
        tx.amount   = txMessage.substr(pos2 + 1);

        mempool.push_back(tx);

        // Log to blocks.log
        {
            std::lock_guard<std::mutex> guard(blockMutex);
            blockLog << "TX " << tx.sender << "," 
                     << tx.receiver << "," 
                     << tx.amount << std::endl;
            blockLog.flush();
        }

        std::cout << "[Node" << nodeId << "] Received TX: "
                  << tx.sender << " -> "
                  << tx.receiver << " : " << tx.amount << "\n";

        // Broadcast to the peer so Node2 also receives it (and vice versa)
        broadcastTx(tx);

    } else {
        std::cerr << "[Node" << nodeId << "] Invalid TX format\n";
    }
}

void broadcastTx(const Transaction& tx) {
    if (peerIp.empty() || peerPort == 0) {
        return; 
    }
    // Build "TX Alice,Bob,100"
    std::string message = "TX " + tx.sender + "," + tx.receiver + "," + tx.amount;

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "[Node" << nodeId << "] broadcastTx: socket() failed\n";
        return;
    }

    sockaddr_in peerAddr;
    peerAddr.sin_family = AF_INET;
    peerAddr.sin_port = htons(peerPort);
    inet_pton(AF_INET, peerIp.c_str(), &peerAddr.sin_addr);

    if (connect(sock, (struct sockaddr*)&peerAddr, sizeof(peerAddr)) < 0) {
        std::cerr << "[Node" << nodeId << "] broadcastTx: connect() failed\n";
        close(sock);
        return;
    }

    int sent = send(sock, message.c_str(), message.size(), 0);
    if (sent < 0) {
        std::cerr << "[Node" << nodeId << "] broadcastTx: send() failed\n";
    }
    close(sock);
}

void listenLoop() {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        std::cerr << "[Node" << nodeId << "] Could not create server socket\n";
        return;
    }
    int opt = 1;
    setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port   = htons(localPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "[Node" << nodeId << "] Bind failed on port " << localPort << "\n";
        close(serverSock);
        return;
    }

    listen(serverSock, 5);
    std::cout << "[Node" << nodeId << "] Listening on port " 
              << localPort << "...\n";

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientSize = sizeof(clientAddr);
        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientSize);
        if (clientSock < 0) {
            std::cerr << "[Node" << nodeId << "] Accept failed\n";
            continue;
        }

        char buffer[1024] = {0};
        int bytesRead = read(clientSock, buffer, 1023);
        if (bytesRead > 0) {
            std::string msg(buffer, bytesRead);

            if (msg.rfind("TX ", 0) == 0) {
                // "TX Alice,Bob,100"
                std::string txData = msg.substr(3);
                handleTx(txData);
                std::string reply = "OK: TX received\n";
                send(clientSock, reply.c_str(), reply.size(), 0);
            } else {
                std::string reply = "ERR: Unknown command\n";
                send(clientSock, reply.c_str(), reply.size(), 0);
            }
        }
        close(clientSock);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: ./poa_node <nodeId> <localPort> <peerIp> <peerPort>\n";
        return 1;
    }
    nodeId    = std::stoi(argv[1]);
    localPort = std::stoi(argv[2]);
    peerIp    = argv[3];
    peerPort  = std::stoi(argv[4]);

    std::cout << "[Node" << nodeId << "] Starting with localPort=" 
              << localPort << ", peer=" << peerIp << ":" << peerPort 
              << "\n";

    // Start listening in a separate thread
    std::thread t_listen(listenLoop);

    // If needed, start a block-producing thread here

    t_listen.join();
    return 0;
}
