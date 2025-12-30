##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile
##

NAME_SERVER    =   zappy_server

SRCS_SERVEUR = $(shell find src -type f -name "*.c")

OBJS_SERVER    =	$(SRCS_SERVEUR:.c=.o)

CFLAGS  =	-Wextra -Wpedantic -Iincludes -Igame_logic \
	-Igame_logic/commands -Igame_logic/game_state -Iinclude \
	-std=c2x

all:    $(NAME_SERVER)

$(NAME_SERVER):    $(OBJS_SERVER)
	$(CC) -o $(NAME_SERVER) $(OBJS_SERVER)

clean:
	$(RM) $(OBJS_SERVER)
	$(RM) *~
	$(RM) coding-style-reports.log

fclean: clean
	$(RM) $(NAME_SERVER)

re: fclean all
