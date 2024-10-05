#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BACKLOG 5

int main(int argc, char* argv[])
{
    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0){
        perror("server: socket");
        return -1;
    }

    struct sockaddr_in server_info;
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET; // IPv4
    server_info.sin_port = htons(atoi(argv[1]));
    server_info.sin_addr.s_addr = INADDR_ANY;
    socklen_t server_info_len = sizeof(server_info);

    struct sockaddr_in client_info;
    memset(&client_info, 0, sizeof(client_info));
    socklen_t client_info_len = sizeof(client_info);

    int yes = 1;
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &(yes), sizeof(yes)) < 0){
        perror("server: setsockopt");
        return -1;
    }

    if(bind(sfd, (struct sockaddr *)&server_info, server_info_len) < 0){
        perror("server: bind");
        return -1;
    }

    if(listen(sfd, BACKLOG) < 0){
        perror("server: listen");
        return -1;
    }

    int cfd = accept(sfd, (struct sockaddr *)&client_info, &client_info_len);
    if(cfd < 0){
        perror("server: accept");
        return -1;
    }
   
    char* buffer = "SUCCESS!\n";
    ssize_t sent = send(cfd, buffer, strlen(buffer), 0);
    if(sent < 0){
        perror("server: send");
        return -1;
    }
    if((sent - strlen(buffer)) != 0){
        perror("FAILED TO SEND WHOLE MESSAGE!\n");
    }

    close(cfd);
    close(sfd);
    return 0;
}
