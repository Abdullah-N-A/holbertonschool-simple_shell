# Simple Shell

## Description
A simple UNIX command interpreter written in C that replicates basic functionality of `/bin/sh`.

This project is part of the Holberton School curriculum.

## Features
- Execute commands with absolute and relative paths
- PATH environment variable support
- Built-in commands: `exit`, `env`
- Interactive and non-interactive modes
- Proper error handling

## Compilation
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage

### Interactive Mode
```bash
$ ./hsh
($) ls
README.md AUTHORS main.c
($) /bin/pwd
/home/user/simple_shell
($) exit
$
```

### Non-Interactive Mode
```bash
$ echo "ls" | ./hsh
README.md AUTHORS main.c
$
```

## Built-in Commands

| Command | Description |
|---------|-------------|
| `exit [status]` | Exit the shell with optional status code |
| `env` | Display environment variables |

## Files

| File | Description |
|------|-------------|
| `main.c` | Entry point and main loop |
| `execute.c` | Command execution logic |
| `builtin.c` | Built-in command implementations |
| `split_line.c` | Input tokenization |
| `utils.c` | Helper functions (PATH, string conversion) |
| `shell.h` | Header file with prototypes |
| `man_1_simple_shell` | Manual page |

## Authors
- Abdullah Al-Asiri
- Fahad Al-Shammari
