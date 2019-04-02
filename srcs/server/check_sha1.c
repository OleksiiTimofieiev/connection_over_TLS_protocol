#include "../../includes/server/server.h"

bool	check_sha1_sum(unsigned char *checksum, unsigned char *decrypted_full_packet)
{
	int i = 0;

	while (i < 16)
	{
		if (checksum[i] != decrypted_full_packet[i])
			break;
		i++;
	}
	if (i == 16)
		return (true);
	return (false);
}