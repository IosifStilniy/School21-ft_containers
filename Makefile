HEAD		=	./

NAME		=	ft_containers

GCC			=	c++

FLAGS		=	-Wall -Werror -Wextra -std=c++98

RM			=	rm -f

$(NAME):	$(HEADS) $(OBJS)
			$(GCC) $(FLAGS) main.cpp -D STL -o stl
			$(GCC) $(FLAGS) main.cpp -D FT -o ft

all:	$(NAME)

clean:

fclean: clean
		rm -f stl
		rm -f ft

re:		fclean all

.PHONY:	all clean fclean lib bonus
