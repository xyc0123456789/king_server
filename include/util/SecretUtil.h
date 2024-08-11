#ifndef _SECRETUTIL_H_
#define _SECRETUTIL_H_

#include "vector"
#include <sodium.h>


// Encrypt using AES
std::vector<unsigned char> aesEncrypt(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& nonce) {
    std::vector<unsigned char> ciphertext(plaintext.size() + crypto_aead_aes256gcm_ABYTES);
    unsigned long long ciphertext_len;
    crypto_aead_aes256gcm_encrypt(ciphertext.data(), &ciphertext_len, plaintext.data(), plaintext.size(), NULL, 0, NULL, nonce.data(), key.data());
    ciphertext.resize(ciphertext_len);
    return ciphertext;
}

// Decrypt using AES
std::vector<unsigned char> aesDecrypt(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key, const std::vector<unsigned char>& nonce) {
    std::vector<unsigned char> plaintext(ciphertext.size() - crypto_aead_aes256gcm_ABYTES);
    unsigned long long plaintext_len;
    if (crypto_aead_aes256gcm_decrypt(plaintext.data(), &plaintext_len, NULL, ciphertext.data(), ciphertext.size(), NULL, 0, nonce.data(), key.data()) != 0) {
        // Handle error
    }
    plaintext.resize(plaintext_len);
    return plaintext;
}


#endif // _SECRETUTIL_H_