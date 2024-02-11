
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>



#include "server_functions.h"

extern char buffer[255];
extern int clientArr[5];
extern char clientName[5][20];
extern int sockfd, portno, nRet;
extern fd_set fr, fw, fe;

int main(int argv, char *args[]){

	

	portno = atoi(args[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Socket init failed");
	}
	
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len;
	
	bzero((char *) &server_addr, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(portno);
	
	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);
	
	
	
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		printf("Bind failed");
	}
	listen(sockfd, 5);
	

	
	
	while(1){
	
		nRet = reset_fd();
		
		if(nRet >= 0){

			if(FD_ISSET(sockfd, &fr)){
				int nLen = sizeof(struct sockaddr);
				int nClientSocket = accept(sockfd, NULL, NULL);
				receive_new_user(nClientSocket);
			}
			for(int i=0;i<5;i++){
				if(FD_ISSET(clientArr[i], &fr)){
					forward_msg(i);

					
				}
				
			}
		}else if(nRet < 0){
			printf("Select failed");
		}else{
			printf("0\n");
		}
	}

	return 0;
}
