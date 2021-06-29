// Server side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 8080
#define MAXLINE 1024

// Driver code
int main() {
	int socket_desc;
	struct sockaddr_in server_addr, client_addr;
	char server_message[]="hola";
	char client_message[100];
	int client_struct_length = sizeof(client_addr);

	//clean buffers
	memset(server_message, '\0',sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));

	//create UDP socket
	socket_desc=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);

	if(socket_desc < 0){
		printf("Error en la creacion de socket");
		return -1;
	}
	printf("Socket creado\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("192.168.2.4");

	if(bind(socket_desc,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
		printf("error puerto\n");
		return -1;
	}
	printf("bind correcto\n");
	printf("Listening for incoming messages...\n\n");
while(1){
    // Receive client's message:
    if (recvfrom(socket_desc, client_message, sizeof(client_message), 0,
         (struct sockaddr*)&client_addr, &client_struct_length) < 0){
        printf("Couldn't receive\n");
        return -1;
    }
    printf("Received message from IP: %s and port: %i\n",
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    printf("Msg from client: %s\n", client_message);
}
    // Respond to client:
    //strcpy(server_message, client_message);    
    /*printf("Enter message : ");
                gets(server_message);
	*/
    printf("mensaje enviado  %s\n",server_message);
    if (sendto(socket_desc, server_message, strlen(server_message), 0,
         (struct sockaddr*)&client_addr, client_struct_length) < 0){
        printf("Can't send\n");
        return -1;
    }

    // Close the socket:
    close(socket_desc);

    return 0;
}
	
