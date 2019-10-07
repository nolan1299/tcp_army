#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h> 
#define PORT 8080 
  
int main() {
    int sock = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("\n Could not generate new file descriptor \n"); 
		return -1; 
	} 
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr);
    bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
 
    listen(sock, SOMAXCONN); 
    struct sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(sock, (struct sockaddr*)& client, &clientSize);
 
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    memset(&host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    
    if (getnameinfo((struct sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
        printf("Connected on port %s\n", service);
        fflush(stdout);
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        printf( "Connected on port %d\n", ntohs(client.sin_port));
        fflush(stdout);
    }
 
    close(sock);
 
    char buf[4096];
    while (1) {
        memset(buf, 0, 4096);
 
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1) {
            printf("Invalid reception, terminating connection\n");
            break;
        }
        else if (bytesReceived == 0) {
            printf("Client disconnected\n");
            break;
        }
        else {
            printf("received: %s",  buf);
        }

        for (int i = 0 ; i < (bytesReceived - 1) / 2 ; i++) {
            int temp = buf[i];
            buf[i] = buf[bytesReceived - 2 - i];
            buf[bytesReceived - 2 - i] = temp;
        }
        buf[bytesReceived - 1] = '!';
        send(clientSocket, buf, bytesReceived + 1, 0);
    }
    close(clientSocket);
    return 0;
}