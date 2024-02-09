NAME  = webserv

SRCS  = srcs/Client.cpp \
		srcs/ConfParse.cpp \
		srcs/Location.cpp \
		srcs/main.cpp \
		srcs/Mommy.cpp \
		srcs/Server.cpp \
		srcs/ServerConf.cpp

OBJS  = $(SRCS:.cpp=.o)

FLAGS = -std=c++98 -g3 -fsanitize=address

.cpp.o:
				@c++ $(FLAGS) -c $< -o $(<:.cpp=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
				@c++ $(FLAGS) $(OBJS) -o $(NAME)
				@printf "\033[32m Done ✓ \033[0m\n"

clean:
				@rm -f $(OBJS)
				@printf "\033[32m Cleaned ✓ \033[0m\n"

fclean: 		clean
				@rm -f $(NAME)

re:				fclean all

.PHONY: all clean fclean re