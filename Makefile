CLIENT			= 	client_app
SERVER			= 	server_app

CLIENT_CODE		= 	client.c \
					initialization_module.c \
					string_iterator_routines.c \
					aes_encryption_module.c \
					rsa_encryption.c \
					
SERVER_CODE		= 	server.c \
					rsa_decryption.c \
					aes_decryption.c \
					check_sha1.c \
					linked_list_functions.c \
			
SRCS_CLIENT     = 	$(addprefix ./srcs/client/, $(CLIENT_CODE))
SRCS_SERVER     = 	$(addprefix ./srcs/server/, $(SERVER_CODE))

CFLAGS			= 	-Wall -Wextra -Werror

OBJECTS_CLIENT 	= 	$(SRCS_CLIENT:.c=.o)
OBJECTS_SERVER 	= 	$(SRCS_SERVER:.c=.o)

MBEDTLS_INCLUDE	= 	/Users/otimofie/.brew/Cellar/mbedtls/2.13.0/include/
MBEDTLS_PATH	= 	/Users/otimofie/.brew/Cellar/mbedtls/2.13.0/lib

#colors
RESET			= 	\033[m
GREEN       	= 	\033[01;38;05;46m
YELLOW      	= 	\033[01;38;05;226m

all: $(CLIENT) $(SERVER)

$(CLIENT): $(OBJECTS_CLIENT)
	@ gcc    $(CFLAGS) -I$(MBEDTLS_INCLUDE) $(SRCS_CLIENT) -L$(MBEDTLS_PATH) -lmbedtls -lmbedx509 -lmbedcrypto -o $(CLIENT)
	@ echo  "$(YELLOW)$(CLIENT): $(GREEN)compiled.$(RESET)"

$(OBJECTS_CLIENT): %.o: %.c
	@ gcc -c $(CFLAGS) -I$(MBEDTLS_INCLUDE) $< -o $@

$(SERVER): $(OBJECTS_SERVER)
	@ gcc    $(CFLAGS)  -I$(MBEDTLS_INCLUDE) $(SRCS_SERVER) -L$(MBEDTLS_PATH)  -lmbedtls -lmbedx509 -lmbedcrypto -o $(SERVER)
	@ echo  "$(YELLOW)$(SERVER): $(GREEN)compiled.$(RESET)"

$(OBJECTS_SERVER): %.o: %.c
	@ gcc -c $(CFLAGS)  -I$(MBEDTLS_INCLUDE) $< -o $@

clean:
	@ rm -f $(OBJECTS_CLIENT)
	@ rm -f $(OBJECTS_SERVER)

fclean: clean
	@ rm -f UDP_INPUT.txt
	@ rm -f $(CLIENT)
	@ rm -f $(SERVER)

re: fclean all

.PHONY: all clean fclean re