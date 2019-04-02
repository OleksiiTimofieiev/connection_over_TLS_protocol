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
					thread_routines.c \

CLIENT_INC		=	./includes/client/
SERVER_INC		=	./includes/server/
			
SRCS_CLIENT     = 	$(addprefix ./srcs/client/, $(CLIENT_CODE))
SRCS_SERVER     = 	$(addprefix ./srcs/server/, $(SERVER_CODE))

CFLAGS			= 	-Wall -Wextra -Werror

OBJECTS_CLIENT 	= 	$(SRCS_CLIENT:.c=.o)
OBJECTS_SERVER 	= 	$(SRCS_SERVER:.c=.o)

#colors
RESET			= 	\033[m
GREEN       	= 	\033[01;38;05;46m
YELLOW      	= 	\033[01;38;05;226m

MBEDTLS_INC		= 	/Users/otimofie/.brew/Cellar/mbedtls/2.13.0/include
MBEDTLS_LIB		= 	/Users/otimofie/.brew/Cellar/mbedtls/2.13.0/lib

all: $(CLIENT) $(SERVER)

$(CLIENT): $(OBJECTS_CLIENT)
	@ gcc    $(CFLAGS)  -I$(CLIENT_INC) -I$(MBEDTLS_INC) $(SRCS_CLIENT) -L$(MBEDTLS_LIB) -lmbedtls -lmbedx509 -lmbedcrypto -o $(CLIENT)
	@ echo  "$(YELLOW)$(CLIENT): $(GREEN)compiled.$(RESET)"

$(OBJECTS_CLIENT): %.o: %.c
	@ gcc -c -I$(CLIENT_INC) -I$(MBEDTLS_INC) $(CFLAGS) $< -o $@

$(SERVER): $(OBJECTS_SERVER)
	@ gcc    $(CFLAGS) -I$(SERVER_INC) -I$(MBEDTLS_INC) $(SRCS_SERVER) -L$(MBEDTLS_LIB) -lmbedtls -lmbedx509 -lmbedcrypto -lpthread -o $(SERVER)
	@ echo  "$(YELLOW)$(SERVER): $(GREEN)compiled.$(RESET)"

$(OBJECTS_SERVER): %.o: %.c
	@ gcc -c -I$(SERVER_INC) -I$(MBEDTLS_INC) $(CFLAGS) $< -o $@

clean:
	@ rm -f $(OBJECTS_CLIENT)
	@ rm -f $(OBJECTS_SERVER)

fclean: clean
	@ rm -f UDP_INPUT
	@ rm -f $(CLIENT)
	@ rm -f $(SERVER)

re: fclean all

.PHONY: all clean fclean re