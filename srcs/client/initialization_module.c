#include "client/client.h"

bool	validation_of_program_arguments(int argc, char **argv)
{
	if 		(argc != 4) 									
	{
		printf("error: %s\n", "Not full command arguments.");
		return (false);
	}
	else if (strlen(argv[1]) > 8)
	{
		printf("error: %s\n", "Client id has to be in limits of 8 chars and not NULL.");
		return (false);
	} 	
	else if (atoi(argv[2]) <= 0)
	{
		printf("error: %s\n", "Delay parameter has to be > 0.");
		return (false);
	}
	return 	(true);
}

void	client_configuration(char **argv, unsigned char *initial_packet, int *delay, short *port, unsigned char *iterator, char **ip)
{
	char *token = NULL;
	char delim[] = ":";

	memset(initial_packet, 0x0, 8);
	memcpy(initial_packet, argv[1], strlen(argv[1]));

	/* 
	** usleep() takes microseconds, 
	** so we have to multiply the input by 1000 in order to sleep in milliseconds. 
	*/

	token = strtok(argv[3], delim);

	if (!(*ip = (char *)malloc(sizeof(char) * (strlen(token) + 1))))
		*ip = NULL;

	strcpy(*ip, token);
	
	token = strtok(NULL, delim);
	
	*port = atoi(token);
	*delay = atoi(argv[2]) * 1000;

	memset(iterator, 0x0, MAX_ITERATOR_SIZE);
	iterator[MAX_ITERATOR_SIZE - 1] = '0';
}