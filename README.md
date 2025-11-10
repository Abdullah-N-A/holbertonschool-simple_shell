# simple_shell 0.1
- Single-word commands only, no PATH lookup.
- Uses fork + execve and passes environ.
- Handles errors and EOF (Ctrl+D).
- Interactive and non-interactive modes.

Build:
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

Usage:
./hsh
#cisfun$ /bin/ls
