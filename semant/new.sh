#!/bin/bash
make clean
make semant
./semant ./test/test$1.seal
