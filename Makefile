NAME = Webserv
CLIENT = client
CC = g++

CFLAGS =
# CFLAGS = -Wall -Wextra -Werror


SRCS_DIR	=	./srcs/
OBJ_DIR		=	./objs/
INC_DIR		=	./headers/

SRC			+= main.cpp

OBJ			=	${addprefix ${OBJ_DIR},${SRC:.cpp=.o}}

INC			=	${addprefix -I,${INC_DIR}}

RM			=	/bin/rm -f
RM_DIR		=	/bin/rm -rf

${OBJ_DIR}%.o:${SRCS_DIR}%.cpp 
	${CC}  ${CFLAGS} -c $< -o $@

all:
	@mkdir -p ${OBJ_DIR}
	${MAKE} ${NAME} --no-print-directory
	${CC} client.cpp -o ${CLIENT}

${NAME}: ${OBJ}
	@${CC}  ${OBJ}  -o ${NAME}

clean:
	@${RM_DIR} ${OBJ_DIR} ${CLIENT}

fclean: clean

	@rm -f ${NAME}

re: fclean all

test: all
	@clear
	@./${NAME}

