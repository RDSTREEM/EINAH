CXX := g++
CXXFLAGS := -std=c++17 -Wall -Iinclude -mconsole

SRC_DIR := src
OBJ_DIR := build/obj
BIN := build/einah

SRC := $(shell powershell -Command "Get-ChildItem -Recurse -Filter *.cpp -Path $(SRC_DIR) | ForEach-Object { \$_.FullName.Replace('$(CURDIR)\\','').Replace('\','/') }")

OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

$(BIN): $(OBJ)
	@powershell -Command "if (!(Test-Path '$(dir $@)')) { New-Item -ItemType Directory -Path '$(dir $@)' | Out-Null }"
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@powershell -Command "if (!(Test-Path '$(dir $@)')) { New-Item -ItemType Directory -Path '$(dir $@)' -Force | Out-Null }"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@powershell -Command "if (Test-Path '$(OBJ_DIR)') { Remove-Item -Recurse -Force '$(OBJ_DIR)' }"
	@powershell -Command "if (Test-Path '$(dir $(BIN))') { Remove-Item -Recurse -Force '$(dir $(BIN))' }"

run: $(BIN)
	$(BIN)

print:
	@echo CURDIR: $(CURDIR)
	@echo Source files: $(SRC)
	@echo Object files: $(OBJ)
	@echo Binary: $(BIN)

.PHONY: clean run print
