# Define the compiler
CXX = g++

# Define the flags
CXXFLAGS = -Wall -std=c++17

# Define the libraries to link
LIBS = -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi 

# Define the source files
SRCS = main.cpp ./common/GLShader.cpp

# Define the output executable
TARGET = main

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

# Clean up build files
clean:
	rm -f $(TARGET)