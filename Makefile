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
					print_to_file_routine.c \
					parsing_with_get_opt_server.c \
					double_linked_list_routines.c \
			
SRCS_CLIENT     = 	$(addprefix ./srcs/client/, $(CLIENT_CODE))
SRCS_SERVER     = 	$(addprefix ./srcs/server/, $(SERVER_CODE))

MBEDTLS_INC		= 	./mbedtls/include/
MBEDTLS_LIB		= 	./mbedtls/lib/

INCLUDES		=	./includes/

CFLAGS			= 	-Wall -Wextra -Werror

OBJECTS_CLIENT 	= 	$(SRCS_CLIENT:.c=.o)
OBJECTS_SERVER 	= 	$(SRCS_SERVER:.c=.o)

#colors
RESET			= 	\033[m
GREEN       	= 	\033[01;38;05;46m
YELLOW      	= 	\033[01;38;05;226m

all: $(CLIENT) $(SERVER)

$(CLIENT): $(OBJECTS_CLIENT)
	@ gcc    $(CFLAGS) -I$(INCLUDES) -I$(MBEDTLS_INC) $(SRCS_CLIENT)  -L$(MBEDTLS_LIB) -lmbedtls -lmbedx509 -lmbedcrypto -o $(CLIENT)
	@ echo  "$(YELLOW)$(CLIENT): $(GREEN)compiled.$(RESET)"

$(OBJECTS_CLIENT): %.o: %.c
	@ gcc -c $(CFLAGS) -I$(INCLUDES) -I$(MBEDTLS_INC)  $< -o $@

$(SERVER): $(OBJECTS_SERVER)
	@ gcc    $(CFLAGS) -I$(INCLUDES) -I$(MBEDTLS_INC) $(SRCS_SERVER) -L$(MBEDTLS_LIB) -lmbedtls -lmbedx509 -lmbedcrypto -lpthread -o $(SERVER)
	@ echo  "$(YELLOW)$(SERVER): $(GREEN)compiled.$(RESET)"

$(OBJECTS_SERVER): %.o: %.c
	@ gcc -c $(CFLAGS) -I$(INCLUDES) -I$(MBEDTLS_INC) $< -o $@

clean:
	@ rm -f $(OBJECTS_CLIENT)
	@ rm -f $(OBJECTS_SERVER)

fclean: clean
	@ rm -f UDP_INPUT
	@ rm -f $(CLIENT)
	@ rm -f $(SERVER)

re: fclean all

.PHONY: all clean fclean re