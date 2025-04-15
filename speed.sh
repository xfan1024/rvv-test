#!/bin/bash

fail_count=0

osname(){
    (source /etc/os-release && echo $PRETTY_NAME)
}

cpuname(){
    local name=$(echo $(lscpu  | grep 'Model name' | cut -d: -f2))
    if [ -z "$name" ]; then
        name="$(uname -m) $(nproc)-core"
    fi
    echo "$name"
}

compiler_version(){
    clang++ --version | head -n1
}

echo "**$(cpuname)**"
echo
echo "- Operation System: $(osname)"
echo "- Compiler: $(compiler_version)"
echo
printf "| %-12s | %-10s | %-10s |\n" "Executable" "norm" "normalize"
printf "| %-12s | %-10s | %-10s |\n" "------------" "----------" "----------"

for exe in speed-*; do
    name=${exe#speed-}
    exe="./$exe"
    if [ ! -x "$exe" ]; then
        continue
    fi

    norm_output=$("$exe" norm 2>&1)
    norm_status=$?
    case $norm_status in
        0)
            ;;
        100)
            norm_output="skip"
            ;;
        *)
            norm_output="fail"
            fail_count=$((fail_count+1))
            ;;
    esac

    normalize_output=$("$exe" normalize 2>&1)
    normalize_status=$?
    case $normalize_status in
        0)
            ;;
        100)
            normalize_output="skip"
            ;;
        *)
            normalize_output="fail"
            fail_count=$((fail_count+1))
            ;;
    esac

    printf "| %-12s | %-10s | %-10s |\n" "$name" "$norm_output" "$normalize_output"
done


if [ $fail_count -ne 0 ]; then
    exit 1
else
    exit 0
fi

