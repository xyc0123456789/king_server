#include <gtest/gtest.h>
#include <botan/ec_group.h>
#include <botan/pubkey.h>
#include <botan/hex.h>
#include <botan/sm2.h>
#include <botan/auto_rng.h>
#include <botan/pkcs8.h>
#include <botan/pem.h>
#include <botan/secmem.h> // 包含 secure_vector
#include <vector>
#include <botan/x509_key.h>

TEST(SM2Test, EncryptDecrypt) {
    Botan::AutoSeeded_RNG rng;

    // 使用 from_name 方法生成 SM2 私钥
    Botan::EC_Group ec_group = Botan::EC_Group::from_name("sm2p256v1");
    Botan::SM2_PrivateKey private_key(rng, ec_group);
    Botan::SM2_PublicKey public_key(private_key);

    // 导出私钥和公钥为 PEM 格式字符串
    std::string private_key_pem = Botan::PKCS8::PEM_encode(private_key);
    std::string public_key_pem = Botan::X509::PEM_encode(public_key);

    // 打印私钥和公钥
    std::cout << "Private Key:\n" << private_key_pem << std::endl;
    std::cout << "Public Key:\n" << public_key_pem << std::endl;

    // 重新导入私钥和公钥
    Botan::DataSource_Memory private_key_data(private_key_pem);
    Botan::DataSource_Memory public_key_data(public_key_pem);
    std::unique_ptr<Botan::Private_Key> loaded_private_key(Botan::PKCS8::load_key(private_key_data));
    std::unique_ptr<Botan::Public_Key> loaded_public_key(Botan::X509::load_key(public_key_data));

    // 加密
    Botan::PK_Encryptor_EME encryptor(*loaded_public_key, rng, "SM3");
    std::vector<uint8_t> plaintext = Botan::hex_decode("48656C6C6F2C20576F726C6421"); // "Hello, World!"
    std::vector<uint8_t> ciphertext = encryptor.encrypt(plaintext, rng);

    // 解密
    Botan::PK_Decryptor_EME decryptor(*loaded_private_key, rng, "SM3");
    Botan::secure_vector<uint8_t> decrypted_secure = decryptor.decrypt(ciphertext);

    // 将 secure_vector 转换为 std::vector
    std::vector<uint8_t> decrypted(decrypted_secure.begin(), decrypted_secure.end());

    ASSERT_EQ(std::string(decrypted.begin(), decrypted.end()), "Hello, World!");
}


TEST(SM2Test, VerifySignature) {
    Botan::AutoSeeded_RNG rng;

    // 使用 from_name 方法生成 SM2 私钥
    Botan::EC_Group ec_group = Botan::EC_Group::from_name("sm2p256v1");
    Botan::SM2_PrivateKey private_key(rng, ec_group);
    Botan::SM2_PublicKey public_key(private_key);

    // 导出私钥和公钥为 PEM 格式字符串
    std::string private_key_pem = Botan::PKCS8::PEM_encode(private_key);
    std::string public_key_pem = Botan::X509::PEM_encode(public_key);

    // 打印私钥和公钥
    std::cout << "Private Key:\n" << private_key_pem << std::endl;
    std::cout << "Public Key:\n" << public_key_pem << std::endl;

    // 签名 EMSA1(SM3)/SM3
    std::string text("This is a tasty burger!");
    std::vector<uint8_t> data(text.begin(), text.end());
    Botan::PK_Signer signer(private_key, rng, "SM3");
    std::vector<uint8_t> signature = signer.sign_message(data, rng);

    // 验签 EMSA1(SM3)/SM3
    Botan::PK_Verifier verifier(public_key, "SM3");
    verifier.update(data);
    bool result = verifier.check_signature(signature);

    ASSERT_TRUE(result);
}