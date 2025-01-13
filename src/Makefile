# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I/usr/include/GLFW -I/usr/include/GL -I/usr/include/glm -Iexternal/cglm/include

LIBS = -lGLEW -lGL -lGLU -lglfw

SRC_DIR = core
BUILD_DIR = build/$(SRC_DIR)

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))

TARGET = engine

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
