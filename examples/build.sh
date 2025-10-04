#!/bin/bash

set -xe

clang -std=c99 -I .. -o deque deque.c
clang -std=c99 -I .. -o dynamic_array dynamic_array.c
clang -std=c99 -I .. -o list list.c
clang -std=c99 -I .. -o log log.c
clang -std=c99 -I .. -o read_file read_file.c
clang -std=c99 -I .. -o string_ops string_ops.c
