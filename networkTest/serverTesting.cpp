#include <asm-generic/socket.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../src/card.h"


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    

    // Create IPv$, TCP ocket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);


    // Resuse adress/ port
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(8080); // host to network

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));


    listen(server_fd, 3);

    std::cout << "Listening for a connection \n";

    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    char buffer[1024] = {0};
    card* recCard; 

    read(new_socket, buffer, 1024);

    std::cout << "Received: " << buffer << std::endl;

    const char* hello = "Hello from server";

    send(new_socket, hello, strlen(hello), 0);
    read(new_socket, recCard, 1024);
    

    close(new_socket);
    close(server_fd);

    return 0;

    
}