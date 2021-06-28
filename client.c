// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 4242

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	/*if(inet_pton(AF_INET, "192.168.2.2", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}*/
	serv_addr.sin_addr.s_addr=inet_addr("192.168.2.2");

	/*if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}*/
	if(sendto(sock , hello , strlen(hello) , 0,(struct sockaddr *) &serv_addr, sizeof(serv_addr) )==-1){
		perror("Error sending data: ");
		return -1;
	};
	printf("Hello message sent\n");
	valread = read( sock , buffer, 1024);
	printf("%s\n",buffer );
	return 0;
}
