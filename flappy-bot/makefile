#=======================================================#
# compiler, flags
CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
#=======================================================#
# objects
OBJS = main.o game.o bird.o evo.o
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
all: gen
#=======================================================#
# linking
gen: $(OBJS)
	$(CXX) $(CXXFLAGS) -o gen $(OBJS) $(LIBS)
#=======================================================#
# main.cpp
main.o: main.cpp evo.h
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
# evo.cpp
evo.o: evo.cpp evo.h game.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c evo.cpp
#=======================================================#
# clean
clean:
	rm -f *.o gen
