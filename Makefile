NAME  = webserv

SRCS  = srcs/Client.cpp \
		srcs/ConfParse.cpp \
		srcs/Location.cpp \
		srcs/main.cpp \
		srcs/Mommy.cpp \
		srcs/Server.cpp \
		srcs/ServerConf.cpp \
		srcs/MimeUtils.cpp \
		srcs/Response.cpp \
		srcs/Request.cpp

OBJS  = $(SRCS:.cpp=.o)

FLAGS = -Wall -Wextra -Werror -std=c++98

.cpp.o:
				@c++ $(FLAGS) -c $< -o $(<:.cpp=.o)

all:			$(NAME)

$(NAME):		$(OBJS)
				@c++ $(FLAGS) $(OBJS) -o $(NAME)
				@printf "\033[32m Done ✓ \033[0m\n"

debug: 			FLAGS += -g3 -fsanitize=address -D DEBUG=1
debug:			$(OBJS)
				@c++ $(FLAGS) $(OBJS) -o $(NAME)
				@printf "\033[32m Done ✓ \033[0m\n"

clean:
				@rm -f $(OBJS)
				@printf "\033[32m Cleaned ✓ \033[0m\n"

fclean: 		clean
				@rm -f $(NAME)

re:				fclean all

red:			fclean debug

.PHONY: all debug clean fclean re red
