Moving to the bonus part is a significant leap in complexity.

**You need to dig much deeper than just changing the tokenizer and parser.**

Implementing the bonus features requires a fundamental redesign of your core data structures and the execution flow. Let's break down each part.

---

### Part 1: Logical Operators (`&&`, `||`) and Priorities `()`

This is by far the more complex of the two bonuses. It changes your shell's structure from a simple pipeline (a linear list) to a logical tree.

#### The Problem: A Linked List is Not Enough

Your current `t_command` linked list represents a pipeline: `cmd1 -> cmd2 -> cmd3`.
This cannot represent `cmd1 && (cmd2 || cmd3)`. This command is not a simple line; it's a tree structure where operators are parents and commands are children.

#### What Needs to Change:

**1. A New Data Structure: The Abstract Syntax Tree (AST)**

You must replace your linked list of commands with a binary tree. Each node in this tree can be either an operator (`&&`, `||`, `|`) or a simple command.

```c
// in minishell.h

typedef enum e_node_type
{
    NODE_COMMAND,
    NODE_PIPE,
    NODE_AND,
    NODE_OR
} t_node_type;

// This is your new central structure, replacing the t_command linked list.
typedef struct s_ast_node
{
    t_node_type         type;
    struct s_ast_node   *left;
    struct s_ast_node   *right;

    // Payload: only used if type is NODE_COMMAND.
    // Your old t_command struct (without the 'next' pointer) fits perfectly here.
    char                **cmd_args;
    t_redir             *redirections;

} t_ast_node;
```
**Example:** `ls && pwd || cat` would be parsed into this tree:
```
      OR (||)
     /       \
   AND (&&)   COMMAND (cat)
  /       \
COMMAND (ls) COMMAND (pwd)
```

**2. The Parser's New Job: Recursive Parsing**

Your parser needs a complete rewrite. It can no longer be a simple loop that splits by pipes. It must become a **recursive descent parser** that understands operator precedence.
*   **Parentheses `()`:** When the parser sees an opening parenthesis, it must make a recursive call to itself to build a sub-tree for the expression inside. The result of that recursive call becomes a single node in the larger tree.
*   **Precedence:** The parser must handle `&&` and `||` at a lower precedence than `|`. This is a classic parsing challenge.

**3. The Executor's New Job: Recursive Tree Traversal**

Your executor can no longer just loop through a list. It must be a recursive function that traverses the AST you built.

```c
int execute_ast(t_ast_node *node)
{
    if (!node)
        return (0);

    // Base Case: If the node is a simple command, execute it.
    if (node->type == NODE_COMMAND)
    {
        return (execute_simple_command(node)); // Your existing logic for one command
    }

    // Recursive Step: Handle the operators
    if (node->type == NODE_PIPE)
    {
        // ... your complex pipe logic goes here ...
    }

    if (node->type == NODE_AND)
    {
        int left_status = execute_ast(node->left);
        if (left_status == 0) // If left side succeeded
            return execute_ast(node->right);
        else
            return (left_status);
    }

    if (node->type == NODE_OR)
    {
        int left_status = execute_ast(node->left);
        if (left_status != 0) // If left side failed
            return execute_ast(node->right);
        else
            return (left_status);
    }
    return (1); // Should not be reached
}
```

**Verdict:** This bonus requires a deep, fundamental redesign of both the parser and the executor.

---

### Part 2: Wildcards `*` (Globbing)

This feature is less about changing the entire structure and more about **adding a new stage** to your shell's processing pipeline.

#### The Problem: `*` is not just a character

The shell must expand the `*` character into a list of matching filenames *before* the command is executed. `execve` does not understand wildcards; it needs a final, concrete list of arguments.

#### What Needs to Change:

**1. The Parser's Job (Minor Change)**

Your parser's only job is to ensure that a token like `*.c` is treated as a single word. It should not try to interpret the `*`. This is likely already working correctly.

**2. A New Stage: "Globbing"**

You need to create a new module/function that performs wildcard expansion (known as "globbing"). This stage runs *after* parsing and variable expansion, but *before* execution.

**The Globbing Function's Logic:**
`t_command *perform_globbing(t_command *cmd)`

1.  It takes a fully parsed command struct.
2.  It iterates through the `cmd->cmd_args` array.
3.  If an argument string (e.g., `*.c`) contains a `*`:
    a.  Use `opendir(".")` to open the current directory.
    b.  Loop with `readdir()` to read every entry in the directory.
    c.  For each entry, check if it matches the wildcard pattern.
        *   The function `fnmatch()` is the standard C library function for this, but it may not be on your allowed list. If not, you must write your own simple pattern matcher (e.g., for `*.c`, check if the string ends with `.c`).
    d.  Collect all matching filenames into a new temporary list.
4.  If any matches were found:
    a.  Sort the list of matches alphabetically (as `bash` does).
    b.  **Rebuild the `cmd->cmd_args` array.** This is the tricky part. You must create a new, larger `char **` array, copy over the arguments that didn't have a wildcard, and then insert the list of matched filenames in place of the original wildcard argument.
    c.  Free the old `cmd_args` array and point `cmd->cmd_args` to the new one.

**3. The Executor's Job (Integration)**

You need to call this new `perform_globbing()` function from your executor at the right time.

```c
// Inside your main `execute` function
int execute(...)
{
    // ...
    // After parsing is complete:
    shell_data->commands = parse(final_tokens);

    // --- NEW GLOBBING STAGE ---
    if (shell_data->commands)
        perform_globbing_on_all_commands(shell_data->commands);

    // Now execute with the potentially expanded arguments
    status = execute_pipeline_or_ast(shell_data);
    // ...
}
```

**Verdict:** This bonus adds a whole new, non-trivial processing stage to your shell. It deeply affects the final argument list that your executor works with.

### Conclusion

Implementing the bonus part is a major undertaking.
*   **Logical Operators (`&&`, `||`, `()`):** Requires a complete architectural shift from a list to a tree for both parsing and execution. This is a **massive change**.
*   **Wildcards (`*`):** Requires adding a new, complex "globbing" stage that dynamically modifies the command's arguments before execution. This is a **significant addition**.

It is highly recommended to perfect and stabilize the mandatory part on your `main` branch before attempting the bonus features on a completely separate `bonus` branch.
