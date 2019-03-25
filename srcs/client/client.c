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

size_t	str_len(unsigned char *str)
{
	size_t 	i = MAX_ITERATOR_SIZE;

	while (i)
	{
		if (str[i] == 0)
			break;
		i--;
			
	}
	return (i);
}

void	add_to_string(unsigned char *str)
{
	if (str[MAX_ITERATOR_SIZE - 1] <= '8')
		str[MAX_ITERATOR_SIZE - 1] += 1;
	else
	{
		size_t 	length_till_end = str_len(str) - 1;

		while (length_till_end)
		{
			if (str[length_till_end] <= '8')
			{
				if (str[length_till_end] == 0)
					str[length_till_end] = '1';
				else
					str[length_till_end] += 1;
				break ;
			}
			else
				str[length_till_end] = '0';
			length_till_end--;
		}
	}
}


void	line_composer(unsigned char *dst, unsigned char *src)
{
	// size_t		len = strlen( (char *)dst );
	size_t		copy_start = 0;
	size_t		i = 0;

	while (i < MAX_ITERATOR_SIZE)
	{
		if (src[i] != 0)
			break ;
		i++;
	}

	memcpy(&dst[8], &src[i], MAX_ITERATOR_SIZE - copy_start);
}

int main(int argc, char **argv)
{
	unsigned char	id[ID_SIZE] 						= { 0 };
	int				delay 								= 0; /* task documentation - specify intervals of input */
	short			port 								= 0; /* task documentation - specify intervals of input */
	unsigned char 	initial_packet[INITIAL_PACKET_SIZE] = { 0 };
	unsigned char	iterator[MAX_ITERATOR_SIZE];

	if (!(validation_of_program_arguments(argc, argv)))
	{
		printf("%s\n", "usage: <id> <delay> <ip:port>");
		exit (0);
	}

	client_configuration(argv, id, &delay, &port, iterator);
	

	/* remaster */
	memcpy(initial_packet, id, ID_SIZE);


	// printf("id    	 -> %s\n", id);
	// printf("delay 	 -> %d\n", delay);
	// printf("port  	 -> %d\n", port);
	// printf("iterator -> %d\n", iterator[3]);



	

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
      

    int i = 0;
    while (42)
    {

     	sendto(sockfd, (const unsigned char *)initial_packet, INITIAL_PACKET_SIZE, 
         			0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
 		
 		line_composer(initial_packet, iterator);
 		
     	printf("%s\n", initial_packet);
 		add_to_string(iterator);
 		line_composer(initial_packet, iterator);

      	usleep( delay / 1000);

     	// system("leaks -q client_app");
      	i++;
      	if (i == 5)
      		break;
    }      
  
    close(sockfd); 
    return 0; 
} 