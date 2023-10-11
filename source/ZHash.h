#ifndef ZHASH_H
#define ZHASH_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

class ZHash {
public:
    ZHash();
    void Reset();
    std::string ToString();
    void NewKey();
    void NewSerial();
    void CalculateKey(const std::string& K);

    static std::vector<unsigned char> HashToBytes(const std::string& Key);
    static ZHash Bytes2Hash(const std::vector<unsigned char>& key);
    static std::string Bytes2HashStr(const std::vector<unsigned char>& key);

private:
    std::string _key;
    std::string _template;
    std::vector<unsigned char> CalculateMD5(const std::string& input);
};

#endif
