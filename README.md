# shx - Simple Unix Shell

A lightweight, educational Unix shell written in C that demonstrates fundamental operating system concepts.

![Version](https://img.shields.io/badge/version-0.1.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)

## Features

- ✅ **Command Execution** - Run any external program
- ✅ **Built-in Commands** - `cd`, `pwd`, `echo`, `exit`, `export`, `env`
- ✅ **I/O Redirection** - `>`, `<`, `>>`
- ✅ **Background Jobs** - Run commands with `&`
- ✅ **Signal Handling** - Ctrl+C doesn't kill the shell
- ✅ **Colored Prompt** - Shows current directory

## Quick Start
```bash
# Clone the repository
git clone https://github.com/YOUR_USERNAME/shx.git
cd shx

# Build
make release

# Run
./bin/shell
```

## Usage Examples
```bash
# Basic commands
shx $ pwd
/home/user/shx

shx $ echo Hello World
Hello World

# I/O Redirection
shx $ ls > files.txt
shx $ cat < input.txt
shx $ echo "log entry" >> log.txt

# Background processes
shx $ sleep 10 &
[1] 12345

# Change directory
shx $ cd /tmp
tmp $ pwd
/tmp

# Environment variables
shx $ export MY_VAR=hello
shx $ env | grep MY_VAR
MY_VAR=hello
```

## Building

### Debug Build
```bash
make debug      # Build with debug symbols
make run        # Build and run
```

### Release Build
```bash
make release    # Optimized build
```

### Other Commands
```bash
make clean      # Remove build artifacts
make test       # Run tests
make valgrind   # Check for memory leaks
```

## Project Structure
```
shx/
├── src/
│   ├── main.c          # Main REPL loop
│   ├── parser.c        # Command parsing
│   ├── executor.c      # Process execution
│   ├── builtins.c      # Built-in commands
│   ├── redirect.c      # I/O redirection
│   ├── signals.c       # Signal handling
│   ├── utils.c         # Utility functions
│   ├── env.c           # Environment handling
│   └── include/
│       ├── shell.h     # Main header
│       └── config.h    # Configuration
├── tests/              # Test scripts
├── Makefile           
└── README.md          
```

## Technical Details

**Process Execution:** Uses `fork()` + `execvp()` pattern

**I/O Redirection:** Implemented with `dup2()` and file descriptors

**Signal Handling:** Custom handlers for `SIGINT` and `SIGCHLD`

**Memory Management:** No memory leaks (verified with valgrind)

## Roadmap

- [ ] Pipeline support (`|`)
- [ ] Command history
- [ ] Tab completion
- [ ] Environment variable expansion (`$VAR`)
- [ ] Job control (`fg`, `bg`, `jobs`)
- [ ] Wildcard expansion (`*.txt`)

## Contributing

Contributions welcome! Feel free to:
- Report bugs
- Suggest features
- Submit pull requests

## License

MIT License - see LICENSE file

## Learning Resources

Great for learning:
- Process management in Unix
- File descriptors and I/O
- Signal handling
- System calls (`fork`, `exec`, `wait`, `dup2`)

## Author

Built as an educational project to understand Unix shell internals.
