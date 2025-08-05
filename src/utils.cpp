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

void handlePostquery(int client_fd, const std::string &fullPath, const std::string &buffer)
{
    // Find the body start (after the blank line)
                            std::string requestStr(buffer);
                            size_t body_pos = requestStr.find("\r\n\r\n");
                            if (body_pos != std::string::npos) {
                                std::string body = requestStr.substr(body_pos + 4);

                                // Save body to file
                                std::ofstream outfile(fullPath);
                                if (outfile) {
                                    outfile << body;
                                    outfile.close();

                                    std::string response = "HTTP/1.1 201 Created\r\n\r\n";
                                    send(client_fd, response.c_str(), response.size(), 0);
                                } else {
                                    std::string response = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
                                    send(client_fd, response.c_str(), response.size(), 0);
                                }
                            } else {
                                // Bad Request (no body found)
                                std::string response = "HTTP/1.1 400 Bad Request\r\n\r\n";
                                send(client_fd, response.c_str(), response.size(), 0);
                            }
}