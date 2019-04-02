#include "../../includes/server/server.h"

/* used global variables for the signal interrupt handling and to avoid race condition */
extern t_data 					*l_data;
extern pthread_mutex_t 			mutex;
int 							sockfd;

void 	sig_handle(int signal);

int 	main(int argc, char **argv)
{
	/* **************************************** signal definition ************************************************************ */

	signal(SIGINT, &sig_handle);

	/* ***************************** init mutex to avoid race condition while writing to the linked list ********************* */

	pthread_mutex_init(&mutex, NULL);

	/* ***************************** basic input validation ****************************************************************** */

	if (argc != 3)
	{
		printf("Invalid quantity of arguments\nUsage: ./server_app <port> <quantity of threads>");
		exit(0);
	}

	/* ***************************** parsing of the command line ************************************************************* */

	short				port = atoi(argv[1]);
	int					number_of_threads = atoi(argv[2]);

	/* ***************************** init of thread pool ******************************************************************** */

	t_thread			thread_pool[number_of_threads];

	memset(thread_pool, 0x0, number_of_threads);

	/* ***************************** init of socket ************************************************************************ */

	struct 				sockaddr_in addr;
	unsigned char		buffer[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY];

	/* **************************************** socket initialization ***************************************************** */

	struct sockaddr_in servaddr, cliaddr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("Socket creation failed. ERROR: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);

	// Bind the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&servaddr,
				sizeof(servaddr)) != 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	int addr_len = sizeof(addr);

	int n = 0;

	/* **************************************** DATAGRAM RECV ROUTINE **************************************************** */

	while (42)
	{
		n = recvfrom(sockfd, (unsigned char *)buffer, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY,
					0, (struct sockaddr *)&cliaddr,
					(socklen_t *)&addr_len);
		if (n > 0)
		{
	/* **************************************** creation of the thread task ********************************************* */

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

			fptr = fopen("UDP_INPUT", "w");

			if (fptr == NULL)
			{
				printf("Error!");
				exit(1);
			}

			/* **************************************** output to file ********************************************** */

			int i = 0 ;

			t_data * ptr = l_data;

			while (ptr)
			{
				i = 0;
				while (i < (INITIAL_PACKET_SIZE + DIGEST_SIZE))
				{
					fprintf(fptr, "%.2x", ptr->data[i]);
					i++;
				}
				fprintf(fptr, "%s", "\n");
				ptr = ptr->next;
			}
			deleteList(&l_data);
			fclose(fptr);
		}
		exit(0);
	}
}