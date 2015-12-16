#!/bin/bash

color=
names=
if [ "$1" = '--color' ]; then
    color=1
    shift 1
elif [ "$1" = '--names' ]; then
    names=1
    shift 1
fi

if [ -z "$1" ]; then
    if [ "$(git rev-parse --is-inside-work-tree 2>&-)" != "true" ]; then
        echo "Must either specify a directory or run from within the git" >&2
        echo "work tree!" >&2
        exit 1
    fi
    cd "./$(git rev-parse --show-cdup)"
    root=.
elif ! [ -d "$1" ]; then
    echo "Error: \"$1\" does not seem to be a valid directory." >&2
    exit 1
else
    root="$1"
fi

function format {
    e=p
    if [ -n "$color" ]; then
        e='s/^(.*):(\s*#.*[defin]{6}\s+)([A-Za-z0-9_]+)(\s+.*)?/'
        e="$e"$'\e[35m\\1\e[32m:\e[0m\\2\e[1;31m\\3\e[0m\\4/;p'
    elif [ -n "$names" ]; then
        e='s/:\s*#.*//;p'
    fi
    sed -rn "$e"
}

find "$root" -name '*.h' -print0 | \
xargs -0 grep -Em2 '#\s*(ifndef|define)' | \
grep -vE '(^|/)([^/]*)\.h.*__\2_h' | \
format
