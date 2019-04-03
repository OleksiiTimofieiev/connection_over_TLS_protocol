#include "client/client.h"

void 	client_configuration(char argc, char **argv, unsigned char *initial_packet, int *delay, short *port, unsigned char *iterator, char **ip_address)
{
	char c;
	char *user = NULL;

	while ((c = getopt(argc, argv, "u:d:i:p:")) != -1)
	{
		switch (c)
		{
			case 'd':
			{
				*delay = atoi(optarg);
				break;
			}
			case 'u':
			{
				user = strdup(optarg);
				memset(initial_packet, 0x0, 8);
				memcpy(initial_packet, user, 8);
				free(user);
				break;
			}
			case 'i':
			{
				*ip_address = strdup(optarg);
				break;
			}
			case 'p':
			{
				*port = atoi(optarg);
				break;
			}
			default:
			{
				printf("Usage example: ./client_app -u xxx -d 1 -i 127.0.0.1 -p 9999\n");
				exit(0);
				break;
			}
		}
	}
	
	memset(iterator, 0x0, MAX_ITERATOR_SIZE);
	iterator[MAX_ITERATOR_SIZE - 1] = '0';
}