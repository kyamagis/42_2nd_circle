NAME = computor
# B_NAME = miniRT_bonus
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g -fsanitize=address
# CXXFLAGS = -MMD -MP -g -fsanitize=address
INCLUDE = -I ./includes/
OBJDIR = ./obj
SRCSDIR = ./srcs

SRCS =	main.cpp \
		QuadraticEquation.cpp

B_SRCS =	

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
B_OBJDIR = ./b_obj
B_OBJS = $(addprefix $(B_OBJDIR)/, $(B_SRCS:.cpp=.o))

all : $(NAME)

$(NAME) : $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(INLCUDE) -o $(NAME)

bonus: fclean $(B_NAME)

$(OBJDIR) :
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCSDIR)/%.cpp | $(OBJDIR)
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(INLCUDE)

clean :
	$(RM) -rf $(OBJDIR)
	$(RM) -rf $(B_OBJDIR)
	$(RM) return.txt

fclean : clean
	$(RM) $(NAME) $(B_NAME)

re : fclean all

test: all
	bash ./test.sh

PHONY : clean fclean all debug re test