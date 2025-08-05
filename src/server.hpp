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
#include <fstream>
#include <sstream>
#include <thread>


void handleMultiClients(int client_fd, struct sockaddr_in &client_addr,
                            int argc, char **argv);

void runServer(int server_fd, struct sockaddr_in &client_addr,
                            int &client_addr_len, int argc, char **argv);

void handleFiles(int client_fd, const std::string &fullPath);
void handlePostquery(int client_fd, const std::string &fullPath, const std::string &buffer);