################################################################################
#                          Makefile configuration                              #
################################################################################

NAME		= ircserv
CC			= c++
CFLAG		= -std=c++98 -Wall -Wextra -Werror -g3 ${INC}

################################################################################
#                                 Include                                      #
################################################################################

INC		=	-I./include

HEADERS		=	include/Macros.hpp \
				include/Server.hpp \
				include/Client.hpp

################################################################################
#                                 Sources                                      #
################################################################################

SRC			=	main.cpp \
				srcs/parsing.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Macros.cpp \
				srcs/commands/command_kick.cpp

################################################################################
#                                   Objects                                    #
################################################################################

OBJDIR 		= objs

OBJ    		= ${SRC:%.cpp=${OBJDIR}/%.o}

################################################################################
#                                   Colors                                     #
################################################################################

BLACK 	= \033[0;30m
RED 	= \033[0;31m
GREEN 	= \033[0;32m
YELLOW 	= \033[0;33m
BLUE 	= \033[0;34m
PURPLE 	= \033[0;35m
CYAN 	= \033[0;36m
RESET 	= \033[0m

################################################################################
#                                    Rules                                     #
################################################################################

all: ${NAME}

${NAME} : ${OBJ} 
	@${CC} ${CFLAG} ${OBJ} -o ${NAME}
	@echo "${YELLOW}✰✰✰ Alright, you can use ${NAME} ✰✰✰ :D${RESET}"

${OBJDIR}/%.o: %.cpp ${HEADERS}
	@mkdir -p ${dir $@}
	${CC} ${CFLAG} -c $< -o $@

clean:
	@rm -rf ${OBJDIR}

fclean: clean
	@echo "${BLUE}➟➟➟ Fclean in progress, please wait...${RESET}"
	@rm -f ${NAME}
	@echo "${GREEN}✓✓✓ Fclean done: ${NAME} has been removed${RESET}"

re: fclean all
	@echo "${CYAN}↻↻↻ Makefile has been re'd ↺↺↺ \o/${RESET}"

.PHONY: all clean fclean re bonus
