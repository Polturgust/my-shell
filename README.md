# my-shell (42sh)

A command-line shell written from scratch in C.

## What is this?

A shell is the program that runs inside a terminal window. It reads the
commands you type, runs them, and shows you the result. When you open a
terminal on Linux or macOS, you are already talking to a shell (usually one
called `bash` or `zsh`).

This project is a homemade version of that program. It behaves like a real
shell: you type a command, press Enter, and it does the work. It was built as
a learning project to understand how shells work under the hood, without
relying on any existing shell to do the job.

## What it can do

- **Run programs** — type a command like `ls` or `echo hello` and it runs it.
- **Built-in commands** — commands handled directly by the shell:
  `cd` (change folder), `exit` (quit), `env`, `setenv`, `unsetenv`
  (manage environment settings), `alias` (create shortcuts for commands),
  and `history` (see and replay past commands).
- **Pipes** — chain commands so the output of one feeds into the next,
  for example `ls | sort`.
- **Redirections** — send a command's output into a file, or read input
  from a file, using `>`, `>>`, `<`, and `<<`.
- **Logical operators** — run commands conditionally with `&&` and `||`,
  and group them with parentheses `( )`.
- **Wildcards (globbing)** — use `*` and `?` to match many files at once,
  for example `rm *.txt`.
- **Variables** — create and use your own variables with `set` and `unset`.
- **Command substitution** — insert the output of one command into another
  using backticks, for example `` echo `date` ``.
- **Command history** — recall and re-run earlier commands.
- **Aliases** — give short names to longer commands.
- **A configuration file** — a `.myshrc` file loaded at startup lets you
  set your own aliases automatically (similar to `.bashrc`).
- **A colored prompt** that shows your current folder.

## How to build it

You need a Linux system with `gcc` and `make` installed.

From the project folder, run:

```
make
```

This produces a program named `42sh`.

## How to run it

```
./42sh
```

You will get a prompt where you can start typing commands, just like a normal
terminal. To quit, type `exit` or press Ctrl+D.

## Configuration

If a file named `.myshrc` is present, the shell reads it at startup. You can
put aliases in it so they are always available. Example:

```
alias c "clear"
alias ll "ls -la"
```

## Cleaning up

- `make clean` removes temporary build files.
- `make fclean` removes everything that was built, including the program.
- `make re` rebuilds the project from scratch.

## Note

This is a student project built for learning purposes. It reimplements shell
features by hand rather than using existing tools, so its goal is education
rather than being a replacement for `bash` or `zsh`.
