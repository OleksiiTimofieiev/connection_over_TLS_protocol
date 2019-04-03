#include "server/server.h"

/* used global variables for the signal interrupt handling and to avoid race condition */
extern t_data 					*l_data;
extern pthread_mutex_t 			mutex;
pthread_mutex_t 				mutex_main;

int 							sockfd;

void 	sig_handle(int signal);

int 	main(int argc, char **argv)
{
	/* ***************************** parsing of the command line ************************************************************* */

	short				port = 0;
	int					number_of_threads = 0;

	parse_cmd_line(argc, argv, &port, &number_of_threads);

	/* **************************************** signal definition ************************************************************ */

	signal(SIGINT, &sig_handle);

	/* ***************************** init mutex to avoid race condition while writing to the linked list ********************* */

	pthread_mutex_init(&mutex, NULL);

	


	/* ***************************** init of thread pool ******************************************************************** */

	t_thread			thread_pool[number_of_threads];

	memset(thread_pool, 0x0, number_of_threads);


	/* ***************************** init of socket ************************************************************************ */

	struct 				sockaddr_in addr;

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
		printf("Bind failed. ERROR: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int addr_len = sizeof(addr);

	int n = 0;

	/* **************************************** DATAGRAM RECV ROUTINE **************************************************** */

	unsigned char		buffer[INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY];

	while (42)
	{
		n = recvfrom(sockfd, (unsigned char *)buffer, INITIAL_PACKET_SIZE + DIGEST_SIZE + LEN_OF_ENCPYPTED_AES_KEY,
					0, (struct sockaddr *)&cliaddr,
					(socklen_t *)&addr_len);
		if (n > 0)
		{
	
	/* **************************************** creation of the thread task ********************************************* */
			pthread_mutex_lock(&mutex_main);

			thread_create(thread_pool, buffer, number_of_threads);
			
			pthread_mutex_unlock(&mutex_main);

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
		pthread_mutex_destroy(&mutex_main);

		if (l_data)
		{
			/* **************************************** output to file ********************************************** */

			print_to_file(l_data);
			
			deleteList(&l_data);
		}
		exit(0);
	}
}