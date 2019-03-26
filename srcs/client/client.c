#include "../../includes/client/client.h"

// TODO: delete all comments;
// TODO: detect connection refused in UDP;
// TODO: handle overflow on port and delay;

// gcc -Wall -Wextra -Werror -o client client.c -I/Users/otimofie/.brew/Cellar/libev/4.24/include -L/Users/otimofie/.brew/Cellar/libev/4.24/lib -lev
 
// TODO: clear out all comments;
// TODO: clear out Makefile -> -I$(INC_SERVER)
// TODO: what if < 8;

/* declared it global for the sig_handle function */
int 					sockfd; 

struct sockaddr_in 		servaddr;

/* declared it global for the sig_handle function */
unsigned char			string_iterator[MAX_ITERATOR_SIZE];

void  	sig_handle(int sgnal);
void	init_socket(short port);

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

	unsigned char key[32] = { 0 };

	aes_key_generation(key);

	// print key
	int i = 0;
	printf("\nInitial key -> ");

	while (i < 32)
	{
		printf("%x ", key[i++]);
	}
    printf("\n");

	/* *************************************** hash checksum variable *************************** */

	unsigned char digest[DIGEST_SIZE];


   	/* *************************************** packet + checksum ********************************* */

	unsigned char initial_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = { 0 };

	/* *************************************** encryption ********************************* */

	unsigned char encrypted_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = { 0 };
	unsigned char encrypted_full_packet_with_rsa_key[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY] = { 0 };


	 unsigned char iv_buf[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};
	
	unsigned char iv[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};

	// unsigned char decrypted_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = { 0 };

	unsigned char encrypted_key_with_rsa[256];

    // for( i = 0; i < 16; i++ )
    //     mbedtls_printf( "%02x", digest[i] );

    // printf("\n");

    // if( ( ret = mbedtls_sha1_ret( initial_packet, 256, digest ) ) != 0 )
    //     return( MBEDTLS_EXIT_FAILURE );

    // for( i = 0; i < 16; i++ )
    //     mbedtls_printf( "%02x", digest[i] );

    // printf("\n");
	// unsigned char decrypted_key_with_rsa[32];


    // rsa_decrypt(encrypted_key_with_rsa, decrypted_key_with_rsa);

    // int j = 0;

	// while (j < 32)
	// {
	// 	printf("%x ", decrypted_key_with_rsa[j]);;
    //     j++;
	// }
    // printf("\n");


	int delete = 0;

	// bzero on every step;

    while (42)
    {
 		counter_line_composer(initial_packet, string_iterator);
 		
		sha1_checksum_generation(digest, initial_packet);

		memcpy(initial_full_packet, initial_packet, 256);
		memcpy(&initial_full_packet[256], digest, 16);


		memcpy(iv, iv_buf, 16);

		aes_encrypt(iv, key, initial_full_packet, encrypted_full_packet);

		// int i = 0;

		// while (i < (INITIAL_PACKET_SIZE + DIGEST_SIZE))
  //    		printf("%x ", initial_full_packet[i++]);
  //    	printf("\n");

		// memcpy(iv, iv_buf, 16);

    	rsa_encrypt(key, encrypted_key_with_rsa);

		memcpy(encrypted_full_packet_with_rsa_key, encrypted_full_packet, INITIAL_PACKET_SIZE + DIGEST_SIZE);
		memcpy(&encrypted_full_packet_with_rsa_key[INITIAL_PACKET_SIZE + DIGEST_SIZE], encrypted_key_with_rsa, LEN_OF_ENCPYPTED_AES_KEY);

		// for (int i = 0; i < INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY; i++)
		// 	printf("%x ", encrypted_full_packet_with_rsa_key[i]);

		printf("\nInitial key -> ");

		i = 0;
		while (i < 32)
		{
			printf("%x ", key[i++]);
		}
		printf("\n");

		// aes_decrypt(iv_buf, key, encrypted_full_packet, decrypted_full_packet);

		i = 0;

		// printf("\ndecrypted -> ");
		// while (i < (INITIAL_PACKET_SIZE + DIGEST_SIZE))
     	// 	printf("%x ", decrypted_full_packet[i++]);
     	// printf("\n");

		sendto(sockfd, (const unsigned char *)encrypted_full_packet_with_rsa_key, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

		add_to_string(string_iterator);
 		
 		counter_line_composer(initial_packet, string_iterator);

      	usleep( delay / 1000);

      	delete++;

      	if (delete == 4)
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
