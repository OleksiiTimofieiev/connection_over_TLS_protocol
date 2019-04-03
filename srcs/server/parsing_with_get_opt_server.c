#include "server/server.h"

void	parse_cmd_line(int argc, char **argv, short *port, int *number_of_threads)
{
	char c;

	while ((c = getopt(argc, argv, "p:t:")) != -1)
	{
		switch (c)
		{
			case 'p':
			{
				*port = atoi(optarg);
				break;
			}
			case 't':
			{
				*number_of_threads = atoi(optarg);
				break;
			}
			default:
			{
				printf("Usage example: ./server_app -p 9999 -t 20\n");
				exit(0);
				break;
			}
		}
	}
}