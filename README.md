<!-- filepath: c:\Users\Nahom\Documents\Projects\einah\README.md -->
# 🌱 Einah Programming Language

_Einah is a modern, expressive programming language and interpreter written in C++. It features a clean, readable syntax inspired by natural language and functional programming._

---

## 🚀 Quick Start

1. **Run Without Building**
   - If you have a prebuilt binary (`einah.exe`), you can use it directly:
     ```sh
     ./einah.exe <source_file.exn>
     ```
     or start the REPL:
     ```sh
     ./einah.exe
     ```
   - No compilation required!

2. **Build the Interpreter (if you want to build from source)**
   - Prerequisites: C++ compiler (MinGW, GCC, or Clang), `make` utility
   - In the project root, run:
     ```sh
     make
     ```
   - The executable will be at `build/einah.exe`.

---

## 🏃‍♂️ Running Without Building
If you already have the prebuilt binary (`einah.exe`), you can run Einah scripts or start the REPL immediately:

- **Run a Script:**
  ```sh
  ./einah.exe <source_file.exn>
  ```
- **Start the REPL:**
  ```sh
  ./einah.exe
  ```

No build steps are needed unless you want to rebuild from source or update the interpreter.

---

## 🌟 Why Einah?
- **Readable, natural syntax** — code that feels like writing in English.
- **First-class functions & closures**
- **Mutable/immutable variables**
- **Arrays, objects, and native utilities**
- **REPL and script execution**
- **Helpful error messages**

---

## 📝 Features
- Simple, readable syntax ([full cheat sheet](./syntax.md))
- Mutable (`sprout`) and immutable (`root`) variables
- First-class functions and closures
- Arrays and objects with literal syntax
- Control flow: if/else, while, for, for-each, break, continue
- Native functions for common operations
- REPL and script execution modes
- Helpful error messages and debugging tools

---

## 📚 Syntax Example
```einah
sprout x -> 10~
root y -> 20~
conjure add <<a, b>> [
  zipback a + b~
]~
spit add | x, y~
```
See [`syntax.md`](./syntax.md) for a full language reference and cheat sheet.

---

## 🛠️ Building & Usage

### Building
- See **Quick Start** above.

### Running Scripts
- `./einah.exe <source_file.exn>`
- Errors are reported with line numbers and helpful messages.

### Interactive REPL
- `./einah.exe`
- Use `help`, `clear`, `exit`, `quit`, or `#ast` for AST debugging.

### Command-Line Options
- `--help` : Show usage and options (to be implemented)
- `--version` : Show interpreter version (to be implemented)

---

## 🧑‍💻 Language Overview
- **Variables:**
  - `sprout x -> 5~` (mutable)
  - `root y -> 10~` (immutable)
- **Functions:**
  - `conjure add <<a, b>> [ zipback a + b~ ]~`
  - Call: `add | 1, 2~`
- **Control Flow:**
  - If/else: `whisper x ~~ 5 then [ ... ] or [ ... ]~`
  - While: `twirl x < 10 spin [ ... ]~`
  - For: `cartwheel i => 0 from 10 by 1 spin [ ... ]~`
  - For-each: `drift through arr catching item [ ... ]~`
- **Data Structures:**
  - Arrays: `$1, 2, 3$`
  - Objects: `@key => value;@`
- **Literals:**
  - String: `#hello#`
  - Number: `42`
  - Boolean: `yup`, `nope`
  - Null: `zip`

---

## ⚡ Native Functions
- `length(arr_or_str)`
- `push(arr, value)` / `pop(arr)`
- `slice(arr, start, end)`
- `keys(obj)` / `has(obj, key)`
- `split(str, sep)`
- `random()`
- `floor(num)` / `ceil(num)`
- `type(val)`

---

## 🛡️ Error Handling
- Syntax and runtime errors are reported with clear messages.
- The REPL will not crash on errors; it will print the error and let you continue.

---

## 📁 Project Structure
- `src/` — Source code for the interpreter
  - `frontend/` — Lexer, parser, and AST utilities
  - `runtime/` — Environment, interpreter, and value handling
    - `eval/` — Expression and statement evaluation
- `include/` — Header files for all modules
- `build/` — Compiled binaries and object files
- `Makefile` — Build instructions
- `syntax.md` — Full language cheat sheet

---

## 🤝 Contributing
Pull requests and issues are welcome! Please:
- Ensure code is well-documented and tested
- Update the syntax and README as needed
- Discuss large changes in an issue first

---

## 📄 License
This project is licensed under the MIT License. See [LICENSE](./LICENSE) for details.

---