#!/usr/bin/sh

set -xe

./a.out $1 > out.cpp
clang out.cpp -include lib.cpp -O3 -o out -std=c++26
clang out.cpp -include lib.cpp -O3 -S -std=c++26
