#!/bin/bash

fail_count=0

printf "| %-24s | %-10s | %-10s |\n" "Executable" "norm" "normalize"
printf "| %-24s | %-10s | %-10s |\n" "------------------------" "----------" "----------"

for exe in speed-*; do
    name=${exe#speed-}
    exe="./$exe"
    if [ ! -x "$exe" ]; then
        continue
    fi

    norm_output=$("$exe" norm 2>&1)
    norm_status=$?
    if [ $norm_status -ne 0 ]; then
        norm_output="fail"
        fail_count=$((fail_count+1))
    fi

    normalize_output=$("$exe" normalize 2>&1)
    normalize_status=$?
    if [ $normalize_status -ne 0 ]; then
        normalize_output="fail"
        fail_count=$((fail_count+1))
    fi

    printf "| %-24s | %-10s | %-10s |\n" "$name" "$norm_output" "$normalize_output"
done

if [ $fail_count -ne 0 ]; then
    exit 1
else
    exit 0
fi

