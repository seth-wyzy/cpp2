# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17

# Target executable
TARGET = game

# Source files
SRCS = main.cpp Pin.cpp card.cpp

# Object files (same names with .o extension)
OBJS = $(SRCS:.cpp=.o)

# Default rule: build target from object files
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
