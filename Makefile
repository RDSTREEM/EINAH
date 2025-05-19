CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -mconsole

SRC_DIR := src
OBJ_DIR := build/obj
BIN := build/einah

SRC := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) $(wildcard $(SRC_DIR)/*/*/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

$(BIN): $(OBJ)
	@powershell -Command "if (!(Test-Path '$(dir $@)')) { New-Item -ItemType Directory -Path '$(dir $@)' }"
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@powershell -Command "if (!(Test-Path '$(dir $@)')) { New-Item -ItemType Directory -Path '$(dir $@)' -Force }"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@powershell -Command "if (Test-Path '$(OBJ_DIR)') { Remove-Item -Recurse -Force '$(OBJ_DIR)' }"
	@powershell -Command "if (Test-Path '$(dir $(BIN))') { Remove-Item -Recurse -Force '$(dir $(BIN))' }"

run: $(BIN)
	$(BIN)

print:
	@echo Source files: $(SRC)
	@echo Object files: $(OBJ)
	@echo Binary: $(BIN)

.PHONY: clean run print
