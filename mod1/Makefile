NAME = mod1

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wshadow -MMD -MP -g -fsanitize=address
LIBS = -lglut -lGLU -lGL 
DEBUGFLAGS = -g -DDEBUG

INCLUDE = -I ./includes/
OBJDIR = ./obj
SRCSDIR = ./srcs

# SRCS = *.cpp
SRCS =	main.cpp \
		Vec.cpp \
		Side.cpp \
		Triangle.cpp \
		DelaunayTriangulation.cpp \
		ParseModFile.cpp \
		TerrestrialSimulation.cpp \
		Graphic.cpp \
		RenderingAlgorithm.cpp \

B_SRCS =	

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
B_OBJDIR = ./b_obj
B_OBJS = $(addprefix $(B_OBJDIR)/, $(B_SRCS:.cpp=.o))


export LIBGL_ALWAYS_INDIRECT=0

all : $(NAME)

$(NAME) : $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) $(INLCUDE) -o $(NAME)

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