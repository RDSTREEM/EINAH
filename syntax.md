
🧙‍♂️ EINAH v2.1 — Final Syntax Rules
> A language so stylish, even Rick would approve.

🔧 CORE PRINCIPLES

Feature            | Syntax
------------------ | -------------------------------------
Statement End      | ~ (tilde) only
Variable Decl      | let x -> value~ / const x -> value~
Reassignment       | x -> newValue~
Function Decl      | craft fname(args) [ ... ]~
Function Call      | fname | arg1, arg2~
Indexing           | arr.0, obj.key
Block Delimiters   | [ ... ]
Conditional        | when <cond> then [ ... ] else [ ... ]~
Loop               | loop <cond> do [ ... ]~
Print              | yeet something~
Return             | boomerang value~
Comments           | shhh ...
Equality           | x ~~ y
Inequality         | x !~ y
Easter Eggs        | rickroll~, kaboom~

🧱 SYNTAX IN DEPTH

1. 📌 Statements and Assignment
let x -> 10~
const pi -> 3.14~
x -> x + 1~

2. 📦 Objects and Arrays
let arr -> [1, 2, 3]~
yeet arr.0~  # prints 1

let obj -> {
  name: "Einah"~,
  power: 9000~
}~
yeet obj.name~
obj.power -> 9999~

3. 📞 Functions
craft greet(name) [
  yeet "Hello, " + name~
]~
greet | "Einah"~

craft add(a, b) [
  boomerang a + b~
]~
yeet add | 10, 5~

4. 🔁 Loops
let x -> 1~
loop <x < 6> do [
  yeet x~
  x -> x + 1~
]~

5. 🌳 Conditionals
when <x ~~ 3> then [
  yeet "Three!"~
] else [
  yeet "Not three!"~
]~

6. 📣 Print and Return
yeet "This is printed"~
boomerang 42~

7. 🧼 Comments
shhh This is a comment

8. 🐣 Easter Eggs
rickroll~
kaboom~

💡 Sample EINAH v2.1 Program
shhh Print numbers and a message~
let x -> 1~
loop <x < 6> do [
  when <x ~~ 3> then [
    yeet "Midway there! 🔥"~
  ] else [
    yeet x~
  ]~
  x -> x + 1~
]~

let person -> {
  name: "Rick"~,
  catchphrase: "Wubba Lubba Dub Dub"~
}~
yeet person.name~
yeet person.catchphrase~

craft add(a, b) [
  boomerang a + b~
]~
yeet add | 10, 20~
rickroll~

✅ Summary

Type         | Syntax Example
------------ | ----------------------------
Decl         | let x -> 5~
Reassign     | x -> x + 1~
Function     | craft foo(a) [ ... ]~
Call         | foo | arg1, arg2~
Conditional  | when <cond> then [ ... ] else [ ... ]~
Loop         | loop <cond> do [ ... ]~
Print        | yeet value~
Return       | boomerang value~
Comments     | shhh comment
EOF Fun      | rickroll~, kaboom~
