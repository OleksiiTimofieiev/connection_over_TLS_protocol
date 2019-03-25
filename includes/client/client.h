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

#define ID_SIZE 				8
#define MAX_ITERATOR_SIZE	 	(256 - 4)

bool	validation_of_program_arguments(int argc, char **argv);
void	client_configuration(char **argv, unsigned char *id, int *delay, short *port, unsigned char *iterator);

#endif