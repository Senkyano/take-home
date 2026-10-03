#   NAME
NAME = prog

#---------------#
#   includes & libs
#---------------#
INCLUDES = include

#--------------------------------------#
#       Commande
#-----------------------#
RM = rm -fr
CC = gcc

FLAGS = -Wall -Werror -Wextra -g -I $(INCLUDES)

#--------------------------------------#
#       directory
#-----------------------#
SRCS = srcs
OBJS = objs

#--------------------------------------#
#       Colors
#-----------------------#
C_R   = \033[1;31m
C_G   = \033[1;32m
C_W   = \033[0;37m
RESET = \033[0m

#--------------------------------------#
#       File
#-----------------------#
FILE_C = main.c hash.c consumer.c data.c gen_json.c producer.c queue.c

OBJ = $(patsubst %.c, $(OBJS)/%.o, $(FILE_C))

#--------------------------------------#
#       Rules
#-----------------------#
all : $(NAME)

$(NAME) : $(LIB_A) $(OBJ)
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ)
	@echo "$(C_G)Compilation de $(NAME) réussie !$(RESET)"

$(OBJS)/%.o : $(SRCS)/%.c
	@mkdir -p $(OBJS)
	$(CC) $(FLAGS) -c $< -o $@

clean :
	@$(RM) $(OBJS)
	@echo "$(C_R)FILE '*.o' for $(NAME) deleted$(RESET)"

fclean : clean
	@$(RM) $(NAME)
	@echo "$(C_W)FILE '$(NAME)' deleted$(RESET)"

re : fclean all

.PHONY : re clean fclean all