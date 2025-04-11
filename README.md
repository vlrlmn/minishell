
# Minishell - replica of bash v3.2

`minishell` is an educational implementation of the Linux Shell (bash version 3.2), written in C.  
The project was developed as part of a team assignment at [42 Abu Dhabi school](https://42abudhabi.ae/).  
The goal is to gain a deeper understanding of how Unix terminals, processes, pipes, and system calls work.

---

## Features

- Execution of standard shell commands
- Implementation of **built-in commands**:
  - `cd`
  - `echo`
  - `env`
  - `exit`
  - `export`
  - `pwd`
  - `unset`
- Support for **redirections**:
  - `>` — overwrite output
  - `>>` — append output
  - `<` — input from file
  - `<<` — heredoc (input until limiter)
- Pipe support (`|`)
- Quote parsing:
  - Single quotes `'`
  - Double quotes `"`
- Environment variable handling
- ⚠️ Not supported:
  - Parentheses `()`
  - Logical operators `&&`, `||`
  - Job control (`fg`, `bg`, `jobs`)
  - Wildcards (`*`, `?`)

---

## Installation
### 1. Clone the repository
``` 
git clone https://github.com/your-username/minishell.git

cd minishell
``` 
### 2. Build the project
```
make
```
### 3. Run minishell
```
./minishell
```

Here you go! Use it just like you would use regular Bash.

## Usage Examples
```
$ echo Hello, world!
Hello, world!

$ export USER=42student
$ echo $USER
42student

$ cd ..
$ pwd
/home/user/

$ cat < input.txt | grep "error" > output.txt
```

## Technologies Used 🛠
Language: C

Standards: POSIX

**Key concepts**: processes, fork/exec, signals, pipes, redirections, system calls, command parsing.

## Team

👩‍💻 Valeriia Lomakina — https://github.com/vlrlmn

👨‍💻 Sofya Abdulkina — https://github.com/sabdulki

Developed as part of 42 Abu Dhabi curriculum.
