#!bin/bash

# OK

if [ "$1" = "debug" ]; then
    text="return_debug.txt"
else
    text=return.txt
fi

echo -e "OK--------------------------------------------------" > $text
echo -e >> $text


echo subject >> $text
arg="5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo subject >> $text
arg="5 * X^0 + 4 * X^1 = 4 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "係数が整数の場合" >> $text
arg="6 * X^0 - 5 * X^1 + 1 * X^2 = 0 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "重解の場合1" >> $text
arg="-4 * X^0 - 3 * X^1 + 6 * X^2 = -1 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "右辺に項がある場合1" >> $text
arg="4 * X^0 - 4 * X^1 + 1 * X^2 = 0 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "右辺に項がある場合2" >> $text
arg="1 * X^0 - 5 * X^1 + 3.5 * X^2 = 2 * X^1"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "実数解を2つ持つ場合1" >> $text
arg="0 * X^0 + 2 * X^1 - 4.2 * X^2 = 0 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "実数解を2つ持つ場合2" >> $text
arg="7 * X^0 + 0 * X^1 - 2.1 * X^2 = -7 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "虚数解を持つ場合" >> $text
arg="8 * X^0 - 1.2 * X^1 + 5 * X^2 = 3 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "一次関数の場合" >> $text
arg="-3 * X^0 + 9 * X^1 + 0 * X^2 = 4 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "重解の場合2" >> $text
arg="4*X^0+1.5*X^1-3*X^2=-2*X^0"
# スペースがない場合
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "構文解釈テスト1" >> $text
arg="4  *  X^0  +  1.5  *  X^1  -  3  *  X^2  =  -2  *  X^0"
# スペースが連続する場合（1箇所）
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "構文解釈テスト2" >> $text
arg="  4   * X^0+   1.5 *   X^1-   3  * X^2= -2*  X^0"
# スペースが連続する場合（複数箇所）
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "構文解釈テスト3" >> $text
arg="4*X^0+1.5*X^1-3*X^2   =    -2*X^0"
# スペースが右辺に多い場合
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "構文解釈テスト4" >> $text
arg="   4  *  X^0  +  1.5  *  X^1  -  3  *  X^2= -2*X^0"
# スペースが左辺に多い場合
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "構文解釈テスト5" >> $text
arg="4 * X^0 + 1.5 * X^1 - 3 * X^2     =     -2 * X^0"
# スペースが等式の直前と直後に多い場合
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "構文解釈テスト6" >> $text
arg="4  *   X^0   +  1.5  *   X^1   -  3   *   X^2 =  -2   *   X^0"
# 左辺と右辺の係数の間にスペースがある場合
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "判別式が 1.0 未満の場合 " >> $text
arg="0.125 * X^0 + 1.0 * X^2 = 0 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "R1" >> $text
arg="5 * X^0 = 5 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "R2" >> $text
arg="4 * X^0 = 8 * X^0"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "R3" >> $text
arg="5 * X^0 = 4 * X^0 + 7 * X^1"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "R4" >> $text
arg="5 * X^0 + 13 * X^1 + 3 * X^2 = 1 * X^0 + 1 * X^1"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

echo "R5" >> $text
arg="6 * X^0 + 11 * X^1 + 5 * X^2 = 1 * X^0 + 1 * X^1"
echo "$arg" >> $text
./computor "$arg" >> $text
echo -e >> $text

# Error

echo -e "Error--------------------------------------------------" >> $text
echo -e >> $text

echo "三次方程式の場合" >> $text
arg="8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0"
echo "$arg" >> $text
./computor "$arg" 2>> $text
echo -e >> $text

echo "a * x^b の 形式をとらない場合1" >> $text
arg="3 * X^0 + 2 * X^1 - 7.5 * X^2 = 0"
echo "$arg" >> $text
./computor "$arg" 2>> $text
echo -e >> $text

echo "a * x^b の 形式をとらない場合2" >> $text
arg="-3 * X^0 + 9 * X^1 + 0 * X2 = 4 * X^0"
echo "$arg" >> $text
./computor "$arg" 2>> $text
echo -e >> $text

echo "a * x^b の 形式をとらない場合3" >> $text
arg="*X^0 + 9 * X^1 + 0 * X^2 = 4 * X^0"
echo "$arg" >> $text
./computor "$arg" 2>> $text
echo -e >> $text

echo "= が二個ある場合1" >> $text
arg="-3 * X^0 + 9 * X^1 + 0 * X^2 == 4 * X^0"
echo "$arg" >> $text
./computor "$arg" 2>> $text
echo -e >> $text

echo "= が二個ある場合2" >> $text
arg="-3 * X^0 + 9 * X^1 + 0 * X^2 = 3 * X^0 = 4 * X^0"
echo "$arg" >> $text
./computor "$arg" 2>> $text
echo -e >> $text

echo "指数が整数でない場合1" >> $text
arg="1*X^0 + 9 * X^1.1 + 0 * X^2 = 4 * X^0"
echo "$arg" >> $text
./computor "$arg" 2>> $text
echo -e >> $text
