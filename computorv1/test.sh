#!bin/bash

# OK

echo -e "OK" > return.txt

echo "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" >> return.txt
./computor "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" >> return.txt
echo -e >> return.txt


echo "5 * X^0 + 4 * X^1 = 4 * X^0" >> return.txt
./computor "5 * X^0 + 4 * X^1 = 4 * X^0" >> return.txt
echo -e

echo "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0" >> return.txt
./computor "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0" >> return.txt
echo -e >> return.txt


echo "6 * X^0 - 5 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
./computor "6 * X^0 - 5 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
echo -e >> return.txt

echo "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
./computor "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
echo -e >> return.txt

echo "4 * X^0 - 4 * X^1 + 1 * X^2=0 * X^0" >> return.txt
./computor "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
echo -e >> return.txt

# Error

echo -e "Error" >> return.txt

# echo "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
# ./computor "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
# echo -e >> return.txt
