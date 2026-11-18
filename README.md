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

- **Run programs** — type a command like `ls` or `echo hello` and it runs it,
  found through your `PATH`.
- **Pipes** — chain commands so the output of one feeds into the next,
  for example `ls | sort | head`.
- **Redirections** — `>`, `>>`, `<`, `<<` (here-documents), `>&` and `>>&`
  (output and errors together), plus `2>`, `2>>` and `2>&1`.
- **Logical operators** — `&&`, `||` and `;`, and sub-shells with `( )`.
- **Quotes and escapes** — `'single'`, `"double"` and `\` work like in tcsh.
- **Variables** — `set name = value` / `unset`, environment with `setenv` /
  `unsetenv`, and `$name`, `${name}`, `$status`, `$?`, `$$` in commands.
- **Command substitution** — `` `command` `` and `$(command)`.
- **Wildcards (globbing)** — `*`, `?` and `[abc]`, for example `rm *.txt`.
- **`~`** expands to your home folder.
- **Job control** — run a command in the background with `&`, suspend the
  current one with Ctrl+Z, then `jobs`, `fg` and `bg`.
- **History** — `history`, `!!`, `!n`, `!-n`, `!prefix`, `!$`, `!*`, saved in
  `~/.42sh_history` between sessions.
- **Aliases** — `alias ll ls -l` (or `alias ll="ls -l"`), `unalias`, plus the
  `precmd` and `cwdcmd` hooks.
- **Other built-ins** — `cd` (with `cd -` and `cd ~`), `echo`, `env`, `exit`,
  `source`, `which`, `where`, `repeat`, `builtins`.
- **A configuration file** — `~/.myshrc` (or `./.myshrc`) is run at startup,
  so you can put your aliases and variables in it.
- **Scripts** — `./42sh script.sh` or `./42sh -c "command"`. Like tcsh, a
  script stops at the first shell error.

## Keyboard

| Keys | What they do |
| --- | --- |
| ← → | Move the cursor |
| ↑ ↓, Ctrl+P / Ctrl+N | Browse the history |
| Ctrl+R | Search the history as you type |
| Tab | Complete a command, file, folder or `$variable`; press again to list the choices |
| Home / End, Ctrl+A / Ctrl+E | Start / end of the line |
| Ctrl+← / Ctrl+→, Alt+B / Alt+F | Move one word |
| Backspace / Delete, Ctrl+D | Delete a character (Ctrl+D on an empty line quits) |
| Ctrl+W / Alt+D | Delete the previous / next word |
| Ctrl+U / Ctrl+K | Delete to the start / end of the line |
| Ctrl+Y | Paste what was deleted |
| Ctrl+L | Clear the screen |
| Ctrl+C | Cancel the line, or stop the running command |
| Ctrl+Z | Suspend the running command |

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

If `~/.myshrc` exists (or `.myshrc` in the folder you start from), the shell
runs it at startup. Example:

```
alias ll "ls -la"
alias c="clear"
set ignoreeof
```

## Tests

```
make tests_run            # interactive features through a pseudo-terminal
tests/compare_tcsh.sh     # every case of tests/cases.txt, compared with tcsh
```

`compare_tcsh.sh` needs `tcsh` installed. The original Epitech batch tester is
still in `tests/tester.sh`.

## Cleaning up

- `make clean` removes temporary build files.
- `make fclean` removes everything that was built, including the program.
- `make re` rebuilds the project from scratch.

## Note

This is a student project built for learning purposes. It reimplements shell
features by hand rather than using existing tools, so its goal is education
rather than being a replacement for `bash` or `zsh`.
