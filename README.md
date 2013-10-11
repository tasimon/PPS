==================================
Parallel Performance Summarizer: PPS
v0.1 Oct. 11 2013 Tyler Simon

inputs: measured application runtimes and core count and number of nodes
outputs: performance and power usage for given system parameters

To Compile:

make PPS 

To Run:
./PPS <inputfile> 

inputfile must be formatted as follows:

[cores] [time(seconds)] [nodes(how many nodes were used)]


Example Run:
[tsimo1@intel01 PPSv01]$ bin/PPS data/ua_b_west_omp.txt | column -t
#Runtime  Speedup  Par_frac  Cores     Eff.   perf_watt  Amdahl(S)   A_MAX      Gust(SS)  K_F   mult_S    Watts   Joules   T_O      K
#-------  -------  -----     --------  -----  --------   ----------  ---------  ------    ---   --------  ------  ------   ---      -
318.87    1.000    1.000     1         1.000  1.000      1.000       0.00       1.00      0.00  1.00      4096    956.61   0.00     0.00
177.70    1.794    0.885     2         0.897  0.95       1.794       8.70       1.88      0.11  2.19      8192    1066.20  36.53    8.73
94.77     3.365    0.937     4         0.841  0.91       3.364       15.87      3.81      0.06  4.20      16384   1137.24  60.21    5.30
72.87     4.376    0.926     6         0.729  0.84       4.380       13.51      5.63      0.07  6.26      24576   1311.66  118.35   2.69
58.90     5.414    0.932     8         0.677  0.81       5.420       14.71      7.52      0.07  8.26      32768   1413.60  152.33   2.09
45.03     7.081    0.937     12        0.590  0.74       7.088       15.87      11.31     0.06  12.19     49152   1621.08  221.49   1.44
80.86     3.943    0.797     16        0.246  0.40       3.956       4.93       12.95     0.20  10.99     65536   3881.28  974.89   0.33
79.22     4.025    0.776     32        0.126  0.22       4.028       4.46       25.06     0.22  13.44     131072  7605.12  2216.17  0.14


./runPPS.sh
compiles and runs PPS on all of the .txt files in the data directory

==================================
Communication performance: commperf

input: a file containing measurements of bandwidth and latency for byte sizes
output: predicted total communication time and latency for user entered message size and number of nodes.

To Compile: 
make COMM

To Run:
./commperf <message size> <number of nodes>

Example Run:
[tsimo1@intel01 PPSv01]$ ./bin/commperf data/comm_data.dat 10342 231

The linear equation that best fits the given data for latency:
       y =   0.01x +  66.88
--------------------------------------------------
   Original (x,y)     Estimated y     Residual
--------------------------------------------------
   (  0.00  31.03)       66.88       -35.85
   (  1.00  30.37)       66.89       -36.52
   (  2.00  29.49)       66.90       -37.41
   (  4.00  28.90)       66.91       -38.01
   (  8.00  29.00)       66.95       -37.95
   ( 16.00  31.89)       67.02       -35.13
   ( 32.00  37.70)       67.15       -29.45
   ( 64.00  38.32)       67.43       -29.11
   (128.00  35.74)       67.98       -32.24
   (256.00  36.90)       69.08       -32.18
   (512.00  42.53)       71.27       -28.74
   (1024.00  65.19)       75.67       -10.48
   (2048.00  77.98)       84.45        -6.47
   (4096.00 100.23)      102.02        -1.79
   (8192.00 154.51)      137.17        17.34
   (16384.00 251.08)      207.46        43.62
   (32768.00 388.24)      348.03        40.21
   (65536.00 669.77)      629.18        40.59
   (131072.00 1294.74)      1191.49       103.25
   (262144.00 2416.87)      2316.10       100.77
   (524288.00 4629.04)      4565.32        63.72
   (1048576.00 9101.91)      9063.76        38.15
   (2097152.00 18004.31)      18060.64       -56.33
--------------------------------------------------
Residual sum = 47879.24
 r^2 =   1.00
--------------------------------------------------
latency(us) = 155.6148
msgsize(bytes) = 10342.0
Bandwidth(MB/s) = 113.73
nodes = 231
Expected Communication Time(s) = 0.0560


==================================
Derivative
./deravative <file>

input: file of x,y values
output: deravitive (f'(x)) and x y values.

To Compile:
make DERIV

Example Run: 
[tsimo1@intel01 PPSv01]$ bin/derivative data/derivtest.dat 
#[index] [xvalue] [y value] [f'(x)]
0 5 0.062 0.0023
1 15 0.085 0.0044
2 20 0.107 0.008
3 22 0.123 0.0075
4 24 0.138 -0.016
5 23.5 0.146 0.05
6 23.8 0.161 0.065
7 24 0.174 0.09
8 24.1 0.183 0.11
9 24.2 0.194 0.39
10 24.3 0.233 0.83
11 24.4 0.316 0.24
12 24.5 0.34 0.11
13 24.6 0.351 0.07
14 24.7 0.358 0.05
15 25 0.373 0.024
16 25.5 0.385 0.022
17 26 0.396 0.015

================================

