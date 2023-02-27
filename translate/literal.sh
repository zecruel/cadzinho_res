#!/bin/sh

for f in $( ls ../src/gui* ) ; do
  echo "------------- $f ----------------"
  sed -n '
    /\".*\"/ {
      =
      p
    }
  ' ../src/$f
done