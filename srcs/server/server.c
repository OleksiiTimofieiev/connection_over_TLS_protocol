#include "../../includes/server/server.h"

// TODO: add validation;

/* used global variables for the signal interrupt handling */
t_data 					*data = NULL;
int 					sockfd;

void sig_handle(int signal);

int main(int argc, char **argv)
{
	struct 				sockaddr_in addr;
	unsigned char		buffer[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY];
	unsigned char		aes_key[32];
	unsigned char		decrypted_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = {0};
	const unsigned char iv_1[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};
	
	if (argc != 3)
		exit(0);
	short				port = atoi(argv[1]);
	int					number_of_threads = atoi(argv[2]);

	printf("%d %d", port, number_of_threads);

	/* **************************************** signal definition ******************************** */

	signal(SIGINT, &sig_handle);

	struct sockaddr_in servaddr, cliaddr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	// Bind the socket with the server address
	if (bind(sockfd, (struct sockaddr *)&servaddr,
				sizeof(servaddr)) != 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int addr_len = sizeof(addr);

	int n = 0;

	while (42)
	{
		n = recvfrom(sockfd, (unsigned char *)buffer, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY,
					0, (struct sockaddr *)&cliaddr,
					(socklen_t *)&addr_len);
		if (n > 0)
		{
			unsigned char iv[16];

			memcpy(iv, iv_1, 16);

			rsa_decrypt(&buffer[INITIAL_PACKET_SIZE + DIGEST_SIZE], aes_key);

			aes_decrypt(iv, aes_key, buffer, decrypted_full_packet);

			unsigned char checksum[DIGEST_SIZE];

			mbedtls_sha1_ret(decrypted_full_packet, 256, checksum);

			if (check_sha1_sum(&decrypted_full_packet[256], checksum))
				push_front(&data, decrypted_full_packet);
			else
				push_front(&data, (unsigned char *)"error");

			n = 0;
		}
	}
	return 0;
}

void	sig_handle(int signal)
{
	if (signal == SIGINT)
	{
		close(sockfd);

		if (data)
		{
			FILE *fptr = NULL;

			fptr = fopen("UDP_INPUT.txt", "wb");

			if (fptr == NULL)
			{
				printf("Error!");
				exit(1);
			}

			int i = 0 ;

			while (data)
			{
				i = 0;
				while (i < INITIAL_PACKET_SIZE + DIGEST_SIZE)
					fprintf(fptr, "%.2x", data->data[i++]);
				fprintf(fptr, "%s", "\n");
				data = data->next;
			}

			fclose(fptr);
		}
		exit(0);
	}
}