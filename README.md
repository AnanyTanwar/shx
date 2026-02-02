# shx - Simple Unix Shell

A lightweight, educational Unix shell written in C that demonstrates fundamental operating system concepts.

<p align="center">
  <img src="https://img.shields.io/badge/VERSION-0.2.0-blue?style=flat-square" />
  <img src="https://img.shields.io/badge/LICENSE-MIT-green?style=flat-square" />
</p>

## Features

- ✅ **Command Execution** - Run any external program
- ✅ **Built-in Commands** - `cd`, `pwd`, `echo`, `exit`, `export`, `env`
- ✅ **Pipelines** - Connect commands with `|` 🔥 **NEW in v0.2.0**
- ✅ **Environment Variables** - `$VAR` and `${VAR}` expansion 🔥 **NEW in v0.2.0**
- ✅ **I/O Redirection** - `>`, `<`, `>>`
- ✅ **Background Jobs** - Run commands with `&`
- ✅ **Signal Handling** - Ctrl+C doesn't kill the shell
- ✅ **Colored Prompt** - Shows current directory

## Quick Start
```bash
# Clone the repository
git clone https://github.com/AnanyTanwar/shx.git
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

# Pipelines (NEW!)
shx $ ls | grep src
shx $ cat file.txt | grep error | wc -l
shx $ echo hello | cat | cat

# Environment variables (NEW!)
shx $ echo $HOME
/home/user

shx $ cd $HOME
shx $ export MY_VAR=test
shx $ echo $MY_VAR
test

# I/O Redirection
shx $ ls > files.txt
shx $ cat < input.txt
shx $ echo "log entry" >> log.txt

# Combining features
shx $ ls $HOME | grep Documents
shx $ cat $USER.txt > output.txt

# Background processes
shx $ sleep 10 &
[1] 12345
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
│   ├── pipeline.c      # Pipeline execution (NEW!)
│   ├── expand.c        # Variable expansion (NEW!)
│   ├── redirect.c      # I/O redirection
│   ├── signals.c       # Signal handling
│   ├── utils.c         # Utility functions
│   ├── env.c           # Environment handling
│   └── include/
│       ├── shell.h     # Main header
│       └── config.h    # Configuration
├── tests/              # Test scripts
├── Makefile           # Build system
└── README.md          # This file
```

## Technical Details

**Process Execution:** Uses `fork()` + `execvp()` pattern

**Pipelines:** Implemented with `pipe()` system call and proper file descriptor management

**Variable Expansion:** Supports `$VAR`, `${VAR}`, and special variables (`$$` for PID)

**I/O Redirection:** Implemented with `dup2()` and file descriptors

**Signal Handling:** Custom handlers for `SIGINT` and `SIGCHLD`

**Memory Management:** No memory leaks (verified with valgrind)


## What's New

### v0.2.0 (Latest)
- ✨ Added pipeline support - connect unlimited commands with `|`
- ✨ Added environment variable expansion - `$VAR` and `${VAR}` syntax
- 🐛 Fixed memory leaks in command parsing
- 📝 Improved error handling

### v0.1.0
- Initial release with basic shell functionality
- Built-in commands and I/O redirection
- Background job support

## Contributing

Contributions welcome! Feel free to:
- Report bugs
- Suggest features  
- Submit pull requests

## License

MIT License - see LICENSE file

## Author

Built as an educational project to understand Unix shell internals.

---

⭐ **Star this repo if you find it useful!**
