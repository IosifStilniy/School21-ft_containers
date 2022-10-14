HEAD		=	./

NAME		=	ft_containers

GCC			=	c++

FLAGS		=	-Wall -Werror -Wextra -std=c++98

RM			=	rm -f

$(NAME):	$(HEADS) $(OBJS)
			$(GCC) $(FLAGS) main.cpp -D STD -o std
			$(GCC) $(FLAGS) main.cpp -D FT -o ft

all:	$(NAME)

clean:

fclean: clean
		rm -f std
		rm -f ft

re:		fclean all

.PHONY:	all clean fclean lib bonus
