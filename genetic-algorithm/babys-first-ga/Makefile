CXX = g++
CXXFLAGS = -Wall -std=c++11

SRC = main.cpp genetic.cpp
OBJ = main.o genetic.o

TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp genetic.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

genetic.o: genetic.cpp genetic.h
	$(CXX) $(CXXFLAGS) -c genetic.cpp -o genetic.o

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean