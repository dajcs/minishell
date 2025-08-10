#!/usr/bin/env python3

##############################################################
#
# Splits the Valgrind log into individual “leak blocks.”
# Keeps only definitely/indirectly/possibly lost blocks (i.e., real leaks).
# Excludes any block whose backtrace contains readline,
# libreadline, libhistory, libncurses, or libtinfo (configurable).
# Prints a filtered leak list plus a recalculated Filtered Leak Summary.
#
##############################################################


# alias vgm='valgrind --leak-check=full --show-leak-kinds=all --log-file=.test/valgrind.log ./minishell'


#################################################################
# #
# # Basic
# ./vg_filter.py valgrind.log > minishell_leaks.txt
#
# # Excludes (add more patterns; repeat -x as needed)
# ./vg_filter.py valgrind.log -x libfoo -x "third_party/.+"
#
# # Show 'still reachable' (off by default)
# ./vg_filter.py valgrind.log --include-still-reachable
#
##################################################################

from pathlib import Path
import re
import sys
import argparse
from collections import Counter
from pathlib import Path

DEFAULT_EXCLUDES = [
    r"\breadline\b",
    r"libreadline",
    r"\blibhistory\b",
    r"libtinfo",
    r"libncurses",
    r"\bhistory\b",
    r"\b_rl\w+",
    r"\btget(ent|str|flag)?\b",
    r"\btermcap\b",
    r"\btparm\b",
    r"\btgoto\b",
]

LEAK_KINDS = ("definitely lost", "indirectly lost", "possibly lost")

BLOCK_RE = re.compile(
    r"(?m)^==\d+==\s+\d+\s+bytes\s+in\s+\d+\s+blocks\s+are\s+.*?"
    r"(?=^\s*$|^==\d+==\s+\d+\s+bytes\s+in\s+\d+\s+blocks\s+are|\Z)",
    re.S | re.M,
)

FIRST_LINE_RE = re.compile(
    r"^==\d+==\s+(\d+)\s+bytes\s+in\s+(\d+)\s+blocks\s+are\s+([^ \n]+(?: [^ \n]+)?)",
    re.M,
)

def parse_args():
    p = argparse.ArgumentParser(
        description="Filter Valgrind leak blocks, excluding ones caused by readline."
    )
    p.add_argument("logfile", help="Valgrind log file path (use '-' for stdin).")
    p.add_argument(
        "-x",
        "--exclude",
        action="append",
        default=[],
        help="Regex to exclude blocks if ANY frame matches. Can be passed multiple times.",
    )
    p.add_argument(
        "--include-still-reachable",
        action="store_true",
        help="Include 'still reachable' blocks (off by default).",
    )
    p.add_argument(
        "--only-kinds",
        default=",".join(LEAK_KINDS),
        help="Comma-separated leak kinds to include. Default: '%s'."
        % ",".join(LEAK_KINDS),
    )
    return p.parse_args()

def read_text(path_str: str) -> str:
    if path_str == "-":
        return sys.stdin.read()
    return Path(path_str).read_text(errors="ignore")

def is_leak_kind(block: str, allowed_kinds) -> bool:
    m = FIRST_LINE_RE.search(block)
    if not m:
        return False
    kind = m.group(3)
    return any(kind == k for k in allowed_kinds)

def block_matches_any(block: str, regexes) -> bool:
    return any(r.search(block) for r in regexes)

def main():
    args = parse_args()
    data = read_text(args.logfile)

    # Compile regexes
    exclude_patterns = DEFAULT_EXCLUDES + (args.exclude or [])
    exclude_res = [re.compile(p, re.I) for p in exclude_patterns]

    allowed_kinds = tuple(s.strip() for s in args.only_kinds.split(",")) if args.only_kinds else LEAK_KINDS

    blocks = BLOCK_RE.findall(data)

    kept = []
    summary_bytes = Counter()
    summary_blocks = Counter()

    for b in blocks:
        # Optionally skip "still reachable"
        if not args.include_still_reachable and "still reachable" in b:
            continue
        if not is_leak_kind(b, allowed_kinds):
            continue
        if block_matches_any(b, exclude_res):
            continue

        kept.append(b.rstrip() + "\n")
        m = FIRST_LINE_RE.search(b)
        if m:
            bytes_n = int(m.group(1))
            blocks_n = int(m.group(2))
            kind = m.group(3)
            summary_bytes[kind] += bytes_n
            summary_blocks[kind] += blocks_n

    # Output
    if not kept:
        print("# No matching leak blocks after filtering :D")
        return

    print("# Filtered Valgrind leak blocks (excluding readline et al.)\n")
    for b in kept:
        print(b)
    print("# FILTERED LEAK SUMMARY:")
    for kind in LEAK_KINDS:
        if kind in summary_bytes:
            print(f"#   {kind}: {summary_bytes[kind]} bytes in {summary_blocks[kind]} blocks")
    # Note: deliberately omit 'still reachable' here unless requested.

if __name__ == "__main__":
    main()
