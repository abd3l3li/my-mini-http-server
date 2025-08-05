#pragma once

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
#include <thread>


void handleMultiClients(int client_fd, struct sockaddr_in &client_addr);
void runServer(int server_fd, struct sockaddr_in &client_addr, int &client_addr_len);