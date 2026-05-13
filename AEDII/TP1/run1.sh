#!/bin/bash

if [ -n e1 ]; then
    gcc exc1.c -o e1
fi

if (diff -w <(./e1 < ./i01/01.in) ./i01/01.sol) > /dev/null; then
echo "Passed 01"
else 
diff <(./e1 < ./i01/01.in) ./i01/01.sol
fi
if diff -w <(./e1 < ./i01/02.in) ./i01/02.sol > /dev/null; then
echo "Passed 02"
fi
if diff -w <(./e1 < ./i01/03.in) ./i01/03.sol > /dev/null; then
echo "Passed 03"
fi
if diff -w <(./e1 < ./i01/04.in) ./i01/04.sol > /dev/null; then
echo "Passed 04"
fi
echo "Finished"