#!/bin/bash
# Runs every case of tests/cases.txt through tcsh and through 42sh and shows the
# differences. Meant to run where tcsh is installed (see tests/README.md).
SH42="$(realpath "${1:-./42sh}")"
CASES="$(realpath "$(dirname "$0")/cases.txt")"
pass=0
fail=0

run_case() { # <shell> <input>
    local dir
    dir=$(mktemp -d)
    (cd "$dir" && printf '%s\n' "$2" | env -i PATH=/usr/bin:/bin HOME="$dir" TERM=xterm $1 2>&1; echo "[exit $?]")
    rm -rf "$dir"
}

compare() { # <name> <input>
    local ref mine
    ref=$(run_case "tcsh -f" "$2")
    mine=$(run_case "$SH42" "$2")
    if [ "$ref" == "$mine" ]; then
        pass=$((pass + 1))
        return
    fi
    fail=$((fail + 1))
    echo "=== FAIL: $1"
    diff <(echo "$ref") <(echo "$mine") | sed 's/^</  tcsh:/; s/^>/  42sh:/'
}

name=""
body=""
while IFS= read -r line || [ -n "$line" ]; do
    if [[ "$line" == "### "* ]]; then
        [ -n "$name" ] && compare "$name" "$body"
        name="${line#\#\#\# }"
        body=""
    else
        body+="$line"$'\n'
    fi
done < "$CASES"
[ -n "$name" ] && compare "$name" "$body"
echo "passed: $pass, failed: $fail"
[ "$fail" -eq 0 ]
