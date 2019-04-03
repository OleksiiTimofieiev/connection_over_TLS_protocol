#include "server/server.h"

void	print_to_file(t_data *ptr)
{
	FILE 	*fptr 	= NULL;
	int 	i 		= 0;

	fptr = fopen("UDP_INPUT", "w");

	if (fptr == NULL)
	{
		printf("Error!");
		exit(1);
	}
	

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

	fclose(fptr);
}