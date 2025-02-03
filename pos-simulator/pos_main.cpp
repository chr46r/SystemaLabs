#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    std::string sender, receiver, amount;
    std::cout << "Enter sender: ";
    std::cin >> sender;
    std::cout << "Enter receiver: ";
    std::cin >> receiver;
    std::cout << "Enter amount: ";
    std::cin >> amount;

    // Connect to Node1 on 127.0.0.1:5001 (adjust as desired)
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket.\n";
        return 1;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5001);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Connection failed.\n";
        close(sock);
        return 1;
    }

    // Format: "TX Alice,Bob,100"
    std::string txMessage = "TX " + sender + "," + receiver + "," + amount;
    send(sock, txMessage.c_str(), txMessage.size(), 0);

    // Read response
    char buf[1024] = {0};
    int n = read(sock, buf, 1023);
    if (n > 0) {
        std::cout << "[Node response] " << std::string(buf, n) << std::endl;
    }

    close(sock);
    return 0;
}
