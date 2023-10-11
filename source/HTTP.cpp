#include "HTTP.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <exception>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

HTTPReplyData HTTP::performRequest(const std::string& url, const std::string& sJson) {
    return Request(url, sJson, "");
}

HTTPReplyData HTTP::performRequest(const std::string& url, const std::string& sJson, const std::string& xSLOwner) {
    return Request(url, sJson, xSLOwner);
}

HTTPReplyData HTTP::Request(const std::string& url, const std::string& sJson, const std::string& xSLOwner) {
    HTTPReplyData rd;

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        return rd;
    }

    // Extract host and path from the URL
    size_t httpPos = url.find("://");
    size_t hostEnd = url.find('/', httpPos + 3);
    std::string host = url.substr(httpPos + 3, hostEnd - (httpPos + 3));
    std::string path = (hostEnd == std::string::npos) ? "/" : url.substr(hostEnd);

    // Get host information
    struct hostent* server = gethostbyname(host.c_str());
    if (server == nullptr) {
        perror("Error, no such host");
        close(sockfd);
        return rd;
    }

    // Set up server address
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    serv_addr.sin_addr = *((struct in_addr*)server->h_addr);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to server");
        close(sockfd);
        return rd;
    }

    // Create the HTTP request
    std::string request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    if (!xSLOwner.empty()) {
        request += "X-SecondLife-Owner-Key: " + xSLOwner + "\r\n";
    }
    request += "Content-Length: " + std::to_string(sJson.length()) + "\r\n";
    request += "Content-Type: application/json\r\n\r\n";
    request += sJson;

    // Send the request
    if (send(sockfd, request.c_str(), request.length(), 0) < 0) {
        perror("Error writing to socket");
        close(sockfd);
        return rd;
    }

    // Receive the response
    std::vector<char> buffer(4096);
    std::string response;

    while (true) {
        int bytesReceived = recv(sockfd, buffer.data(), buffer.size(), 0);
        if (bytesReceived < 0) {
            perror("Error reading from socket");
            break;
        } else if (bytesReceived == 0) {
            break;
        }

        response.append(buffer.data(), bytesReceived);
    }

    // Parse the HTTP response
    size_t bodyStart = response.find("\r\n\r\n") + 4;
    if (bodyStart == std::string::npos) {
        perror("Error parsing HTTP response");
        close(sockfd);
        return rd;
    }

    rd.MessageAsString = response.substr(bodyStart);
    rd.Code = std::atoi(response.substr(9, 3).c_str());  // Extract the response code
    rd.ContentType = "application/json";  // Hardcoded content type for this example

    // Close the socket
    close(sockfd);

    return rd;
}
