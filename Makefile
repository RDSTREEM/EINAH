CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC_DIR = src
OBJ_DIR = build/obj
EXEC_DIR = build
EXEC = $(EXEC_DIR)/interpreter

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

$(EXEC): $(OBJECTS)
	@if not exist $(EXEC_DIR) mkdir $(EXEC_DIR)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(EXEC_DIR) rmdir /s /q $(EXEC_DIR)

run: $(EXEC)
	$(EXEC)

.PHONY: clean run
