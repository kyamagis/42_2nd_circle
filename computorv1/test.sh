#!bin/bash

# OK

echo -e "OK--------------------------------------------------" > return.txt
echo -e >> return.txt

echo "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" >> return.txt
./computor "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0" >> return.txt
echo -e >> return.txt

echo "5 * X^0 + 4 * X^1 = 4 * X^0" >> return.txt
./computor "5 * X^0 + 4 * X^1 = 4 * X^0" >> return.txt
echo -e >> return.txt

echo "6 * X^0 - 5 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
./computor "6 * X^0 - 5 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
echo -e >> return.txt

echo "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
./computor "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
echo -e >> return.txt

echo "-4 * X^0 - 3 * X^1 + 6 * X^2 = -1 * X^0" >> return.txt
./computor "-4 * X^0 - 3 * X^1 + 6 * X^2 = -1 * X^0" >> return.txt
echo -e >> return.txt

echo "1 * X^0 - 5 * X^1 + 3.5 * X^2 = 2 * X^1" >> return.txt
./computor "1 * X^0 - 5 * X^1 + 3.5 * X^2 = 2 * X^1" >> return.txt
echo -e >> return.txt

echo "0 * X^0 + 2 * X^1 - 4.2 * X^2 = 0 * X^0" >> return.txt
./computor "0 * X^0 + 2 * X^1 - 4.2 * X^2 = 0 * X^0" >> return.txt
echo -e >> return.txt

echo "8 * X^0 - 1.2 * X^1 + 5 * X^2 = 3 * X^0" >> return.txt
./computor "8 * X^0 - 1.2 * X^1 + 5 * X^2 = 3 * X^0" >> return.txt
echo -e >> return.txt

echo "7 * X^0 + 0 * X^1 - 2.1 * X^2 = -7 * X^0" >> return.txt
./computor "7 * X^0 + 0 * X^1 - 2.1 * X^2 = -7 * X^0" >> return.txt
echo -e >> return.txt

echo "-3 * X^0 + 9 * X^1 + 0 * X^2 = 4 * X^0" >> return.txt
./computor "-3 * X^0 + 9 * X^1 + 0 * X^2 = 4 * X^0" >> return.txt
echo -e >> return.txt

# スペースがない場合
echo "4*X^0+1.5*X^1-3*X^2=-2*X^0" >> return.txt
./computor "4*X^0+1.5*X^1-3*X^2=-2*X^0" >> return.txt
echo -e >> return.txt

# スペースが連続する場合（1箇所）
echo "4  *  X^0  +  1.5  *  X^1  -  3  *  X^2  =  -2  *  X^0" >> return.txt
./computor "4  *  X^0  +  1.5  *  X^1  -  3  *  X^2  =  -2  *  X^0" >> return.txt
echo -e >> return.txt

# スペースが連続する場合（複数箇所）
echo "  4   * X^0+   1.5 *   X^1-   3  * X^2= -2*  X^0" >> return.txt
./computor "  4   * X^0+   1.5 *   X^1-   3  * X^2= -2*  X^0" >> return.txt
echo -e >> return.txt

# スペースが右辺に多い場合
echo "4*X^0+1.5*X^1-3*X^2   =    -2*X^0" >> return.txt
./computor "4*X^0+1.5*X^1-3*X^2   =    -2*X^0" >> return.txt
echo -e >> return.txt

# スペースが左辺に多い場合
echo "   4  *  X^0  +  1.5  *  X^1  -  3  *  X^2= -2*X^0" >> return.txt
./computor "   4  *  X^0  +  1.5  *  X^1  -  3  *  X^2= -2*X^0" >> return.txt
echo -e >> return.txt

# スペースが等式の直前と直後に多い場合
echo "4 * X^0 + 1.5 * X^1 - 3 * X^2     =     -2 * X^0" >> return.txt
./computor "4 * X^0 + 1.5 * X^1 - 3 * X^2     =     -2 * X^0" >> return.txt
echo -e >> return.txt

# 左辺と右辺の係数の間にスペースがある場合
echo "4  *   X^0   +  1.5  *   X^1   -  3   *   X^2 =  -2   *   X^0" >> return.txt
./computor "4  *   X^0   +  1.5  *   X^1   -  3   *   X^2 =  -2   *   X^0" >> return.txt
echo -e >> return.txt

# Error

echo -e "Error--------------------------------------------------" >> return.txt
echo -e >> return.txt

echo "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0" >> return.txt
./computor "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0" 2>> return.txt
echo -e >> return.txt

echo "3 * X^0 + 2 * X^1 - 7.5 * X^2 = 0" >> return.txt
./computor "3 * X^0 + 2 * X^1 - 7.5 * X^2 = 0" 2>> return.txt
echo -e >> return.txt

# echo "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
# ./computor "4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0" >> return.txt
# echo -e >> return.txt
