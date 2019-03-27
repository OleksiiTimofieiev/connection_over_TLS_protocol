#include "../../includes/client/client.h"


/* declared variables below global for the sig_handle function */

int 					sockfd; 
struct sockaddr_in 		servaddr;
unsigned char			string_iterator[MAX_ITERATOR_SIZE];

void  	sig_handle(int sgnal);
void	init_socket(short port);

void	print_size(unsigned char *str, int size)
{
	int i = 0;

	while(i < size)
	{
		printf("%.2x", str[i]);
		i++;
	}
	
}

int		main(int argc, char **argv)
{
	/* **************************************** signal definition ******************************** */

	signal(SIGINT, &sig_handle);

	/* **************************************** configuration variables ************************** */

	int				delay 								= 0;
	short			port 								= 0;
	unsigned char 	initial_packet[INITIAL_PACKET_SIZE];

	bzero(initial_packet, INITIAL_PACKET_SIZE);

	/* **************************************** validation *************************************** */	

	if (!(validation_of_program_arguments(argc, argv)))
	{
		printf("%s\n", "usage: ./client_app <id> <delay> <ip:port>");
		exit (0);
	}

	/* **************************************** configuration routines ************************** */

	client_configuration(argv, initial_packet, &delay, &port, string_iterator);

	print_size(string_iterator, MAX_ITERATOR_SIZE);

	printf("\nstop2\n");

	/* *************************************** socket initializatiion *************************** */
	
	init_socket(port);

	/* *************************************** aes key generation ******************************* */

	unsigned char key[32] = { 0 };

	aes_key_generation(key);

	/* *************************************** hash checksum variable *************************** */

	unsigned char digest[DIGEST_SIZE];

   	/* *************************************** packet + checksum ********************************* */

	unsigned char initial_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE];
	bzero(initial_full_packet, INITIAL_PACKET_SIZE + DIGEST_SIZE);

	/* *************************************** encryption ********************************* */

	unsigned char encrypted_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = {0};
	unsigned char encrypted_full_packet_with_rsa_key[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY] = { 0 };
	unsigned char iv_buf[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};
	unsigned char iv[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};
	unsigned char encrypted_key_with_rsa[256];

	print_size(string_iterator, MAX_ITERATOR_SIZE);

	printf("\nstop2\n");

	int n = 0;

	// memset(string_iterator, 0x0, MAX_ITERATOR_SIZE);

	// print_size(string_iterator, MAX_ITERATOR_SIZE);

	print_size(string_iterator, MAX_ITERATOR_SIZE);

	printf("\nstop2\n");

	while (42)
    {
		/* adding value of the string iterator to the packet */
 		counter_line_composer(initial_packet, string_iterator);

		print_size(initial_packet, INITIAL_PACKET_SIZE);
 		
		/* generation of the checksum for the packet */
		sha1_checksum_generation(digest, initial_packet);

		/* composing the packet */
		memcpy(initial_full_packet, initial_packet, 256);
		memcpy(&initial_full_packet[256], digest, 16);

		memcpy(iv, iv_buf, 16);

		/* encryption of the packet */
		aes_encrypt(iv, key, initial_full_packet, encrypted_full_packet);

		/* encryption of the aes key */
    	rsa_encrypt(key, encrypted_key_with_rsa);

		/* composition of the encrypted packet */
		memcpy(encrypted_full_packet_with_rsa_key, encrypted_full_packet, INITIAL_PACKET_SIZE + DIGEST_SIZE);
		memcpy(&encrypted_full_packet_with_rsa_key[INITIAL_PACKET_SIZE + DIGEST_SIZE], encrypted_key_with_rsa, LEN_OF_ENCPYPTED_AES_KEY);

		sendto(sockfd, (unsigned char *)encrypted_full_packet_with_rsa_key, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

		/* +1 to the string iterator */
		add_to_string(string_iterator);
 		
		 /* adding new value of the string iterator to the string */
 		counter_line_composer(initial_packet, string_iterator);

		/* delay for the process */
      	usleep(delay);

		  n++;

		  if (n == 1)
		  	break ;
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
