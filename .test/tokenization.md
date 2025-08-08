Failed tests:

Test   4: ❌ echo hello'world'\
mini output = (hello world)\
bash output = (helloworld)

Test   5: ❌ echo hello""world\
mini output = (hello world)\
bash output = (helloworld)

Test  28: ❌ export HELLO="123 A-"\
mini exit code = 1\
bash exit code = 0\
mini error = ( not a valid identifier)\
bash error = ()


---

### The Grand Unified Theory of Expansion and Splitting

Instead of thinking of separate `tokenize`, `expand`, and `clean` steps, it's better to think of a two-phase process:

**Phase 1: Tokenization (What you're already doing)**
*   **Input:** `export HELLO="123 A-"`
*   **Goal:** Break the raw string into a list of "fields" or potential words. Crucially, things that are "stuck together" (no spaces) must remain as a single token.
*   **Rule:** Split by whitespace **unless** that whitespace is inside quotes.
*   **Correct Output of `tokenize()` for your failed tests:**
    *   `echo hello'world'` -> `["echo", "hello'world'"]`
    *   `echo hello""world` -> `["echo", "hello\"\"world"]`
    *   `export HELLO="123 A-"` -> `["export", "HELLO=\"123 A-\""]`

The rule is: if there's no space in the input, there should be no split between tokens.

**Phase 2: Expansion and Word Splitting (The `expand_and_clean` function needs a major redesign)**

This is where the magic happens. Your current `expand_and_clean` function is likely doing too much splitting and not respecting the "quoting protects from splitting" rule.

Here is the refined, step-by-step logic that `expand_and_clean` should perform **for each token** it receives from Phase 1:

1.  **Expansion:**
    *   Scan through the token string.
    *   If you find a `$` that is **not** inside single quotes, perform variable expansion (`$VAR`, `$?`). This might make the string longer or shorter.
    *   Result: You now have a string with variables expanded but quotes still present.
    *   Example: `"$USER"` becomes `"jdoe"`. `'$USER'` remains `'$USER'`.

2.  **Quote Removal:**
    *   Scan through the expanded string and build a *new* string character by character, removing the quotes that are not themselves quoted.
    *   Example: `"jdoe"` becomes `jdoe`. `'$USER'` becomes `$USER`.
    *   Example: `hello'world'` becomes `helloworld`. (This is the fix for Test 4).
    *   Example: `hello""world` becomes `helloworld`. (This is the fix for Test 5).
    *   Result: You have a single, clean string, post-expansion and post-quote-removal.

3.  **Word Splitting (The CRITICAL Step):**
    *   **This is the key:** Word splitting (splitting a string by spaces, tabs, newlines) happens **only** on the parts of the string that resulted from an **unquoted** variable expansion.
    *   This is a very complex rule to implement perfectly, and for minishell, a slightly simplified version is usually accepted: **If the original token contained no quotes, perform word splitting on the final string. If the original token contained any quotes, do not perform any word splitting at all.**

Let's trace your failing tests with this new logic.

---

### Applying the New Rules

#### Test 4 & 5: `echo hello'world'` and `echo hello""world`

1.  **Phase 1 (Tokenizer):**
    *   Input: `echo hello'world'`
    *   Raw Tokens: `["echo", "hello'world'"]` (Correct!)
2.  **Phase 2 (Expander) on token `"hello'world'"`:**
    *   **Step 2.1 (Expansion):** No `$` found. String remains `hello'world'`.
    *   **Step 2.2 (Quote Removal):** Build a new string.
        *   copy `h`, `e`, `l`, `l`, `o`
        *   see `'`, skip it.
        *   copy `w`, `o`, `r`, `l`, `d`
        *   see `'`, skip it.
        *   Final string: `helloworld`.
    *   **Step 2.3 (Word Splitting):** The original token (`"hello'world'"`) contained quotes. Therefore, **do not split**.
    *   **Result:** The final argument is `helloworld`.
    *   **Final `cmd_args` for `parse()`:** `["echo", "helloworld"]` -> **TEST PASSES**

#### Test 28: `export HELLO="123 A-"`

1.  **Phase 1 (Tokenizer):**
    *   Input: `export HELLO="123 A-"`
    *   Raw Tokens: `["export", "HELLO=\"123 A-\""]` (This is what your tokenizer MUST produce).
2.  **Phase 2 (Expander) on token `"HELLO=\"123 A-\""`:**
    *   **Step 2.1 (Expansion):** No `$` found. String remains `HELLO="123 A-"`.
    *   **Step 2.2 (Quote Removal):** Build a new string.
        *   copy `H`, `E`, `L`, `L`, `O`, `=`
        *   see `"`, skip it.
        *   copy `1`, `2`, `3`, ` `, `A`, `-`
        *   see `"`, skip it.
        *   Final string: `HELLO=123 A-`.
    *   **Step 2.3 (Word Splitting):** The original token contained quotes. Therefore, **do not split**.
    *   **Result:** The final argument is `HELLO=123 A-`.
    *   **Final `cmd_args` for `parse()`:** `["export", "HELLO=123 A-"]` -> **TEST PASSES**

### Summary of Refined Rules

1.  **Tokenizer:** Must not split words if they are not separated by whitespace, regardless of quotes. `a'b'c` is one token. `a"b"c` is one token. `a=b` is one token.

2.  **Expander (`expand_and_clean`):** This function should produce a **list of final arguments** from a **single raw token**. Most of the time, it will be a list with one item, but unquoted expansion can create more.

    **Function Signature:** `char **expand_and_clean_single_token(char *raw_token)`

    **Logic for `expand_and_clean_single_token`:**
    a.  Remember if the `raw_token` had any quotes in it (`bool had_quotes`).
    b.  Perform variable expansion on the token.
    c.  Perform quote removal on the result of (b).
    d.  **If `had_quotes` is `false`**: Perform word splitting on the result of (c). This might produce multiple strings. Return them as a `char **`.
    e.  **If `had_quotes` is `true`**: Do NOT perform word splitting. Return the single string from (c) as a `char **` with one element.

Your main loop would then call this function for each raw token and join the resulting lists of final arguments together.
