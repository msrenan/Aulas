#!/bin/bash

if [ -n e2 ]; then
    gcc exc2.c -o e2
fi

if (diff -w <(./e2 < ./i02/01.in) ./i02/01.sol) > /dev/null; then
echo "Passed 01"
else 
diff <(./e2 < ./i02/01.in) ./i02/01.sol
fi
if diff -w <(./e2 < ./i02/02.in) ./i02/02.sol > /dev/null; then
echo "Passed 02"
else 
diff <(./e2 < ./i02/01.in) ./i02/01.sol
fi
if diff -w <(./e2 < ./i02/03.in) ./i02/03.sol > /dev/null; then
echo "Passed 03"
else 
diff <(./e2 < ./i02/01.in) ./i02/01.sol
fi
if diff -w <(./e2 < ./i02/04.in) ./i02/04.sol > /dev/null; then
echo "Passed 04"
else 
diff <(./e2 < ./i02/01.in) ./i02/01.sol
fi
echo "Finished"