#include "pq_crypto.h"
#include <iostream>

namespace PQCrypto {

KeyPair generateKeyPair() {
    KeyPair kp;
    kp.publicKey = {'P','Q','P','U','B'};
    kp.privateKey = {'P','Q','P','R','I','V'};
    return kp;
}

std::vector<unsigned char> signData(const std::vector<unsigned char> &msg,
                                    const std::vector<unsigned char> &privateKey)
{
    // Simple example: just append "SIG"
    std::vector<unsigned char> signature(msg);
    signature.insert(signature.end(), {'S','I','G'});
    return signature;
}

bool verifySignature(const std::vector<unsigned char> &msg,
                     const std::vector<unsigned char> &signature,
                     const std::vector<unsigned char> &publicKey)
{
    if (signature.size() < msg.size() + 3) return false;

    size_t n = signature.size();
    // Check last 3 bytes are S,I,G
    if (signature[n-3] == 'S' && signature[n-2] == 'I' && signature[n-1] == 'G') {
        // Compare the portion of signature that should match msg
        for (size_t i = 0; i < msg.size(); ++i) {
            if (msg[i] != signature[i]) return false;
        }
        return true;
    }
    return false;
}

}
