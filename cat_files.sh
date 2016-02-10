#!/bin/bash

# this script prints the content of all the source files in the
# current directory.
# you can use xclip to pipe the output to the clipboard so that it
# can easily be pastet to whereever you want.
#
#   $ ./cat_files.sh | xclip -selection clipboard

echo_80() {
  for ((i = 0; i < 79; i++)) {
    echo -ne "$1"
  }
  echo ""
}

remove_double_newlines() {
  # the sed command makes a single blank line from multiple
  sed -e '/./b' -e :n -e 'N;s/\n$//;tn'
}

for file in cat_files.sh Makefile *.h *.c *.l *.y
do
  echo_80 "="
  echo "===  Dateiname: $file"
  echo_80 "="
  cat $file | remove_double_newlines
  echo ""
done
