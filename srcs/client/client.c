#include "../../includes/client/client.h"

// TODO: delete all comments;
// TODO: defines to header;
// TODO: detect connection refused in UDP;
// TODO: handle overflow on port and delay;

// TODO: encription_module.c;

// gcc -Wall -Wextra -Werror -o client client.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
 
// TODO: clear out all comments;
// TODO: clear out Makefile -> -I$(INC_SERVER)

int		main(int argc, char **argv)
{
	/* **************************************** configuration variables ***************************/

	int				delay 								= 0; /* task documentation - specify intervals of input */
	short			port 								= 0; /* task documentation - specify intervals of input */
	unsigned char 	initial_packet[INITIAL_PACKET_SIZE] = { 0 };
	unsigned char	string_iterator[MAX_ITERATOR_SIZE];

	/*********************************************** validation **********************************/	

	if (!(validation_of_program_arguments(argc, argv)))
	{
		printf("%s\n", "usage: <id> <delay> <ip:port>");
		exit (0);
	}

	/***************************************** configuration routines ***************************/

	client_configuration(argv, initial_packet, &delay, &port, string_iterator);

	/**************************************** socket initializatiion ****************************/
	
    int 					sockfd; 
    struct sockaddr_in 		servaddr; 
  
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    int i = 0;

    while (42)
    {
 		counter_line_composer(initial_packet, string_iterator);
 		
     	printf("%s\n", initial_packet);

     	sendto(sockfd, (const unsigned char *)initial_packet, INITIAL_PACKET_SIZE, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
 		
 		add_to_string(string_iterator);
 		
 		counter_line_composer(initial_packet, string_iterator);

      	usleep( delay / 1000);

     	// system("leaks -q client_app");
      	i++;
      	if (i == 5)
      		break;
    }      
  
    close(sockfd); 
    return 0; 
} 