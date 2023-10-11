#ifndef HTTP_H
#define HTTP_H

#include <string>
#include <map>
#include <vector>

class HTTPReplyData {
public:
    int Code;
    std::string ContentType;
    std::string MessageAsString;
    std::vector<char> MessageAsBytes;
    std::map<std::string, std::string> Headers;
};

class HTTP {
public:
    static HTTPReplyData performRequest(const std::string& url, const std::string& sJson);
    static HTTPReplyData performRequest(const std::string& url, const std::string& sJson, const std::string& xSLOwner);

private:
    static HTTPReplyData Request(const std::string& url, const std::string& sJson, const std::string& xSLOwner);
};

#endif
