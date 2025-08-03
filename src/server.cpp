#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sstream>




int main(int argc, char **argv) {
  // Flush after every std::cout / std::cerr
  //! unbuffered mode
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  // You can use print statements as follows for debugging, they'll be visible when running tests.
  std::cout << "Logs from your program will appear here!\n";

  // Uncomment this block to pass the first stage

  //! consider socket like a new phone
  //! in general socket is like bidirectional pipe
    //! in the same machine or over network
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create server socket\n";
    return 1;
  }
  
  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);
  
  //! then bind is like giving the phone a number
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port 4221\n";
    return 1;
  }
  
  int connection_backlog = 5;
  //! listen is like setting the phone to accept calls
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return 1;
  }
  
  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  
  std::cout << "Waiting for a client to connect...\n";
  
  // Accept a client connection in a loop
  while (true) {
    //! accept is like ansering the phone when it rings
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
    if (client_fd < 0) {
      std::cerr << "Failed to accept client connection\n";
      continue;
    }

    char buffer[1024];
    std::string str1, str2;
    str1 = "HTTP/1.1 200 OK\r\n\r\n";
    str2 = "HTTP/1.1 404 Not Found\r\n\r\n";

    // recv is like receiving a call through the socket fd
    // used c_string in buffer for recv function
    ssize_t bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
      std::cerr << "Failed to receive data from client\n";
      close(client_fd);
      continue;
    }
    else if (bytes_received == 0) {
      std::cout << "No data sent from client\n";
    } else {
      buffer[bytes_received] = '\0'; // Null-terminate the received data
      // istringstream is like a string it takes c_string in constructor
      std::istringstream request(buffer);
      std::string method, path, version;

      // using >> allows us to read the input stream word by word
      request >> method >> path >> version;

      if (path == "/") { // send sends a response to the client through the socket fd
        send(client_fd, str1.c_str(), str1.size(), 0);
      } else {
        send(client_fd, str2.c_str(), str2.size(), 0);
      }
    }

    close(client_fd);
  }

  close(server_fd);

  return 0;
}
