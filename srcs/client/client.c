#include "../../includes/client/client.h"

// TODO: delete all comments;
// TODO: defines to header;
// TODO: detect connection refused in UDP;
// TODO: handle overflow on port and delay;

// TODO: encription_module.c;

// gcc -Wall -Wextra -Werror -o client client.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
 
// TODO: clear out all comments;
// TODO: clear out Makefile -> -I$(INC_SERVER)

/* declared it global for the sig_handle function */
int 					sockfd; 

struct sockaddr_in 		servaddr;

/* declared it global for the sig_handle function */
unsigned char			string_iterator[MAX_ITERATOR_SIZE];

void  	sig_handle(int sgnal);
void	init_socket(short port);

void	aes_key_generation(unsigned char *key) // container for the key ;)
{
	mbedtls_ctr_drbg_context 	ctr_drbg;

	mbedtls_entropy_context 	entropy;

	memset(key, 0, 32);

	char *pers = "and here comes some line for key generation"; // random line for the key generation ;
	
	int ret;

	mbedtls_entropy_init( &entropy );

	mbedtls_ctr_drbg_init( &ctr_drbg );

	if( ( ret = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, (unsigned char *) pers, strlen( pers ) ) ) != 0 )
	    printf( " failed\n ! mbedtls_ctr_drbg_init returned -0x%04x\n", -ret );

	if( ( ret = mbedtls_ctr_drbg_random( &ctr_drbg, key, 32 ) ) != 0 )
	    printf( " failed\n ! mbedtls_ctr_drbg_random returned -0x%04x\n", -ret );
}

int		main(int argc, char **argv)
{
	/* **************************************** signal definition ******************************** */

	signal(SIGINT, &sig_handle);

	/* **************************************** configuration variables ************************** */

	int				delay 								= 0; /* task documentation - specify intervals of input */
	short			port 								= 0; /* task documentation - specify intervals of input */
	unsigned char 	initial_packet[INITIAL_PACKET_SIZE] = { 0 };

	/* **************************************** validation *************************************** */	

	if (!(validation_of_program_arguments(argc, argv)))
	{
		printf("%s\n", "usage: <id> <delay> <ip:port>");
		exit (0);
	}

	/* **************************************** configuration routines ************************** */

	client_configuration(argv, initial_packet, &delay, &port, string_iterator);

	/* *************************************** socket initializatiion *************************** */
	
	init_socket(port);

	/* *************************************** aes key generation ******************************* */

	unsigned char key[32];

	aes_key_generation(key);

	// 	int j = 0;

	// printf("key -> ");

	// while (j < 32)
	// {
	// 	printf("%d ", key[j++]);
	// }

	// printf("\n");

	/* *************************************** hash ******************************* */

	int i;
	int ret;
	unsigned char digest[16];

    if( ( ret = mbedtls_sha1_ret( initial_packet, 256, digest ) ) != 0 )
        return( MBEDTLS_EXIT_FAILURE );

    for( i = 0; i < 16; i++ )
        mbedtls_printf( "%02x", digest[i] );

    printf("\n");

    if( ( ret = mbedtls_sha1_ret( initial_packet, 256, digest ) ) != 0 )
        return( MBEDTLS_EXIT_FAILURE );

    for( i = 0; i < 16; i++ )
        mbedtls_printf( "%02x", digest[i] );

    printf("\n");

	int delete = 0;

    while (42)
    {
 		counter_line_composer(initial_packet, string_iterator);
 		
     	printf("%s\n", initial_packet);

     	sendto(sockfd, (const unsigned char *)initial_packet, INITIAL_PACKET_SIZE, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr)); 
 		
 		add_to_string(string_iterator);
 		
 		counter_line_composer(initial_packet, string_iterator);

      	usleep( delay / 1000);

      	delete++;

      	if (delete == 5)
      		break ;

     	// system("leaks -q client_app");
    }      
  
    close(sockfd); 

    return (0); 
} 

void  sig_handle(int signal)
{
	if (signal == SIGINT)
	{
		close(sockfd);

		size_t i = 0;
		
		while (!isdigit(string_iterator[i]))
			i++;

		printf("\nPackets have been sent: %s\n", &string_iterator[i]);

		exit(0);
	}
}

void	init_socket(short port)
{
  
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
}
