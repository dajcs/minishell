
# Minishell

## Mandatory part

- program name: `minishell`
- files: Makefile, *.h, *.c
- Makefile: NAME, all, clean, fclean, re
- allowed external functions: `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`, `printf`, `malloc`, `free`, `write`, `access`, `open`, `read`,
`close`, `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `signal`, `sigaction`, `sigemptyset`, `sigaddset`, `kill`, `exit`, `getcwd`, `chdir`, `stat`, `lstat`, `fstat`, `unlink`, `execve`, `dup`, `dup2`, `pipe`, `opendir`, `readdir`, `closedir`, `strerror`, `perror`, `isatty`, `ttyname`, `ttyslot`, `ioctl`, `getenv`, `tcsetattr`, `tcgetattr`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`
- additionally all functions from `libft`
- description: write a shell

The shell should:
- display a **prompt** when waiting for a new command
- have a working history
- search and launch the right executable (based on the PATH variable or using a relative or an absolute path)
- Use at most **one global variable** to indicate a received signal --- however, this approach ensures that the signal handler will not access the main data structures.
  - **Careful** -- This global variable must only store the signal number and must not provide any additional info or access to data -- meaning that using "norm" type structures in the global scope is forbidden.
- do not interpret unclosed quotes or special characters which are not required by the subject, such as `\` (backslash) or `;` (semicolon)
- handle `'` (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence
- handle `"` (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence, except for `$` (dollar) sign.
- implement the following **redirections**:
  - `<` should redirect input
  - `>` should redirect output
  - `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn't have to update the history!
  - `>>` should redirect output in append mode
- implement **pipes** (`|` character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
- handle **environment variables** (`$` followed by a sequence of characters) which should expand to their values.
- handle `$?` which should expand to the exit status of the most recently executed foreground pipeline.
- handle `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` which should behave like in bash.
- in interactive mode:
  - `Ctrl-C` displays a new prompt on a new line.
  - `Ctrl-D` exits the shell.
  - `Ctrl-\` does nothing.
- the shell must implement the following **built-in** commands:
  - `echo` with option `-n`
  - `cd` with only a relative or absolute path
  - `pwd` with no options
  - `export` with no options
  - `unset` with no options
  - `env` with no options or arguments
  - `exit` with no options

- the `readline()` function may cause memory leaks, we are not required to fix them, but the code written by us can't have memory leaks.

- **info**:
  - the implementation should limit to the subject description
  - anything that is not asked is not required
  - if there are doubts about a requirement then [bash](https://www.gnu.org/savannah-checkouts/gnu/bash/manual/) is the reference.


## Bonus part

The bonus part will implement:
- `&&` and `||` with parenthesis for priorities
- wildcards `*` should work for the current working directory


## The Norminette rules

- only while loops are allowed
- ternary operators are not allowed
- variable definition and variable assignment in separate rows
- one empty line after variable definitions, no other empty lines
- only one variable definition/assignment on a line
- only one instruction on a single line
- max 80 characters / line
- max 4 parameters / function
- max 5 variables / function
- max 25 lines / function
- max 5 functions / .c file
