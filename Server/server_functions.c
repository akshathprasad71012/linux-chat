#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server_functions.h"

char buffer[255];
int clientArr[5] = {0};
char clientName[5][20];
int sockfd, portno, nRet;
fd_set fr, fw, fe;

void read_sock(int sockId){
	int n = read(sockId, buffer, 255);
	if(n < 0){
		printf("Error on read");
	}
}

void write_sock(int sockId, char buffer[]){
	int n = write(sockId, buffer, strlen(buffer));
	  if(n < 0){
		  printf("error on write");
	  }
}

void receive_new_user(int nClientSocket){
	if(nClientSocket > 0){
		for(int i=0;i<5;i++){
			if(clientArr[i] == 0){
			    clientArr[i] = nClientSocket;
				bzero(buffer, 255);

				read_sock(clientArr[i]);
				

				printf("Client [%d]: %s\n", i, buffer);
				strcpy(clientName[i], buffer);
				
				
				bzero(buffer, 255);
				for(int k=0;k<255;k++){
					if(clientName[i][k] == '\n' || clientName[i][k] == '\0')
						break;
					 
					buffer[k] = clientName[i][k];
				}
				strcat(buffer, " joined!");
				printf("%s\n", buffer);
				for(int j=0;j<5;j++){
					
					if(clientArr[j] != sockfd && clientArr[j] != nClientSocket && clientArr[j] != 0){
						write(clientArr[j], buffer, strlen(buffer));
					}
					
				}
				printf("\n");
				   
				break;
			}
		}
	}
}

char* forward_msg(int i){
	int n;
	bzero(buffer, 255);
	read_sock(clientArr[i]);

	if(strlen(buffer) == 0){
		bzero(buffer, 255);
		clientArr[i] = 0;
		printf("\nBye Bye ....\n");
		return;
	}

	char message[255], toName[255], finalMsg[512];
	bzero(message, 255);
	bzero(toName, 255);
	bzero(finalMsg, 512);

	int k=0, atfound=0;
	for(;k<255;k++){
	if(buffer[k] == '@'){
		atfound = k;
		continue;
	}
	if(atfound)
		toName[k - atfound - 1] = buffer[k];
	else
		message[k] = buffer[k];
	}

	printf("Message - %s | To - %s\n", message, toName);

	int receiver = -1;

	for(k=0;k<5;k++){
	if(strcmp(toName, clientName[k]) == 0 && strlen(clientName[k]) > 0){
	//printf("\nCheck %lu == %lu\n", strlen(toName), strlen(clientName[k]));
		receiver = clientArr[k];
		
	}
	}

	if(receiver == -1){
	if(toName[0] == 'a' && toName[1]=='l' && toName[2]=='l'){
	receiver = -2;
	}
	}



	k=0;
	int s=0;

	while(clientName[i][k] != '\n' && clientName[i][k] != '\0'){
		finalMsg[k] = clientName[i][k];
		s++;
		k++;
		if(k >= 255) break;
	}
	k=0;
	finalMsg[s++] = ' ';
	finalMsg[s++] = ':';
	finalMsg[s++] = ' ';
	while(message[k] != '\0'){
		finalMsg[k + s] = message[k];
		k++;
		if(k >= 255) break;
	}
	if(receiver == -2){
	finalMsg[s + k++] = ' ';
	finalMsg[s + k++] = '@';
	finalMsg[s + k++] = 'a';
	finalMsg[s + k++] = 'l';
	finalMsg[s + k++] = 'l';
	}

	printf("Message - %s | To - %s | receiver - %d\n", finalMsg, toName, receiver);
	if(receiver == -2){
	  for(int z=0;z<5;z++){
	    if(clientArr[z] != clientArr[i]){
	      write(clientArr[z], finalMsg, strlen(finalMsg));
	    }
	  }
	}else{
	  write_sock(receiver, finalMsg);
	}

	return (char*)finalMsg[0];
}

int reset_fd(){
	FD_ZERO(&fr);
	FD_ZERO(&fe);
	FD_ZERO(&fw);
	
	FD_SET(sockfd, &fr);
	FD_SET(sockfd, &fe);
	int max_sd = sockfd;
	
	for(int i=0;i<5;i++){
		if(clientArr[i] != 0){
			FD_SET(clientArr[i], &fr);
			FD_SET(clientArr[i], &fe);
			if(clientArr[i] > max_sd){
				max_sd = clientArr[i];
			}
		}
	}
	
	return select(max_sd + 1, &fr, &fw, &fe, NULL);
}