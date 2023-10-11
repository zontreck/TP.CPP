#include "ZHash.h"
#include <openssl/md5.h>

ZHash::ZHash() : _template("") { }

void ZHash::Reset() {
    _key = _template;
}

std::string ZHash::ToString() {
    return _key;
}

void ZHash::NewKey() {
    _key = std::string(10, '0') + "-" +
            std::string(4, '0') + "-" +
            std::string(6, '0') + "-" +
            std::string(8, '0');
}

void ZHash::NewSerial() {
    _key = std::string(10, '0') + "-" +
            std::string(6, '0') + "-" +
            std::string(4, '0') + "-" +
            std::string(4, '0') + "-" +
            std::string(2, '0') + "-" +
            std::string(4, '0') + "-" +
            std::string(8, '0');
}

void ZHash::CalculateKey(const std::string& K) {
    std::string valid = "abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ=.+/\\][{}';:?><,_-)(*&^%$#@!`~|";
    while (valid.length() < K.length()) {
        valid += valid;
    }

    std::string tmp = _key;

    for (size_t i = 0; i < _key.length(); i++) {
        char V = _key[i];
        if (V != '-') {
            MD5_CTX ctx;
            MD5_Init(&ctx);
            for (size_t ii = 0; ii < std::min(K.length(), _key.length()); ii++) {
                std::string inputStr = K + std::to_string(i) + valid[i] + valid[ii];
                MD5_Update(&ctx, inputStr.c_str(), inputStr.length());
                unsigned char md5Data[MD5_DIGEST_LENGTH];
                MD5_Final(md5Data, &ctx);
                tmp[i] = md5Data[i > 31 ? 1 : i];
            }
        }
    }
    _key = tmp;
}

std::vector<unsigned char> ZHash::HashToBytes(const std::string& Key) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < Key.length(); i += 2) {
        std::string byteStr = Key.substr(i, 2);
        unsigned char byte = static_cast<unsigned char>(std::stoi(byteStr, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}

ZHash ZHash::Bytes2Hash(const std::vector<unsigned char>& key) {
    ZHash itm;
    for (unsigned char b : key) {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
        itm._key += ss.str();
    }
    itm._template = itm._key;
    return itm;
}

std::string ZHash::Bytes2HashStr(const std::vector<unsigned char>& key) {
    return Bytes2Hash(key).ToString();
}
