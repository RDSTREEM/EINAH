# Einah Interpreter

Einah is a custom programming language interpreter written in C++. This project includes a lexer, parser, AST, and runtime environment for executing Einah programs.

## Project Structure

- `src/` - Source code for the interpreter
  - `frontend/` - Lexer, parser, and AST utilities
  - `runtime/` - Environment, interpreter, and value handling
    - `eval/` - Expression and statement evaluation
- `include/` - Header files for all modules
- `build/` - Compiled binaries and object files
- `Makefile` - Build instructions

## Building the Project

### Prerequisites
- C++ compiler (e.g., MinGW, GCC, or Clang)
- `make` utility (e.g., MinGW Make)

### Build Steps
1. Open a terminal in the project root directory.
2. Run:
   ```sh
   mingw32-make
   ```
3. The executable will be generated at `build/einah.exe`.

## Running the Interpreter

After building, run the interpreter with:
```sh
./build/einah.exe <source_file.einah>
```

## Contributing
Pull requests and issues are welcome. Please ensure code is well-documented and tested.

