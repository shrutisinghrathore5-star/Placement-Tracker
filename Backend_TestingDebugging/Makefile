CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC = src/main.cpp src/PlacementSystem.cpp src/FileManager.cpp src/Student.cpp src/Admin.cpp src/Company.cpp src/Application.cpp
TARGET = placement_tracker

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
	rm -f data/*.txt

.PHONY: all clean
