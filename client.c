#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#define PORT 8080 

int main(int argc, char const *argv[]) { 
	int sock = 0, valread; 
	char *message = (char*) malloc(4096 * sizeof(char));
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
		printf("Could not generate new file descriptor"); 
		return -1; 
	} 

	struct sockaddr_in serv_addr; 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	if(inet_pton(AF_INET, "0.0.0.0", &serv_addr.sin_addr) <= 0) { 
		printf("Address not properly formatted or invalid\n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		printf("Connection Failed\n"); 
		return -1; 
	}
	while (1) {
		fgets(message, 4096, stdin);
		send(sock, message, strlen(message), 0 ); 
		valread = read(sock , buffer, 1024); 
		printf("received: %s\n", buffer); 
	}
	free(message);
	return 0; 
} 
