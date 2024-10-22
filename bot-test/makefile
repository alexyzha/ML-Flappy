#=======================================================#
# compiler, flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
#=======================================================#
# objects
OBJS = main.o game.o bird.o
#=======================================================#
# SFML
SFML_INC = /opt/homebrew/Cellar/sfml/2.6.1/include
SFML_LIB = /opt/homebrew/Cellar/sfml/2.6.1/lib
#=======================================================#
# include SFML
INCLUDES = -I$(SFML_INC)
LIBS = -L$(SFML_LIB) -lsfml-graphics -lsfml-window -lsfml-system
#=======================================================#
# make target
all: test
#=======================================================#
# linking
test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o test $(OBJS) $(LIBS)
#=======================================================#
# main.cpp
main.o: main.cpp util.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c main.cpp
#=======================================================#
# game.cpp
game.o: game.cpp game.h bird.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c game.cpp
#=======================================================#
# bird.cpp
bird.o: bird.cpp bird.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c bird.cpp
#=======================================================#
# clean
clean:
	rm -f *.o test
