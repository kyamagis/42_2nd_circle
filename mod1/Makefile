NAME = mod1

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g -fsanitize=address
DEBUGFLAGS = -g -DDEBUG

INCLUDE = -I ./includes/
OBJDIR = ./obj
SRCSDIR = ./srcs

SRCS =	main.cpp \
		Coordinates.cpp \
		ParseModFile.cpp 

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
	$(RM) return_debug.txt

fclean : clean
	$(RM) $(NAME) $(B_NAME)

re : fclean all

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(INLCUDE) -o $(NAME)

dt: debug
	bash ./test.sh debug

test: all
	bash ./test.sh

PHONY : clean fclean all debug re debug dt test 