# 	Assignment 1 

> Author : 胡子昂，何子龙

## V1

## 文件夹内容解释：

- bin/ ——可执行程序文件夹(Mac环境下编译)
- src/ —— 源代码文件夹
- output/ —— 存放输出结果文件，文件名第一个数字为n，第二个数子为指定核数
- src/as1.cpp —— 提交程序
- src/test.h —— 用于计时头文件
- src/isprime.cpp —— 用于测试程序结果是否正确

## 程序运行时间：

~~~bash
huziangdeMacBook-Pro:1 huziang$ ./as1 1000
Init time: 34 us
Running time: 584 us
Print time: 1078 us
huziangdeMacBook-Pro:1 huziang$ ./as1 2000
Init time: 50 us
Running time: 765 us
Print time: 1442 us
huziangdeMacBook-Pro:1 huziang$ ./as1 4000
Init time: 39 us
Running time: 997 us
Print time: 2117 us
huziangdeMacBook-Pro:1 huziang$ ./as1 8000
Init time: 46 us
Running time: 1471 us
Print time: 4394 us
huziangdeMacBook-Pro:1 huziang$ ./as1 16000
Init time: 52 us
Running time: 1966 us
Print time: 6715 us
huziangdeMacBook-Pro:1 huziang$ ./as1 32000
Init time: 65 us
Running time: 3163 us
Print time: 10864 us
huziangdeMacBook-Pro:1 huziang$ ./as1 64000
Init time: 99 us
Running time: 3845 us
Print time: 19580 us
huziangdeMacBook-Pro:1 huziang$ ./as1 128000
Init time: 144 us
Running time: 5056 us
Print time: 28861 us
huziangdeMacBook-Pro:1 huziang$ ./as1 256000
Init time: 291 us
Running time: 8944 us
Print time: 62052 us
huziangdeMacBook-Pro:1 huziang$ ./as1 512000
Init time: 441 us
Running time: 12894 us
Print time: 97867 us
~~~

提升率如下：

| N的大小（*10^3） | 时间增长率  |
| :--------------: | :---------: |
|        1         |      -      |
|        2         | 1.309931507 |
|        4         | 1.303267974 |
|        8         | 1.475426279 |
|        16        | 1.336505778 |
|        32        | 1.608850458 |
|        64        | 1.215618084 |
|       128        | 1.314954486 |
|       256        | 1.768987342 |
|       512        | 1.441636852 |

由于系统核数的问题，当n增加一倍时，时间增长大概在1.3-1.4之间。

## 程序正确性测试：

~~~bash
huziangdeMacBook-Pro:1 huziang$ ./isprime answer1000_16.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer2000_23.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer4000_32.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer8000_45.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer16000_64.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer32000_90.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer64000_127.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer128000_179.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer256000_253.txt
All number test over!
huziangdeMacBook-Pro:1 huziang$ ./isprime answer512000_358.txt
All number test over!
~~~

所有答案均正确！

## V2

## 更新部分：

- 将输出0/1改成输出的质数值
- 计时修改成每个线程独自计时


## 程序运行时间：

见output文件夹中的time文件。
