#pragma once

#include <string>
#include <openssl/sha.h>

using std::string;

namespace sha {
    inline string sum256(string data)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];

        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, data.c_str(), data.size());
        SHA256_Final(hash, &sha256);

        char buffer[64];
        for (int i(0); i < SHA256_DIGEST_LENGTH; i++)
            sprintf(buffer + (i * 2), "%02x", hash[i]);

        return string{std::begin(buffer), std::end(buffer)};
    }
}