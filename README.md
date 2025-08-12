# minishell
[minishell project](.test/minishell.pdf) @ shool 42

---

[Writing your own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)

---

Repo structure

```
minishell/
├── .gitignore
├── Makefile
├── README.md
├── include/
│   └── minishell.h
├── libft/
│   ├── Makefile
│   ├── libft.h
│   └── *.c
└── src/
    ├── main.c
    │
    ├── parsing/
    │   ├── utils.c
    │   ├── tokenizer.c
    │   ├── tokenizer_utils.c
    │   ├── parser.c
    │   ├── parser_utils.c
    │   └── expander.c
    │
    ├── execution/
    │   ├── executor.c
    │   ├── executor_utils.c
    │   ├── path_finder.c
    │   ├── redirections.c
    │   ├── redirections_utils.c
    │   └── signals.c
    │
    └── builtins/
        ├── builtin_cd.c
        ├── builtin_echo.c
        ├── builtin_env.c
        └── ... (etc.)

```

---


A detailed plan for dividing the Minishell project between two groups to allow for independent development and testing before integration.

### The Core Principle: Separation of Concerns

The most effective way to divide this project is to separate the **parsing** of the command line from the **execution** of the commands.

*   **Group 1: The Parser Team.** This team is responsible for reading the user's input and translating it into a well-defined data structure. They are the "front-end" of the shell, figuring out *what* the user wants to do.
*   **Group 2: The Execution Team.** This team takes the data structure from the parser and makes it happen. They are the "back-end," managing processes, file descriptors, and built-in logic.

The "contract" between the two groups will be the data structure that represents the parsed command(s). Agreeing on this structure is the first and most critical step before coding begins.

### Proposed Data Structure (The "Contract")

A good way to represent commands is with a linked list of structures, where each structure represents a single command in a pipeline.

```c
// t_redir a linked list of redirections for a command
typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;	// The filename or delimiter
	struct s_redir	*next;
}	t_redir;

// t_command a linked list of commands with arguments and redirections
typedef struct s_command
{
	char				**cmd_args;		/* full arguments array for execve
											(e.g., {"ls", "-l", NULL}) */
	t_redir				*redirections;	// A linked list of redirections
	struct s_command	*next;			// Next command in the pipeline
}	t_command;

// t_shell holds the final command list
typedef struct s_shell
{
	t_command	*commands;
	char		**envp_list;			// our private modifiable env list
	int			last_exit_status;
	char		*debug;					// flag to print tokens/commands/exit_status
}	t_shell;
```
---

## Group 1: The Parser Team (Command Line Interpretation)

This group's goal is to take a raw string from `readline` and produce a fully parsed `t_command` linked list. They will not execute any commands.

### Key Responsibilities:
1.  **Input Reading and History:** Integrate the `readline` library to show a prompt and manage command history.
2.  **Lexical Analysis (Tokenization):** Break the input string into a sequence of "tokens" (e.g., words, operators like `|`, `<`, `>`).
3.  **Parsing:** Analyze the token sequence to build the `t_command` structures. This includes handling quotes and identifying redirections.
4.  **Environment Variable Expansion:** Implement the logic for expanding `$VAR` and `$?`.
5.  **Quote Handling:** Correctly interpret single (`'`) and double (`"`) quotes.

### Development Plan:

**Step 1: The Foundation - Readline and Tokenizer**
*   **Task:** Set up the `Makefile` and main loop. Use `readline` to read a line of input and `add_history` to save it.
*   **Task:** Create a "lexer" (or tokenizer). This function will take the input string and split it into tokens.
    *   `ls -l | wc` should become a list of tokens: `["ls", "-l", "|", "wc"]`.
    *   Handle spaces as delimiters.
    *   Recognize metacharacters (`|`, `<`, `>`, `>>`, `<<`) as separate tokens.
*   **Testing:** You can test this by printing the tokens for various inputs.

**Step 2: Parsing Simple Commands**
*   **Task:** Create the parser function. Initially, it will only handle a single command without pipes or redirections.
*   **Task:** The parser takes the token list and populates a `t_command` struct. The `arguments` array should be `NULL`-terminated, as required by `execve`.
*   **Testing:** Create a test function that feeds the parser an array of tokens (e.g., `{"ls", "-a", NULL}`) and then prints the contents of the resulting `t_command` struct to verify it was populated correctly.

**Step 3: Handling Quotes and Expansions**
*   **Task:** Enhance the tokenizer and parser to handle quotes.
    *   `'echo "$USER"'` should result in one token: `echo "$USER"`.
    *   `"echo '$USER'"` should result in two tokens: `echo`, `'$USER'`.
*   **Task:** Implement environment variable expansion. This should happen *after* tokenizing but *before* final command creation.
    *   Inside double quotes, `$` is active.
    *   Inside single quotes, `$` is ignored.
*   **Testing:** Test with inputs like `echo "Hello, $USER"` and `echo 'Hello, $USER'`. Check that the resulting command structure has the correctly expanded or unexpanded strings.

**Step 4: Parsing Pipelines and Redirections**
*   **Task:** Extend the parser to recognize the `|` pipe character. When a `|` is found, create a new `t_command` struct and link it to the previous one using the `next` pointer.
*   **Task:** Extend the parser to handle redirections (`<`, `>`, `<<`, `>>`). The parser's job is **not** to `open` the files. It should simply identify the redirection type and the associated filename, storing them in temporary fields in the `t_command` struct. The Execution Team will handle the file opening.
*   **Testing:** For an input like `cat < infile | grep 'a' > outfile`, verify that the parser creates two linked `t_command` structs. The first should note an input redirection to `infile`, and the second should note an output redirection to `outfile`.

---

## Group 2: The Execution Team (System Interaction)

This group's focus is on executing the commands described by the `t_command` data structure. They can begin work by creating and testing with manually-created `t_command` structs.

### Key Responsibilities:
1.  **Command Execution:** Execute external commands using `fork` and `execve`.
2.  **Path Resolution:** Find the full path of an executable using the `PATH` environment variable.
3.  **Built-in Commands:** Implement all built-in commands (`echo`, `cd`, `pwd`, etc.).
4.  **Redirection and Pipes:** Manage file descriptors using `pipe()`, `dup2()`, `open()`, and `close()`.
5.  **Signal Handling:** Implement the specified behavior for `Ctrl-C`, `Ctrl-D`, and `Ctrl-\`.

### Development Plan:

**Step 1: Executing a Single External Command**
*   **Task:** Create a function to find an executable in the `PATH`. It takes a command name (e.g., "ls") and returns the full path (e.g., "/bin/ls").
*   **Task:** Create an `execute_command` function. It will `fork()`. The child process will use `execve()` to run the command. The parent process will use `waitpid()` to wait for the child to complete and capture its exit status for `$?`.
*   **Testing:** Manually create a `t_command` struct in your test code (e.g., representing `ls -l`). Pass it to your execution function and verify that it runs correctly.

**Step 2: Implementing Built-in Commands**
*   **Task:** Create a function for each built-in (`builtin_echo`, `builtin_cd`, etc.).
*   **Task:** Create a dispatcher that checks if a command is a built-in. If so, it calls the corresponding function directly in the main process (except for built-ins in a pipe). If not, it calls the external command executor from Step 1.
*   **Testing:** Test each built-in thoroughly. Test `cd` by following it with `pwd`. Test `exit` with an exit code. Test `env`, `export`, and `unset`.

**Step 3: Handling Redirections**
*   **Task:** Create a function to handle I/O redirection. Before a command is executed, this function checks the `t_command` struct for redirection information.
*   **Task:** Use `open()` to get file descriptors for the specified files (`O_CREAT | O_WRONLY | O_TRUNC` for `>`, `O_CREAT | O_WRONLY | O_APPEND` for `>>`, `O_RDONLY` for `<`).
*   **Task:** Use `dup2()` to replace `STDIN_FILENO` or `STDOUT_FILENO` with the new file descriptors.
*   **Task:** Implement here-document (`<<`) logic. This involves reading from standard input until the delimiter is found and piping that input to the command.
*   **Testing:** Test each redirection type with simple commands: `echo "hello" > file`, `cat < file`, `ls >> file`, etc.

**Step 4: Implementing Pipes**
*   **Task:** Create a master `execute_pipeline` function that takes the head of a `t_command` linked list.
*   **Task:** This function will loop through the commands, creating a `pipe()` for each `|`. It will `fork()` for each command, carefully using `dup2()` to connect the write-end of one pipe to the standard output of the current process, and the read-end to the standard input of the next process.
*   **Crucial:** Be meticulous about closing file descriptors in both parent and child processes to avoid hanging.
*   **Testing:** Test with simple pipelines first (`ls | wc`), then add complexity (`cat < file | grep a | wc -l`).

**Step 5: Signal Handling**
*   **Task:** Use `sigaction` to set up handlers for `SIGINT` (Ctrl-C) and `SIGQUIT` (Ctrl-\).
*   **Task:** The `Ctrl-C` handler should make `readline` display a new prompt. When a command is running, `Ctrl-C` should terminate the child process, not the shell.
*   **Task:** Handle `Ctrl-D` (EOF) from `readline`, which should cause the shell to exit cleanly.
*   **Testing:** Press the key combinations during idle prompts and while commands (like `sleep 5`) are running.

---

## Integration Plan: Merging the Branches

1.  **Code Merge:** Merge the Parser team's branch into the Execution team's branch. Resolve any conflicts, primarily in the main loop file.
2.  **Connect Parser to Executor:** The main loop will now look like this:
    *   `line = readline("minishell> ");`
    *   `if (!line) { /* handle Ctrl-D */ }`
    *   `add_history(line);`
    *   `command_list = parse(line, env); // Function from Parser Team`
    *   `status = execute(command_list, &env); // Function from Execution Team`
    *   `free(line);`
    *   `free_command_list(command_list);`
3.  **Refine Redirection Handling:** The Parser identifies redirection files, and the Executor opens them. Ensure this handoff is smooth. The parser might just pass the filename, and the executor performs the `open` call just before `forking`.
4.  **Full-Scale Testing:** Test the entire workflow with complex commands that use every feature:
    *   `cat < Makefile | grep "FLAGS" | wc -l >> counts.txt`
    *   `echo "Your home is $HOME"`
    *   Running built-ins like `cd` and then external commands like `ls`.
    *   All signal handling cases.
    *   Invalid inputs, command-not-found errors, and permission errors.
