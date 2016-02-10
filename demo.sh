#!/bin/bash

DIR=examples

show_and_execute() {
  clear
  echo "File: $*"
  cat $DIR/$*
  read -p "Press Enter to Execute $*"
  echo ""
  ./qpl $DIR/$*
  echo ""
  read -p "Press Enter to Continue ..."
}

cat_file() {
  CAT_FILE_CMD="cat $DIR/$*"
  echo ""
  read -p " \$ $CAT_FILE_CMD"
  $CAT_FILE_CMD
  echo ""
}

show_and_execute comments.qpl
show_and_execute funcs1.qpl
show_and_execute stdio.qpl
show_and_execute cmdline.qpl
show_and_execute file_io.qpl
cat_file temp1.txt
cat_file temp2.txt
read -p ""
show_and_execute strreplace.qpl
show_and_execute rtti.qpl
show_and_execute fibonacci.qpl
show_and_execute primes.qpl

clear
echo ""
echo "  Thanks :-) "
echo ""
echo ""

