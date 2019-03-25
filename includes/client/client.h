#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <ctype.h>
#include <signal.h>

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_fprintf         fprintf
#define mbedtls_printf          printf
#define mbedtls_exit            exit
#define MBEDTLS_EXIT_SUCCESS    EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE    EXIT_FAILURE
#endif /* MBEDTLS_PLATFORM_C */

/* generate aes key */
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"


#define ID_SIZE 				8
#define MAX_ITERATOR_SIZE	 	248 /* 256 - 8 */
#define INITIAL_PACKET_SIZE		256

bool	validation_of_program_arguments(int argc, char **argv);
void	client_configuration(char **argv, unsigned char *initial_packet, int *delay, short *port, unsigned char *iterator);
void	add_to_string(unsigned char *str);
void	counter_line_composer(unsigned char *dst, unsigned char *src);

#endif