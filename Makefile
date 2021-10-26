CC			=	g++
CFLAGS		=	-Wall -Wextra -Werror 

NAME    	= webserver

SRC_DIR		=	./srcs/
OBJ_DIR		=	./objs/

SRC			+=	main.cpp

OBJ			=	$(addprefix $(OBJ_DIR),$(SRC:.cpp=.o))


RM			=	/bin/rm -f
RM_DIR		=	/bin/rm -rf

$(OBJ_DIR)%.o:$(SRC_DIR)%.cpp ./headers/*.hpp
	$(CC) $(CFLAGS) -c $< -o $@

all:
	mkdir -p $(OBJ_DIR)
	$(MAKE) $(NAME) --no-print-directory

$(NAME): $(OBJ)
	$(CC) ${OBJ} -o ${NAME}


clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

eclean: clean fclean

re: fclean all

test: all
	clear
	./${NAME} 
norm:
	norminette ./srcs/ ./includes/