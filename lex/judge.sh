#!/bin/bash
cd test
for filename in *.seal; do
    echo "--------Test using" $filename "--------"
    ./lexer $filename > tempfile
    diff tempfile ../test-answer/$filename.out > /dev/null
    if [ $? -eq 0 ] ; then
        echo passed
    else
        ./lexer $filename>$filename.seal.out
        echo NOT passed
    fi
done
rm -f tempfile
cd ..