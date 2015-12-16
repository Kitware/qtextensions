#!/bin/bash

scripts="$(dirname $0)"

tabwidth=8
replace=
options="$scripts/.astyle"

while getopts 'o:a:t:r' opt; do
  case $opt in
    o) options="$OPTARG";;
    r) replace=1;;
    t) tabwidth=$OPTARG;;
    *) echo "$0: Invalid option, $opt" >&2; exit 1;;
  esac
done
shift $(( OPTIND - 1 ))

[ -r "$options" ] || options=/dev/null

copyyear="$(date +%Y)"

for f in "$@"; do (
  ulimit -v 2097152
  t1="$(mktemp)"
  t2="$(mktemp)"

  # - Expand tabs.
  # - Squeeze multiple blank lines, make sure file ends with newline, strip
  #   leading or trailing blank lines.
  # - Format with qtestyle. and ppstyle.
  ( echo; expand -t$tabwidth "$f"; echo ) | \
  cat -s | sed '1d;$d' | "$scripts"/qtestyle.pl | "$scripts"/ppstyle.pl >> "$t1"
  # - Format with astyle; run against a temporary file because astyle has been
  #   known to hang when run against a pipe.
  # - Format with qtestyle-post to fix some things that astyle doesn't catch or
  #   gets wrong.
  # - Apply more sed magic to update copyright.
  astyle --options="$options" < "$t1" | "$scripts"/qtestyle-post.py | \
  sed "/Copyright/s/Copyright [[:digit:]]\{2,4\}/Copyright $copyyear/" >> "$t2"
  rm -f "$t1"

  # Replace input file or dump result to stdout
  if [ $replace ]; then
    echo "$f"
    cmp -s "$t2" "$f"
    if [ $? -eq 1 ]
      then mv "$t2" "$f"
      else rm -f "$t2"
    fi
  else
    cat "$t2"
    rm -f "$t2"
  fi
) done
