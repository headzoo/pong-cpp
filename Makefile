CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -O2 
LDFLAGS  := -lSDL2 -lSDL2_ttf -lSDL2_mixer
TARGET   := pong
SRC      := $(wildcard *.cpp)
OBJ      := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
