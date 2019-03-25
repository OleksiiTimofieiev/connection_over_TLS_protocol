#include "../../includes/client/client.h"

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
	else if (atoi(&argv[3][3]) <= 0)
	{
		printf("error: %s\n", "Port number has to be > 0.");
		return (false);
	}
	return 	(true);
}

void	client_configuration(char **argv, char *id, int *delay, short *port, unsigned char **iterator)
{
	memcpy(id, argv[1], 8);

	/* 
	** usleep() takes microseconds, 
	** so we have to multiply the input by 1000 in order to sleep in milliseconds. 
	*/

	*delay		 = atoi(argv[2]) * 1000;
	*port 		 = atoi(&argv[3][3]);
	*iterator	 = (unsigned char *)malloc(sizeof(unsigned char) * INITIAL_ITERATOR_SIZE);
	
	bzero(*iterator, INITIAL_ITERATOR_SIZE);
	
	*iterator[3] = '0';
}