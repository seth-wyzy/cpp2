#include <iostream>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "../src/card.h"

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // convert to binary
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // connect to server
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    const char* hello = "Hello from Client";
    send(sock,hello,strlen(hello), 0 );
    

    read(sock, buffer, 1024);

    std::cout << "Recieved: " << buffer << std::endl;

    close(sock);
    return 0;



}