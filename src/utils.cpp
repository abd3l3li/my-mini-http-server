#include "server.hpp"

void handleFiles(int client_fd, const std::string &fullPath)
{
    std::string response;
    if (!fullPath.empty()) {
        std::ifstream file(fullPath);
        if (file) {
            std::ostringstream oss;
            oss << file.rdbuf();
            std::string content = oss.str();
            response = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/octet-stream\r\n"
                    "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n"
                    + content;
        } else {
            response = "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/octet-stream\r\n"
                    "Content-Length: 0\r\n\r\n";
        }
    }
    send(client_fd, response.c_str(), response.size(), 0);
}