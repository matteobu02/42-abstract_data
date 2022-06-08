NAME		=	test

CXX			=	c++

CXXFLAGS	=	-Wall -Wextra -Werror -g

SRCS		=	main.cpp

OBJS		=	$(SRCS:.cpp=.o)


all:		$(NAME)

$(NAME):	$(OBJS)
