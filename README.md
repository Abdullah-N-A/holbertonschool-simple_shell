# Simple Shell

## Description
**Simple Shell** is a basic UNIX command interpreter written in C.  
It replicates the behavior of the standard shell (`sh`) and can execute commands read from the standard input or from a file.  

This project is part of the Holberton School curriculum and focuses on:
- Process creation and management
- File descriptors
- System calls (`fork`, `execve`, `wait`, etc.)
- Environment variables
- Memory management

---

## Compilation
To compile the shell, run:
```bash
gcc -Wall -Werror -Wextra -pedantic *.c -o hsh
Usage
You can use the shell in interactive or non-interactive mode.

Interactive mode:
bash
Copy code
$ ./hsh
($) /bin/ls
hsh main.c shell.c
($) exit
$
Non-interactive mode:
bash
Copy code
$ echo "/bin/ls" | ./hsh
hsh main.c shell.c test_ls_2
$
Built-in Commands
Command	Description
exit	Exit the shell
env	Display the current environment variables

Examples
bash
Copy code
($) ls
AUTHORS  README.md  main.c
($) echo "Hello, world!"
Hello, world!
($) env
USER=holberton
LANG=C
HOME=/root
($) exit
Files
File	Description
main.c	Entry point and command loop
shell.c	Core shell logic
execute.c	Handles execution of external commands
builtins.c	Handles built-in shell commands
path.c	PATH resolution for commands
utils.c	Helper functions (string ops, memory management)
man_1_simple_shell	Manual page for the shell
AUTHORS	List of contributors

Flowchart
pgsql
Copy code
+------------------+
|  Start program   |
+------------------+
          |
          v
+------------------+
| Display prompt   |
+------------------+
          |
          v
+------------------+
| Read user input  |
+------------------+
          |
          v
+----------------------------+
| Parse and tokenize command |
+----------------------------+
          |
          v
+---------------------------+
| Built-in or external cmd? |
+---------------------------+
     |                   |
     v                   v
 Built-in?          External command
     |                   |
     v                   v
Execute directly     Fork & execve()
          |
          v
+------------------+
| Display prompt   |
+------------------+
          |
          v
+------------------+
|       Exit       |
+------------------+
