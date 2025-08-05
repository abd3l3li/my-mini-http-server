#include "server.hpp"

void handleMultiClients(int client_fd, struct sockaddr_in &client_addr)
{
    std::thread
    (
        [client_fd]()
        {
        char buffer[1024];
        std::string str1, str2;
        str1 = "HTTP/1.1 200 OK\r\n\r\n";
        str2 = "HTTP/1.1 404 Not Found\r\n\r\n";

        // recv is like receiving a call through the socket fd
        // used c_string in buffer for recv function
        ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received < 0) {
            std::cerr << "Failed to receive data from client\n";
        }
        else if (bytes_received == 0) {
            std::cout << "No data sent from client\n";
        }
        else
        {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            // istringstream is like a string it takes c_string in constructor
            std::istringstream request(buffer);
            std::string method, path, version;

            // using >> allows us to read the input stream word by word
            request >> method >> path >> version;

            if (path == "/") { // send sends a response to the client through the socket fd
            send(client_fd, str1.c_str(), str1.size(), 0);
            } else if (path.substr(0, 5) == "/echo") {
            // Extract the message after "/echo/"
            std::string empty = "HTTP/1.1 400 Bad Request\r\n\r\n";
            if (path.size() < 6) {
                send(client_fd, empty.c_str(), empty.size(), 0);
            }
            std::string message = path.substr(6); // 6 to skip "/echo/"
            std::string response = "HTTP/1.1 200 OK"
            "\r\nContent-Type: text/plain\r\nContent-Length: "
            + std::to_string(message.size())
            + "\r\n\r\n"
            + message;
            send(client_fd, response.c_str(), response.size(), 0);
            } else if (path == "/user-agent")
            {
            // Extract User-Agent from the request headers
            std::string user_agent;
            std::string line;
            while (std::getline(request, line) && !line.empty()) {
                // headers are case-insensitive in real HTTP servers
                if (line.find("User-Agent:") == 0) {
                user_agent = line.substr(12); // 12 to skip "User-Agent: "
                if (!user_agent.empty() && user_agent[0] == ' ')
                    user_agent.erase(0, 1); // Remove leading space if present 
                    // back() gets reference to last character
                if (user_agent.back() == '\r') {
                    // pop_back() removes the last character from the string
                    user_agent.pop_back(); // Remove trailing \r if present
                }
                break;
                }
            }
            if (!user_agent.empty()) {
                std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "
                + std::to_string(user_agent.size())
                + "\r\n\r\n"
                + user_agent;
                send(client_fd, response.c_str(), response.size(), 0);
            } else {
                send(client_fd, str2.c_str(), str2.size(), 0);
            }

            } else
                send(client_fd, str2.c_str(), str2.size(), 0);
        }

        close(client_fd);
        }
    ).detach(); // Detach the thread to handle the client connection asynchronously
}