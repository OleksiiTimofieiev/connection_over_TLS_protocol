#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <stdbool.h> 

// TODO: delete all comments;
// TODO: defines to header;

// gcc -Wall -Wextra -Werror -o client client.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
  
#define PORT     3333 
#define MAXLINE 1024

#define ID_SIZE 8
  
bool	validation_of_program_arguments(int argc, char **argv)
{
	if (argc != 4 || strlen(argv[1]) > 8)
		return (false);
	return (true);
}

void	init_client_socket_configuration(char **argv, char *id, int *delay, short *port)
{
	memcpy(id, argv[1], 8);
	/* usleep() takes microseconds, 
	so we have to multiply the input by 1000 in order to sleep in milliseconds. */
	*delay = atoi(argv[2]) * 1000;
	*port = atoi(&argv[3][3]);
}

int main(int argc, char **argv)
{
	char	id[ID_SIZE] = { 0x0 };
	int		delay; /* task documentation - specofy intervals */
	short	port; /* task documentation - specofy intervals */

	printf("%s\n", "usage: <id[0:8]> <delay[ms]> <ip:port>");

	if (!(validation_of_program_arguments(argc, argv)))
	{
		printf("%s\n", "Invalide command arguments.");
		printf("%s\n", "usage: <id[0:8]> <delay[ms]> <ip:port>");
		exit (0);
	}

	init_client_socket_configuration(argv, id, &delay, &port);

	printf("id    -> %s\n", id);
	printf("delay -> %d\n", delay);
	printf("port  -> %d\n", port);


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
      
      usleep(3000000);

      y++;
    }      
  
    close(sockfd); 
    return 0; 
} 