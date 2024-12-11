#!bin/bash

# OK

echo -e "OK--------------------------------------------------" > return.txt
echo -e >> return.txt


arg="5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="5 * X^0 + 4 * X^1 = 4 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="6 * X^0 - 5 * X^1 + 1 * X^2 = 0 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="-4 * X^0 - 3 * X^1 + 6 * X^2 = -1 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="1 * X^0 - 5 * X^1 + 3.5 * X^2 = 2 * X^1"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="0 * X^0 + 2 * X^1 - 4.2 * X^2 = 0 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="8 * X^0 - 1.2 * X^1 + 5 * X^2 = 3 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="7 * X^0 + 0 * X^1 - 2.1 * X^2 = -7 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="-3 * X^0 + 9 * X^1 + 0 * X^2 = 4 * X^0"
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="4*X^0+1.5*X^1-3*X^2=-2*X^0"
# スペースがない場合
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="4  *  X^0  +  1.5  *  X^1  -  3  *  X^2  =  -2  *  X^0"
# スペースが連続する場合（1箇所）
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="  4   * X^0+   1.5 *   X^1-   3  * X^2= -2*  X^0"
# スペースが連続する場合（複数箇所）
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="4*X^0+1.5*X^1-3*X^2   =    -2*X^0"
# スペースが右辺に多い場合
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="   4  *  X^0  +  1.5  *  X^1  -  3  *  X^2= -2*X^0"
# スペースが左辺に多い場合
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="4 * X^0 + 1.5 * X^1 - 3 * X^2     =     -2 * X^0"
# スペースが等式の直前と直後に多い場合
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt

arg="4  *   X^0   +  1.5  *   X^1   -  3   *   X^2 =  -2   *   X^0"
# 左辺と右辺の係数の間にスペースがある場合
echo "$arg" >> return.txt
./computor "$arg" >> return.txt
echo -e >> return.txt


# Error

echo -e "Error--------------------------------------------------" >> return.txt
echo -e >> return.txt

arg="8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0"
echo "$arg" >> return.txt
./computor "$arg" 2>> return.txt
echo -e >> return.txt

arg="3 * X^0 + 2 * X^1 - 7.5 * X^2 = 0"
echo "$arg" >> return.txt
./computor "$arg" 2>> return.txt
echo -e >> return.txt

arg="-3 * X^0 + 9 * X^1 + 0 * X2 = 4 * X^0"
echo "$arg" >> return.txt
./computor "$arg" 2>> return.txt
echo -e >> return.txt

arg="-3 * X^0 + 9 * X^1 + 0 * X^2 == 4 * X^0"
echo "$arg" >> return.txt
./computor "$arg" 2>> return.txt
echo -e >> return.txt

arg="-3 * X^0 + 9 * X^1 + 0 * X^2 = 3 * X^0 = 4 * X^0"
echo "$arg" >> return.txt
./computor "$arg" 2>> return.txt
echo -e >> return.txt

arg="*X^0 + 9 * X^1 + 0 * X^2 = 4 * X^0"
echo "$arg" >> return.txt
./computor "$arg" 2>> return.txt
echo -e >> return.txt

arg="*X^0 + 9 * X^1 + 0 * X^2 = 4 * X^0"
echo "$arg" >> return.txt
./computor "$arg" 2>> return.txt
echo -e >> return.txt
