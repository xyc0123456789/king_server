#include <gtest/gtest.h>
#include <iostream>
#include <ostream>
#include <sodium.h>
#include <cstring>
#include <string>
#include "util/SecretUtil.h"
#include "util/StringUtil.h"

// 对称加密测试
TEST(LibSodiumTest, SymmetricEncryptionDecryption) {
    // 初始化libsodium
    ASSERT_NE(sodium_init(), -1);

    // 原始消息
    const char *message = "Hello, World!";
    size_t message_len = strlen(message);

    // 密钥和nonce
    unsigned char key[crypto_secretbox_KEYBYTES];
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char ciphertext[crypto_secretbox_MACBYTES + message_len];
    unsigned char decrypted[message_len];

    // 生成随机密钥和nonce
    randombytes_buf(key, sizeof key);
    randombytes_buf(nonce, sizeof nonce);

    // 加密
    crypto_secretbox_easy(ciphertext, (const unsigned char *)message, message_len, nonce, key);

    // 解密
    ASSERT_EQ(crypto_secretbox_open_easy(decrypted, ciphertext, sizeof ciphertext, nonce, key), 0);

    // 验证解密后的消息与原始消息相同
    ASSERT_EQ(memcmp(message, decrypted, message_len), 0);
}


TEST(LibSodiumTest, AESSymmetricEncryptionDecryption) {
    // Initialize sodium library
    ASSERT_NE(sodium_init(), -1);

    // Test data
    std::vector<unsigned char> plaintext = {'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd'};
    std::vector<unsigned char> key(crypto_aead_aes256gcm_KEYBYTES);
    std::vector<unsigned char> nonce(crypto_aead_aes256gcm_NPUBBYTES);

    // Generate random key and nonce
    randombytes_buf(key.data(), key.size());
    randombytes_buf(nonce.data(), nonce.size());

    // Encrypt
    std::vector<unsigned char> ciphertext = aesEncrypt(plaintext, key, nonce);

    // Decrypt
    std::vector<unsigned char> decryptedtext = aesDecrypt(ciphertext, key, nonce);

    std::string ori = vectorToString(plaintext);
    std::string hex = vectorToHex(ciphertext);
    std::string decode = vectorToString(decryptedtext);
    std::cout << "ori:" << ori << std::endl;
    std::cout << "hex:" << hex << std::endl;
    std::cout << "decode:" << decode << std::endl;
    // Verify
    ASSERT_EQ(plaintext, decryptedtext);

}