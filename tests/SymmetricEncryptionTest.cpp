#include <gtest/gtest.h>
#include <sodium.h>
#include <cstring>

// 对称加密测试
TEST(LibSodiumTest, SymmetricEncryptionDecryption) {
    // 初始化libsodium
    ASSERT_EQ(sodium_init(), 0);

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
