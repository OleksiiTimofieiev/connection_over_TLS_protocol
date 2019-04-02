#include "client/client.h"

static size_t	str_len(unsigned char *str)
{
	size_t 	i = MAX_ITERATOR_SIZE;

	while (i)
	{
		if (str[i] == 0)
			break;
		i--;
	}
	return (i);
}

void			add_to_string(unsigned char *str)
{
	if (str[MAX_ITERATOR_SIZE - 1] <= '8')
		str[MAX_ITERATOR_SIZE - 1] += 1;
	else
	{
		size_t 	length_till_end = str_len(str) - 1;

		while (length_till_end)
		{
			if (str[length_till_end] <= '8')
			{
				if (str[length_till_end] == 0)
					str[length_till_end] = '1';
				else
					str[length_till_end] += 1;
				break ;
			}
			else
				str[length_till_end] = '0';
			length_till_end--;
		}
	}
}

void			counter_line_composer(unsigned char *dst, unsigned char *src)
{
	size_t		i = 0;

	while (i < MAX_ITERATOR_SIZE)
	{
		if (src[i] != 0)
			break ;
		i++;
	}
	
	memcpy(&dst[8], &src[i], MAX_ITERATOR_SIZE - i);
}
