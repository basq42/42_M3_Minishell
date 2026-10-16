# TEST STUBS - NOT PART OF THE SUBMISSION

**Delete this folder before submitting.**

These files stand in for Bahtiyor's modules until the real ones land, so that
Baran's code (lexer, parser, heredoc, executor, shell, env) can be built, run,
tested and debugged now.

| Stub file | Stands in for | Follows |
|---|---|---|
| `stub_main.c` | `main.c` | implementation guide, Section 4 |
| `stub_signals.c` | `signals/` (+ the global `g_signal`) | Section 12 |
| `stub_expander.c` | `expander/` | Section 12 (returns only 0 or 1; drops unquoted empty words) |
| `stub_builtins.c` | `builtins/` | the 7 subject builtins, output with `write` only |

They define the `PLACE_HOLDER_..._FUNC` names that Baran's code calls.
Simplifications: no word splitting of unquoted `$VAR`; `cd` does not treat
`-...` as options; `exit` does not check for overflow.

## Build and run

```sh
make stubs              # builds ./minishell_stubs (with -g3 -O0 for gdb)
./minishell_stubs
```

`make` (the real target) never compiles anything from this folder.
`make clean` / `make fclean` also remove `obj_stubs/` and `minishell_stubs`.

## valgrind

```sh
valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes \
         --track-fds=yes --suppressions=stubs/readline.supp ./minishell_stubs
```

`readline.supp` hides readline's own leaks (the subject says you do not have to
fix those). Add `--trace-children-skip='*/ls,*/cat,*/wc'` to stop valgrind from
also checking system programs started with `execve`.

## gdb cheat sheet

```sh
gdb ./minishell_stubs
```

| Command | What it does |
|---|---|
| `break build_cmds` | stop when the function starts (`b` for short) |
| `break parser.c:64` | stop at a line |
| `run` / `run < input.txt` | start (the second reads the commands from a file) |
| `next` (`n`) | run one line, stepping OVER function calls |
| `step` (`s`) | run one line, stepping INTO function calls |
| `finish` | run until the current function returns, show its return value |
| `continue` (`c`) | run until the next breakpoint |
| `print *tokens` (`p`) | show a variable; `p tokens->next->value`, `p *cmd->argv@3` |
| `display cur` | print `cur` again after every step |
| `backtrace` (`bt`) | which functions called which, to get here |
| `info locals` | all local variables of the current function |
| `watch cmd->argv` | stop when this value changes |

**Following a child process** (pipelines, heredocs: they run in `fork`ed
children):

```
set follow-fork-mode child     # after fork, debug the CHILD
set detach-on-fork on          # let the parent run on by itself
break child_process
run
```

Use `set follow-fork-mode parent` (the default) to stay in the shell.

**Typing into the shell while debugging:** `run` with no input file lets you
type at the `minishell$ ` prompt in the same terminal. ctrl-C goes to gdb
first; type `continue` to send it on, or use `signal SIGINT`.
