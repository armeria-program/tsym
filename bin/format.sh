#!/usr/bin/env bash

CLANGFMT="clang-format --style=file"

if ! $CLANGFMT --version | grep -qs '[5-9]\.[0-9]'; then
    echo "Version requirement of clang format not met, >= 5 is required"
    exit 1
fi

inplace=""
files=`git ls-files -- '*.h' '*.cpp'`

if [ $# -gt 0 -a "$1" = "-i" ]; then
    inplace="-i"
    shift
fi

if [ -n "`echo $@`" ]; then
    files=$@
fi

tmpfile=`mktemp`

for file in `ls $files`; do
    if [ -z "$inplace" ]; then
        diff --color -u <(cat $file) <($CLANGFMT $file)
    else
        cp $file $tmpfile
        clang-format $inplace --style=file $file
        diff -q $tmpfile $file > /dev/null || echo "$file changed..."
    fi
done

rm -f "$tmpfile"
