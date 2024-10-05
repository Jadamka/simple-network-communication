#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXDATASIZE 100

int main(int argc, char* argv[])
{
    // Need host and port
    if(argc != 3){
        printf("Usage: %s <IP address> <port>\n", argv[0]);
        return -1;
    }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0){
        perror("client: socket");
        return -1;
    }

    struct sockaddr_in client_info;
    memset(&client_info, 0, sizeof(client_info));
    client_info.sin_family = AF_INET; // IPv4
    client_info.sin_port = htons(atoi(argv[2])); // port
    client_info.sin_addr.s_addr = inet_addr(argv[1]); // host address (127.0.0.1)
    socklen_t client_info_len = sizeof(client_info); 

    if(connect(sfd, (struct sockaddr *)&client_info, client_info_len) < 0){
        perror("client: connect");
        return -1;
    }

    char buffer[MAXDATASIZE];
    int bytes_received;

    if((bytes_received = recv(sfd, buffer, MAXDATASIZE-1, 0)) < 0){
        perror("client: recv");
        return -1;
    }

    if(bytes_received != 0){
        buffer[bytes_received] = '\0';
        printf("\n%s\n", buffer);
    }
    else{
        printf("Server closed the connection\n");
    }

    close(sfd);
    return 0;
}
