#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	int s;
	struct sockaddr_in dst;
	char data[512];
	
	s=socket(PF_INET, SOCK_DGRAM, 0);

	if(s==-1) exit(-1);

	dst.sin_family      = AF_INET;
	dst.sin_port        = htons(4242);
	dst.sin_addr.s_addr = inet_addr("192.168.2.2");

	while(1) {
		usleep(2000); // 2ms => 500Hz (increased from 100Hz to make the 1010 fail earlier)

		if(sendto(s, data, 512, 0, (struct sockaddr *) &dst, sizeof(dst))==-1) {
			perror("Error sending data: ");
		} 
	}
}
