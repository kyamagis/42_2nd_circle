NAME = mod1

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wshadow -O3 -MMD -MP \
           -g -fsanitize=address -ftrapv -fsanitize=undefined \
		   -fopenmp
LIBS = -lglut -lGLU -lGL 
DEBUGFLAGS = -g -DDEBUG

INCLUDE = -I ./includes/
OBJDIR = ./obj
SRCSDIR = ./srcs

# SRCS = *.cpp
SRCS =	main.cpp \
		Quadrangle.cpp \
		Quaternion.cpp \
		Triangle.cpp \
		Circle.cpp \
		Vec.cpp \
		Particle.cpp \
		DelaunayTriangulation.cpp \
		RMD.cpp \
		TerrestrialSimulation.cpp \
		Graphic.cpp \
		Utils.cpp \
		MPS.cpp \
		BucketsController.cpp \

B_SRCS =	

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPS = $(OBJS:.o=.d)
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
	$(RM) wall_weight.png
	$(RM) WallWeight.csv


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