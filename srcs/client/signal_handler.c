#include "../../includes/client/client.h"

void  sig_handle(int sig)
{
	if (sig == SIGINT)
	{
		size_t i = 0;

		while (!isdigit(string_iterator[i]))
			i++;

    	printf("\nPackets sent -> %s.\n", &string_iterator[i]);

    	close(sockfd); 
    	exit(0);
	}
}
