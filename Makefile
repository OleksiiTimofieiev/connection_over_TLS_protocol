CLIENT			= client_app
SERVER			= server_app

CLIENT_CODE		= 	client.c \
					initialization_module.c \
					string_iterator_routines.c 
SERVER_CODE		= server.c 
			
SRCS_CLIENT     = $(addprefix ./srcs/client/, $(CLIENT_CODE))
SRCS_SERVER     = $(addprefix ./srcs/server/, $(SERVER_CODE))

CFLAGS			= -Wall -Wextra -Werror

OBJECTS_CLIENT 	= $(SRCS_CLIENT:.c=.o)
OBJECTS_SERVER 	= $(SRCS_SERVER:.c=.o)

INC_SERVER		= /Users/otimofie/.brew/Cellar/libev/4.24/include/
LIBEV_PATH		= /Users/otimofie/.brew/Cellar/libev/4.24/lib/

#colors
RESET			= \033[m
GREEN       	= \033[01;38;05;46m
YELLOW      	= \033[01;38;05;226m

all: $(CLIENT) $(SERVER)

$(CLIENT): $(OBJECTS_CLIENT)
# 	@ gcc    $(CFLAGS) -I $(INC) $(SRCS) -L ./libft -lft -ltermcap -o $(CLIENT)
# gcc aes_experiments.c -lmbedtls -lmbedcrypto
	@ gcc    $(CFLAGS) $(SRCS_CLIENT) -o $(CLIENT)
	@ echo  "$(YELLOW)$(CLIENT): $(GREEN)compiled.$(RESET)"

$(OBJECTS_CLIENT): %.o: %.c
	@ gcc -c $(CFLAGS) $< -o $@

$(SERVER): $(OBJECTS_SERVER)
# 	@ gcc    $(CFLAGS) -I $(INC) $(SRCS) -L ./libft -lft -ltermcap -o $(CLIENT)
	@ gcc    $(CFLAGS) -I$(INC_SERVER) $(SRCS_SERVER) -L$(LIBEV_PATH) -lev -o $(SERVER)
	@ echo  "$(YELLOW)$(SERVER): $(GREEN)compiled.$(RESET)"

$(OBJECTS_SERVER): %.o: %.c
	@ gcc -c $(CFLAGS) -I$(INC_SERVER) $< -o $@

clean:
	@ rm -f $(OBJECTS_CLIENT)
	@ rm -f $(OBJECTS_SERVER)

fclean: clean
	@ rm -f $(CLIENT)
	@ rm -f $(SERVER)

re: fclean all

.PHONY: all clean fclean re