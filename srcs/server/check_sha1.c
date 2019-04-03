#include "server/server.h"

bool	check_sha1_sum(unsigned char *checksum, unsigned char *decrypted_full_packet)
{
	int i = 256;
	int j = 0;

	while (j < 16)
	{
		if (checksum[0 + j] != decrypted_full_packet[i + j])
			break;
		i++;
		j++;

	}
	if (j == 16)
		return (true);
	return (false);
}