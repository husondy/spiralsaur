# Variables
CXX = g++
CXXFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
TARGET = game_program
SRC = main.cpp

# Compile the program
build: $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS)

# Run the program
run:
	./$(TARGET)

# Compile and run
all: build run

# Clean the executable
clean:
	rm -f $(TARGET)
