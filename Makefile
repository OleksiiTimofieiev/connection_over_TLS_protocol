CLIENT			= client_app
SERVER			= server_app

CLIENT_CODE		= client.c 
SERVER_CODE		= server.c 
			
SRCS_CLIENT     = $(addprefix ./srcs/client/, $(CLIENT_CODE))
SRCS_SERVER     = $(addprefix ./srcs/server/, $(SERVER_CODE))

CFLAGS			= -Wall -Wextra -Werror
OBJECTS_CLIENT 	= $(SRCS_CLIENT:.c=.o)
OBJECTS_SERVER 	= $(SRCS_SERVER:.c=.o)

# LIB				= #./libft.a
# INC				= #./includes/ft_select.h

#colors
RESET			= \033[m
RED         	= \033[1;31m
GREEN       	= \033[01;38;05;46m
YELLOW      	= \033[01;38;05;226m
BLUE        	= \033[03;38;05;21m
VIOLET      	= \033[01;38;05;201m
CYAN        	= \033[1;36m
WHITE       	= \033[01;38;05;15m

all: $(CLIENT) $(SERVER)

$(CLIENT): $(OBJECTS_CLIENT)
# 	@ gcc    $(CFLAGS) -I $(INC) $(SRCS) -L ./libft -lft -ltermcap -o $(CLIENT)
	@ gcc    $(CFLAGS) $(SRCS_CLIENT) -o $(CLIENT)
	@ echo  "$(YELLOW)$(CLIENT): $(GREEN)compiled.$(RESET)"

$(OBJECTS_CLIENT): %.o: %.c
	@ gcc -c $(CFLAGS) $< -o $@

$(SERVER): $(OBJECTS_SERVER)
# 	@ gcc    $(CFLAGS) -I $(INC) $(SRCS) -L ./libft -lft -ltermcap -o $(CLIENT)
	@ gcc    $(CFLAGS) $(SRCS_SERVER) -lev -o $(SERVER)
	@ echo  "$(YELLOW)$(SERVER): $(GREEN)compiled.$(RESET)"

$(OBJECTS_SERVER): %.o: %.c
	@ gcc -c $(CFLAGS) $< -o $@

clean:
	@ rm -f $(OBJECTS_CLIENT)
	@ rm -f $(OBJECTS_SERVER)

fclean: clean
	@ rm -f $(CLIENT)
	@ rm -f $(SERVER)

re: fclean all

.PHONY: all clean fclean re