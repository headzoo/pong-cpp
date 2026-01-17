CXX       := g++
CXXFLAGS  := -std=c++20 -Wall -Wextra -Wpedantic -O2 
LDFLAGS   :=
TARGET    := pong
BUILD_DIR := build
CPPFLAGS  := -Iinclude
LDLIBS    := -lSDL2 -lSDL2_ttf -lSDL2_mixer
SRC       := $(shell find src -name '*.cpp')
OBJ       := $(patsubst src/%.cpp,$(BUILD_DIR)/%.o,$(SRC))
INCLUDES  := $(shell find include -name '*.hpp')

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run:
	$(BUILD_DIR)/$(TARGET)

.PHONY: all clean run
