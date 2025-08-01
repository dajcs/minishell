# minishell
[minishell project](.test/minishell.pdf) @ shool 42

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
    │   ├── 01_tokenizer.c
    │   ├── 02_parser.c
    │   ├── 03_expander.c
    │   └── 04_quotes.c
    │
    ├── execution/
    │   ├── 01_executor.c
    │   ├── 02_path_finder.c
    │   ├── 03_redirections.c
    │   └── 04_pipes.c
    │
    ├── builtins/
    │   ├── builtin_cd.c
    │   ├── builtin_echo.c
    │   ├── builtin_env.c
    │   └── ... (etc.)
    │
    └── shared/
        ├── signals.c
        └── error_handling.c
```

---
