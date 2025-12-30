##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile
##

NAME	= zappy_2d

SRC	=	renderingmain.cpp	\
		Player.cpp	\
		Food.cpp	\
		Rendering.cpp	\

OBJ	= $(SRC:.cpp=.o)

CXX	= g++

CXXFLAGS	= -Wall -Wextra -Werror

SFMLFLAGS	= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all:	$(NAME)

$(NAME):	$(OBJ)
		$(CXX) $(CXXFLAGS) -o $@ $^ $(SFMLFLAGS)

clean:
		rm -f $(OBJ)

fclean:	clean
		rm -f $(NAME)

re:	fclean all
