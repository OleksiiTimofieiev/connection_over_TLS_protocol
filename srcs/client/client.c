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
// TODO: detect connection refused in UDP;

// TODO: initialization_module.c;
// TODO: encription_module.c;

// gcc -Wall -Wextra -Werror -o client client.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
 
// TODO: clear out all comments;

#define PORT     3333 
#define MAXLINE 1024

#define ID_SIZE 				8
#define INITIAL_ITERATOR_SIZE 	4
  
bool	validation_of_program_arguments(int argc, char **argv)
{
	if 		(argc != 4) 									
	{
		printf("error: %s\n", "Not full command arguments.");
		return (false);
	}
	else if (strlen(argv[1]) > 8)
	{
		printf("error: %s\n", "Client id has to be in limits of 8 chars and not NULL.");
		return (false);
	} 	
	else if (atoi(argv[2]) <= 0)
	{
		printf("error: %s\n", "Delay parameter has to be > 0.");
		return (false);
	}
	else if (atoi(&argv[3][3]) <= 0)
	{
		printf("error: %s\n", "Port number has to be > 0.");
		return (false);
	}
	return 	(true);
}

void	client_configuration(char **argv, char *id, int *delay, short *port, unsigned char **iterator)
{
	memcpy(id, argv[1], 8);

	/* 
	** usleep() takes microseconds, 
	** so we have to multiply the input by 1000 in order to sleep in milliseconds. 
	*/

	*delay		 = atoi(argv[2]) * 1000;
	*port 		 = atoi(&argv[3][3]);
	*iterator	 = (unsigned char *)malloc(sizeof(unsigned char) * INITIAL_ITERATOR_SIZE);
	
	bzero(*iterator, INITIAL_ITERATOR_SIZE);
}

int main(int argc, char **argv)
{
	char			id[ID_SIZE] 		= { 0x0 };
	int				delay 				= 0x0; /* task documentation - specify intervals of input */
	short			port 				= 0x0; /* task documentation - specify intervals of input */
	unsigned char	*iterator			= NULL;
	unsigned char 	padding				= 0x0;
	unsigned char 	initial_packet[256] = { 0x0 }; // later bzero it;

	if (!(validation_of_program_arguments(argc, argv)))
	{
		printf("%s\n", "usage: <id> <delay> <ip:port>");
		exit (0);
	}

	client_configuration(argv, id, &delay, &port, &iterator);

	printf("id    	 -> %s\n", id);
	printf("delay 	 -> %d\n", delay);
	printf("port  	 -> %d\n", port);
	printf("iterator -> %s\n", iterator);

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

    // int y = 0;

    char *buf ="test2";

    while (42)
    {
      sendto(sockfd, (const char *)buf, strlen(buf), 
          0, (const struct sockaddr *) &servaddr,  
              sizeof(servaddr)); 
      printf("Hello message sent.\n"); 
      
      usleep(delay);

      // y++;
      system("leaks -q client_app");
    }      
  
    close(sockfd); 
    return 0; 
} 