## 🪄 EINAH v3.1 — Full Functional Core Cheat Sheet

### 🚧 SYNTAX CORE

| Category             | Keyword / Symbol | Meaning / Usage                |
| -------------------- | ---------------- | ------------------------------ |
| **Statement End**    | `~`              | Marks the end of any statement |
| **Declare Variable** | `sprout`         | Declare a mutable variable     |
| **Declare Constant** | `root`           | Declare an immutable constant  |
| **Assign/Reassign**  | `->`             | Assign or reassign a value     |
| **Define Function**  | `conjure`        | Declare a function             |
| **Call Function**    | `\|`             | Call a function                |
| **Return**           | `zipback`        | Return a value from a function |
| **Block**            | `[ ... ]`        | Code block                     |
| **Comment**          | `hush`           | Single-line comment            |
| **Print**            | `spit`           | Output to the console          |

---

### 🔁 CONTROL FLOW

| Category          | Keyword / Symbol                                 | Meaning / Usage                |
| ----------------- | ------------------------------------------------ | ------------------------------ |
| **Conditional**   | `whisper <cond> then [ ... ] or [ ... ]~`        | If / else block                |
| **While Loop**    | `twirl <cond> spin [ ... ]~`                     | While loop                     |
| **Infinite Loop** | `twirl forever spin [ ... ]~`                    | Loop forever                   |
| **Break Loop**    | `shatter~`                                  | Break out of current loop      |
| **Continue**      | `skip~`                                     | Continue next loop iteration   |
| **For Loop**      | `cartwheel from !i! to !n! spin [ ... ]~`        | Standard indexed loop          |
| **For-Each**      | `drift through $array$ catching $item$ [ ... ]~` | Iterate over array or iterable |

---

### 🧠 EXPRESSIONS

| Category        | Symbol / Literal             | Meaning / Usage      |
| --------------- | ---------------------------- | -------------------- |
| **Boolean**     | `yup`, `nope`                | Boolean literals     |
| **Logical AND** | `&=`                         | Logical AND          |
| **Logical OR**  | `\|=`                        | Logical OR           |
| **Logical NOT** | `~!`                         | Logical NOT          |
| **Equality**    | `~~`                         | Equal to             |
| **Inequality**  | `!~`                         | Not equal to         |
| **Comparison**  | `<`, `>`, `<~`, `>~`         | Relational operators |
| **Arithmetic**  | `+`, `-`, `*`, `/`, `%`, `^` | Math operations      |
| **Null**        | `zip`                        | Null / None          |

---

### 🔡 LITERALS

| Type        | Syntax Example  | Meaning                          |
| ----------- | --------------- | -------------------------------- |
| **String**  | `#hello world#` | Text literal (wrapped in `#`)    |
| **Integer** | `42`            | Integer literal (wrapped in `!`) |
| **Boolean** | `yup`, `nope`   | True / False                     |
| **Null**    | `zip`           | Null / None                      |

---

### 📦 DATA STRUCTURES

| Type             | Syntax Example           | Meaning                         |
| ---------------- | ------------------------ | ------------------------------- |
| **Array**        | `$1, 2, 3$`              | Ordered collection              |
| **Index Access** | `arr.0`                  | Access element at index 0       |
| **Map**          | `@key => value;@` | Key-value pair collection (map)        |

---

### 🧙‍♂️ FUNCTION STUFF

| Category                  | Syntax                                     | Meaning                    |
| ------------------------- | ------------------------------------------ | -------------------------- |
| **Function Definition**   | `conjure name <<params>>  [ ... ]~`        | Define a function          |
| **Function Call**         | `name \| args~`                            | Call a function            |
| **Return Value**          | `zipback value~`                           | Return value from function |