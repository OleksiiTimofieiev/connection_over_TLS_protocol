#include "server/server.h"

bool	check_sha1_sum(unsigned char *checksum, const unsigned char * const decrypted_full_packet)
{
	int j = 256;
	int i = 0;

	while (i < 16)
	{
		if (checksum[i] != decrypted_full_packet[j + i])
			break;
		i++;

	}
	if (i == 16)
		return (true);
	return (false);
}