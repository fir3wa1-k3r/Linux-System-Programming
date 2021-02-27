 // Author: Yashwanth KL                                                         
 // Description: Simple echo server                                           
 // Compiling syntaxt:
 //     # gcc -pthread core.c -o core
                                                                              
#include <stdio.h>                                                           
#include <stdlib.h>
#include <unistd.h>                                                          
#include <sys/types.h>                                                       
#include <sys/socket.h>                                                     
#include <netinet/in.h> 
#include <netinet/ip.h>
#include <pthread.h>
#include <string.h>
                                                                              
#define PORT 1337                                                            
#define BANNER "Heyyaa! Good to see you\n"                                     
#define DEBUG 0
#define BUF_SIZE 10

void *conn_handler(void *tmp)
{	
	char buff[BUF_SIZE];
	int acptdfd = *((int *)tmp);
	unsigned int num_bytes;

	num_bytes = write(acptdfd, BANNER, sizeof(BANNER));
	for(;;){
		read(acptdfd, buff, BUF_SIZE);
		write(acptdfd, buff, BUF_SIZE);
		memset(buff, 0, BUF_SIZE);
	}
	close(acptdfd);	
	
}
                                                                              
int main(int argc, char *argv[])                                             
{                                                                            
        int sockfd, bindret, acptdfd, addrlen;
        struct sockaddr_in servaddr, clnt;
	pthread_t thread;
                                                                             
        printf("Initializing...\n");                                         
                                                                             
        sockfd = socket(AF_INET, SOCK_STREAM, 0);                               
        if(sockfd < 0)                                                       
        {                                                                    
                perror("socket");                                            
                exit(1);                                                     
        }
      
	#if DEBUG == 1
        printf("[+] Socket successfully created\n");                        
	#endif
	
	// Initialize struct sockaddr_in
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bindret = bind(sockfd, (struct sockaddr_in *)&servaddr, sizeof(servaddr));
	if(bindret != 0){
		perror("bind");
		exit(1);
	} 

	if(listen(sockfd, 3)) {
		perror("listen");
		exit(1);
	}

	printf("Listening for incoming connections\n");

	//Accept the incomming connections
	
	addrlen = sizeof(clnt);
	for(;;){
		acptdfd = accept(sockfd, (struct sockaddr_in *)&clnt, &addrlen);
		if(acptdfd < 0)	{
			perror("accept");
			exit(1);
		}

	//Create and join thread
		pthread_create(&thread, NULL, (void *)conn_handler, (void *)&acptdfd);
		pthread_join(thread, NULL);
	}

	close(sockfd);

        return 0;                                                            
}          
