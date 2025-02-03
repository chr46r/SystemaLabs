#ifndef PQ_CRYPTO_H
#define PQ_CRYPTO_H

#include <vector>

namespace PQCrypto {
    struct KeyPair {
        std::vector<unsigned char> publicKey;
        std::vector<unsigned char> privateKey;
    };

    KeyPair generateKeyPair();
    std::vector<unsigned char> signData(const std::vector<unsigned char> &msg,
                                        const std::vector<unsigned char> &privateKey);
    bool verifySignature(const std::vector<unsigned char> &msg,
                         const std::vector<unsigned char> &signature,
                         const std::vector<unsigned char> &publicKey);
}

#endif
