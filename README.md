# minishell

A small UNIX shell written in C, re-implementing the core behavior of `bash` — built as part of the 42 curriculum.

## Features

- Interactive prompt powered by **GNU Readline** (line editing, history)
- **Tokenizer & parser** with full single/double quote handling and syntax error detection
- **Environment variable expansion** (`$VAR`, `$?`) with quote-aware rules
- **Pipelines**: `cmd1 | cmd2 | ...`
- **Redirections**: `<`, `>`, `>>` and heredoc (`<<`)
- **Built-in commands**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- **Signal handling** (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`) mirroring bash behavior in interactive and child-process contexts
- Exit status propagation (`$?`) like a real shell

## Project structure

```
src/
├── parsing/       # tokenizer, quote handling, variable expansion, syntax errors
├── preparation/   # building the command table from parsed tokens
├── exec/          # fork/execve, pipes, redirections, heredoc
├── builtins/      # built-in command implementations
├── signal/        # signal handling
└── utils/
libft/             # custom C standard library (42's libft)
```

## Build & run

Requires `cc`, `make` and GNU Readline (`apt install libreadline-dev` / `brew install readline`).

```bash
make
./minishell
```

```bash
minishell$ echo "hello $USER" | tr a-z A-Z > out.txt
minishell$ cat < out.txt
HELLO MAXENCE
```

## What this project demonstrates

- Process lifecycle management: `fork`, `execve`, `wait`, exit codes
- File descriptor manipulation (`dup2`, `pipe`) for pipelines and redirections
- Writing a tokenizer and expander with bash-like quoting rules from scratch
- Careful manual memory management (no leaks) under the strict 42 Norm coding standard
