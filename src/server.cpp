#include "server.hpp"

void runServer(int server_fd, struct sockaddr_in &client_addr, int &client_addr_len){
  // Flush after every std::cout / std::cerr
  //! unbuffered mode
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  std::cout << "Waiting for a client to connect...\n";
  
  // Accept a client connection in a loop
  while (true)
  {
    //! accept is like ansering the phone when it rings
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
    if (client_fd < 0) {
      std::cerr << "Failed to accept client connection\n";
      continue;
    }

    handleMultiClients(client_fd, client_addr);
  }

  close(server_fd);


}
