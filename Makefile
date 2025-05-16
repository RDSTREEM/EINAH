CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -mconsole

SRC_DIR := src
OBJ_DIR := build/obj
BIN := build/einah

SRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

$(BIN): $(OBJ)
	@mkdir $(dir $@) 2>NUL || exit 0
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir $(dir $@) 2>NUL || exit 0
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rmdir /s /q $(OBJ_DIR) 2>NUL || exit 0
	@rmdir /s /q $(dir $(BIN)) 2>NUL || exit 0

run: $(BIN)
	$(BIN)

print:
	@echo Source files: $(SRC)
	@echo Object files: $(OBJ)
	@echo Binary: $(BIN)

.PHONY: clean run print
