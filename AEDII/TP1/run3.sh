#!/bin/bash

if [ -n e3 ]; then
    gcc exc3.c -o e3
fi

if (diff -w <(./e3 < ./i03/01.in) ./i03/01.sol) > /dev/null; then
echo "Passed 01"
else 
diff -w <(./e3 < ./i03/01.in) ./i03/01.sol
fi
if diff -w <(./e3 < ./i03/02.in) ./i03/02.sol > /dev/null; then
echo "Passed 02"
else 
diff -w <(./e3 < ./i03/01.in) ./i03/01.sol
fi
if diff -w <(./e3 < ./i03/03.in) ./i03/03.sol > /dev/null; then
echo "Passed 03"
else 
diff -w <(./e3 < ./i03/01.in) ./i03/01.sol
fi
if diff -w <(./e3 < ./i03/04.in) ./i03/04.sol > /dev/null; then
echo "Passed 04"
else 
diff -w <(./e3 < ./i03/01.in) ./i03/01.sol
fi
if diff -w <(./e3 < ./i03/05.in) ./i03/05.sol > /dev/null; then
echo "Passed 05"
else 
diff -w <(./e3 < ./i03/05.in) ./i03/05.sol
fi
echo "Finished"