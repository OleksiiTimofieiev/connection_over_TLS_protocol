#include "../../includes/server/server.h"

// TODO: add validation;
// TODO: delete all comments;

/* used global variables for the signal interrupt handling */
t_data 					*l_data = NULL;
pthread_mutex_t 		mutex;
int 					sockfd;

void 	sig_handle(int signal);

void	*handle_new_data( void *data )
{
	

	unsigned char aes_key[32];
	unsigned char decrypted_full_packet[INITIAL_PACKET_SIZE + DIGEST_SIZE] = {0};
	const unsigned char iv_1[16] = {0xb6, 0x58, 0x9f, 0xc6, 0xab, 0x0d, 0xc8, 0x2c, 0xf1, 0x20, 0x99, 0xd1, 0xc2, 0xd4, 0x0a, 0xb9};

	t_thread_data *input = (t_thread_data *)data;

	unsigned char iv[16];

	memcpy(iv, iv_1, 16);

	rsa_decrypt(&input->data[INITIAL_PACKET_SIZE + DIGEST_SIZE], aes_key);

	aes_decrypt(iv, aes_key, input->data, decrypted_full_packet);

	unsigned char checksum[DIGEST_SIZE];

	mbedtls_sha1_ret(decrypted_full_packet, 256, checksum);

	pthread_mutex_lock(&mutex);

	if (check_sha1_sum(&decrypted_full_packet[256], checksum))
		push_front(&l_data, decrypted_full_packet);
	else
		push_front(&l_data, (unsigned char *)"error");

	pthread_mutex_unlock(&mutex);

	input->current_status = FINISHED;

	return (NULL);
}

void thread_create(t_thread *thread_pool, unsigned char *buffer, int threads_limit)
{
	int ret, i;

	/*
     * Find in-active or finished thread slot
     */

	for (i = 0; i < threads_limit; i++)
	{
		if (thread_pool[i].data.current_status == INACTIVE)
			break;

		if (thread_pool[i].data.current_status == FINISHED)
		{
			// mbedtls_printf("  [ main ]  Cleaning up thread %d\n", i);
			pthread_join(thread_pool[i].thread, NULL);
			break;
		}
		printf("thread status -> %d \n", thread_pool[i].data.current_status);
	}

	if (i == threads_limit)
	{
		printf("%s\n", "thread error");
		exit(0);
	}

	/*
     * Fill thread-info for thread
     */

	memcpy(thread_pool[i].data.data, buffer, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY);
	
	thread_pool[i].data.current_status = RUNNING;
	// memcpy(&threads[i].data.client_fd, client_fd, sizeof(mbedtls_net_context));

	if ((ret = pthread_create(&thread_pool[i].thread, NULL, handle_new_data, &thread_pool[i].data)) != 0)
	{
		printf("%s\n", "thread creation error");
		exit(0);
	}

	// return (0);
}

int main(int argc, char **argv)
{
	pthread_mutex_init(&mutex, NULL);

	struct 				sockaddr_in addr;
	unsigned char		buffer[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY];
	
	if (argc != 3)
		exit(0);

	short				port = atoi(argv[1]);
	int					number_of_threads = atoi(argv[2]);

	t_thread			thread_pool[number_of_threads];

	int i = 0;

	while( i < number_of_threads )
	{
		thread_pool[i].data.current_status = INACTIVE;
		i++;
	}
	



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
			thread_create(thread_pool, buffer, number_of_threads);

			n = 0;
		}
	}
	return (0);
}

void	sig_handle(int signal)
{
	if (signal == SIGINT)
	{
		close(sockfd);
		pthread_mutex_destroy(&mutex);

		if (l_data)
		{
			FILE *fptr = NULL;

			fptr = fopen("UDP_INPUT.txt", "wb");

			if (fptr == NULL)
			{
				printf("Error!");
				exit(1);
			}

			int i = 0 ;

			while (l_data)
			{
				i = 0;
				while (i < INITIAL_PACKET_SIZE + DIGEST_SIZE)
				{
					fprintf(fptr, "%.2x", l_data->data[i++]);
				}

				printf("%s\n", l_data->data); // delete

				fprintf(fptr, "%s", "\n");

				l_data = l_data->next;
			}
			fclose(fptr);
		}
		exit(0);
	}
}