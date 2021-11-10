NAME = Webserv
CLIENT = client
CC = clang++

CFLAGS =
# CFLAGS = -Wall -Wextra -Werror


SRCS_DIR	=	./srcs/
OBJ_DIR		=	./objs/
INC_DIR		=	./headers/

SRC			+= server.cpp
SRC			+= utils.cpp

OBJ			=	${addprefix ${OBJ_DIR},${SRC:.cpp=.o}}

INC			=	${addprefix -I,${INC_DIR}}

RM			=	/bin/rm -f
RM_DIR		=	/bin/rm -rf

# colors
GREEN = \033[0;32m
YELLOW  = \033[0;33m
RED = \033[0;31m
RESET = \033[0m

${OBJ_DIR}%.o:${SRCS_DIR}%.cpp 
	@printf "${YELLOW}Generating Webserv objects... %-33.33s\r" $@
	@${CC}  ${CFLAGS} -c $< -o $@ ${INC}

all:
	@mkdir -p ${OBJ_DIR}
	@echo "${GREEN}Compiling WebServ..."
	@${MAKE} ${NAME} --no-print-directory
	@echo "\n${RESET}Done !"

${NAME}: ${OBJ}
	@${CC}  ${OBJ}  -o ${NAME}

clean:
	@echo "${RED}Removing objects...${RESET}"
	@${RM_DIR} ${OBJ_DIR} ${CLIENT}

fclean: clean
	@echo "${RED}Deleting executable..."
	@rm -f $(NAME)
	@echo "${RESET}"

re: fclean all

test: all
	@clear
	@./${NAME}

