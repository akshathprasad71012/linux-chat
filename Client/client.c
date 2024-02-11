#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>    //def hostent struct which stores info about server
#include <signal.h>
#include <stdbool.h>
#include <libnotify/notify.h>



int child_pid = -1;
int sockfd;
char wbuffer[255], rbuffer[255];
int loggedIn = 0;

void signal_handler(int signal){
	close(sockfd);
	printf("\nExiting...\n");
	exit(0);
}

void Write(int sockfd){
	int n;
	
	if(!loggedIn){
	  //printf("Your Name : ");
	}else{
		bzero(wbuffer, 255);
		//printf("MSG  : ");
		fgets(wbuffer, 255, stdin);
	 }
	
	//scanf("Your Message %s", wbuffer);
	
	n = write(sockfd, wbuffer, strlen(wbuffer));
	if(n<0){
		printf("write error");
	}
	bzero(wbuffer, 255);
	printf("\n");
	
	loggedIn = 1;
	
}

int main(int argc, char *argv[]){
	
	int portno, n;
	
	struct sockaddr_in serv_addr;        //netinet address families
	struct hostent *server;
	
	bool libnotifyInitStatus = false;
	NotifyNotification *notifyHandle;

	libnotifyInitStatus = notify_init("Chat");
	if(!libnotifyInitStatus){
		printf("libnotify err\n");
		exit(1);
	}
	
	bzero(wbuffer, 255);
	bzero(rbuffer, 255);
		
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("Error opening Socket");
		exit(1);
	}
	
	
	portno = atoi(argv[2]);                         //str to int in stdlib.h
	
	server = gethostbyname(argv[1]);
	if(server == NULL){
		printf("Host not found");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server -> h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	
	printf("Your name : ");
	fgets(wbuffer, 255, stdin);
	
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0){
		printf("Connect failed");
	}else{
		printf("\nConnection Successfull\n");
	}
	
	
	/*
	bzero(wbuffer, 255);
	printf("Your Name : ");
	fgets(wbuffer, 255, stdin);
	//scanf("Your Message %s", wbuffer);
	
	n = write(sockfd, wbuffer, strlen(wbuffer));
	if(n<0){
		printf("write error");
	}
	bzero(wbuffer, 255);*/
	
	while(1){
		signal(SIGINT, signal_handler);
		
		if(child_pid != -1 && child_pid != 0){
			//printf("%d killed\n", child_pid);
			kill(child_pid, SIGKILL);
		}
		
		child_pid = fork();
		//printf("%d", child_pid);
		if (child_pid > 0) {
			while(1){
			 	Write(sockfd);
			}
		}
		else if (child_pid == 0){
		while(1){
		 	bzero(rbuffer, 255);
			//printf("Reading Now \n");
			n = read(sockfd, rbuffer, 255);
			if(n<0){
				printf("read error");
			}
			if(1)
				printf("\n%s\n\n", rbuffer);
				notifyHandle = notify_notification_new("New Chat", rbuffer, "dialog_information");
				if(notifyHandle != NULL){
					
					notify_notification_show(notifyHandle, NULL);
				}else{
					printf("\nNotifyHandle NULL\n");
				}
			}	
			break;
		}
	}
		
		
		
	
	close(sockfd);
	
	
	return 0;
	
}
