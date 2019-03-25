#include "../../includes/client/client.h"

// TODO: delete all comments;
// TODO: defines to header;
// TODO: detect connection refused in UDP;
// TODO: handle overflow on port and delay;

// TODO: initialization_module.c;
// TODO: encription_module.c;

// gcc -Wall -Wextra -Werror -o client client.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
 
// TODO: clear out all comments;

// TODO: clear out Makefile -> -I$(INC_SERVER)

/* reverse:  reverse string s in place */


void	line_composer(unsigned char *dst, unsigned char *src)
{
	size_t		len = strlen( (char *)dst );
	size_t		copy_start = 0;
	// size_t		copy_len = 0;

	while (src[copy_start] == 0)
	// {
		copy_start++;
	// }

	printf("copy_start -> %zu\n", copy_start);

	memcpy(&dst[len], &src[copy_start], strlen( (char *)src ));
}

int main(int argc, char **argv)
{
	unsigned char	id[ID_SIZE] 		= { 0x0 };
	int				delay 				= 0x0; /* task documentation - specify intervals of input */
	short			port 				= 0x0; /* task documentation - specify intervals of input */
	unsigned char	*iterator			= NULL;
	// unsigned char 	padding				= 0x0;
	unsigned char 	initial_packet[256] = { 0x0 };

	if (!(validation_of_program_arguments(argc, argv)))
	{
		printf("%s\n", "usage: <id> <delay> <ip:port>");
		exit (0);
	}

	client_configuration(argv, id, &delay, &port, &iterator);

	printf("id    	 -> %s\n", id);
	printf("delay 	 -> %d\n", delay);
	printf("port  	 -> %d\n", port);
	printf("iterator -> %c\n", iterator[3]);

	// unsigned char *test1 = (unsigned char *)"test1";
	// unsigned char *test2 = (unsigned char *)"test2";
	// unsigned char *test3 = (unsigned char *)"test3";


	line_composer(initial_packet, id);
	// reverse(iterator);
	line_composer(initial_packet, iterator);
	// reverse(&initial_packet[8]);
	// line_composer(initial_packet, test3);



	printf("initial packet -> %s\n", initial_packet);

	// printf("test -> %c\n", '0');


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
    servaddr.sin_port = htons(port); 
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