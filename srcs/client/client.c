#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

// gcc -Wall -Wextra -Werror -o client client.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
  
#define PORT     3333 
#define MAXLINE 1024 
  
// Driver code 
int main() { 
    int sockfd; 
    // char buffer[MAXLINE]; 
    // char *hello = "Hello from client"; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    // int n, len; 

    int y = 0;

    char *buf ="test2";

    while (y < 10)
    {
      sendto(sockfd, (const char *)buf, strlen(buf), 
          0, (const struct sockaddr *) &servaddr,  
              sizeof(servaddr)); 
      // printf("Hello message sent.\n"); 
      // usleep() takes microseconds, so you will have to multiply the input by 1000 in order to sleep in milliseconds.
      usleep(3000000);

      y++;
    }      
  
    close(sockfd); 
    return 0; 
} 