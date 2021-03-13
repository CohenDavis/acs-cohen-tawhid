Course Mini-Project #3: Memory and Storage Performance Profiling

# Overview

In this project we will try to observe the trade-off between latency and throughput in both memory/cacche and storage systems of a computer. We will try to observe how increasing data access queue depth will result in increased memory/storage utilization, higher throughput, and higher bandwidth while the latency for each request will take longer time. 

For the experiments regarding memory and cache, we will be using Intel Memory Latency Checker to determine the latency in nenoseconds and bandwidth in MB per second for various access sizes and operation types. We will test read only, write only, and read+write operations with various read and write ratios. We will be using 64 byte and 256 byte data access sizes in our experiment. 

For storage experiment, we will first install the Flexible IO Tester (FIO) for Linux using the following command:

```
sudo apt-get install fio
```

Using FIO, we will then conduct similar experiments as the memory experiment. We will then determine the latency in microseconds and bandwidth in KB per second for read only, write only, and read+write operations. We will be using the 4KB and 32KB data access size and set direct to 1 in order to use non-buffered I/O. In order to test using FIO we will run a command similar to this one below:

```
fio --name=random --ioengine=posixaio --rw=randrw --bs=32k --numjobs=1 --size=2g --iodepth=1024 --time_based --end_fsync=1 --direct=1
```
This command is telling fio to create a file named random and perform random read and write operations given by the "--rw=randw" tag. This random file will be 2GB in size and it will be accesed in 32KB access size. Finally the iodepth will be set to 1024. 

# Testing

### Hardware Used During Testing

Dell XPS-9560:
* i7-7700HQ (4C/8T, 2.80GHz Base/3.80GHz Boost)
* GTX 1050 (2GB GDDR5)
* 16GB RAM
* 512 GB NVMe SSD (Samsung PM961)
* Windows 10 64-bit

### Memory & Cache Data

#### Loaded Latency Test - 64B Access Size, Read-Only
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 128.65 | 28439.1
00002 | 136.27 | 27675.9
00008 | 136.74 | 26189.8
00015 | 158.86 | 23349.2
00050 | 138.18 | 23127.8
00100 | 97.45 | 15356.6
00200 | 71.65 | 11327.2
00300 | 71.57 |  8367.2
00400 | 63.96 | 6978.2
00500 | 62.60 | 5881.1
00700 | 64.54 | 4466.7
01000 | 80.94 | 3038.1
01300 | 66.00 | 2826.7
01700 | 65.39 | 2413.1
02500 | 63.53 | 1997.8
03500 | 64.48 | 1694.9
05000 | 68.31 | 1423.6
09000 | 61.35 | 1323.5
20000 | 67.90 | 1061.7

#### Loaded Latency Test - 64B Access Size, Write-Only (Non-Temporal)
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 345.68 | 29295.7
00002 | 419.53 | 28551.6
00008 | 412.90 | 28695.4
00015 | 196.32 | 27080.9
00050 | 80.13 | 11832.8
00100 | 72.06 | 5967.7
00200 | 67.36 | 3821.4
00300 | 65.01 | 2970.5
00400 | 63.45 | 2521.0
00500 | 62.58 | 2249.2
00700 | 61.07 | 1937.3
01000 | 60.35 | 1690.8
01300 | 59.83 | 1555.8
01700 | 59.29 | 1453.1
02500 | 59.15 | 1336.6
03500 | 59.11 | 1264.9
05000 | 58.54 | 1221.7
09000 | 58.54 | 1164.7
20000 | 58.16 | 1132.7

#### Loaded Latency Test - 64B Access Size, 3:1 R:W
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 236.81 | 26656.9
00002 | 236.12 | 26571.2
00008 | 226.16 | 26702.5
00015 | 301.77 | 22778.9
00050 | 226.64 | 25892.8
00100 | 204.10 | 25866.0
00200 | 118.77 | 23682.7
00300 | 100.03 | 16857.7
00400 | 93.64 | 12977.2
00500 | 87.49 | 10712.3
00700 | 79.00 | 7991.1
01000 | 76.94 | 5886.4
01300 | 86.48 | 4424.0
01700 | 73.56 | 3849.2
02500 | 69.41 | 2962.8
03500 | 70.42 | 2353.7
05000 | 69.91 | 1926.3
09000 | 74.26 | 1424.5
20000 | 67.22  | 1209.3

#### Loaded Latency Test - 64B Access Size, 2:1 R:W
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 245.58 | 26632.4
00002 | 242.93 | 26662.8
00008 | 244.64 | 26462.4
00015 | 235.57 | 26584.7
00050 | 210.66 | 26260.0
00100 | 141.38 | 24710.2
00200 | 90.07 | 17640.6
00300 | 77.64 | 12705.0
00400 | 72.28 | 9995.3
00500 | 80.98 | 7805.5
00700 | 66.94 | 6268.2
01000 | 68.34 | 4483.2
01300 | 63.83 | 3886.1
01700 | 64.37 | 3192.5
02500 | 61.29 | 2562.5
03500 | 60.06 | 2158.8
05000 | 59.61 | 1841.7
09000 | 59.14 | 1509.7
20000 | 58.58 | 1285.5

#### Loaded Latency Test - 64B Access Size, 1:1 R:W
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 372.38 | 25247.4
00002 | 378.30 | 25660.6
00008 | 399.49 | 25612.2
00015 | 397.45 | 25401.2
00050 | 338.45 | 25641.8
00100 | 374.03 | 25606.0
00200 | 127.83 | 21891.5
00300 | 95.98 | 15599.5
00400 | 86.91 | 12345.2
00500 | 81.46 | 10151.9
00700 | 77.35 | 7668.6
01000 | 76.11 | 5582.3
01300 | 72.22 | 4547.9
01700 | 70.50 | 3774.6
02500 | 66.43 | 2938.6
03500 | 66.04 | 2386.8
05000 | 66.37 | 1941.1
09000 | 64.58 | 1543.5
20000 | 60.75 | 1308.5


#### Loaded Latency Test - 256B Access Size, Read-Only
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 97.59 | 20346.0
00002 | 91.05 | 21173.9
00008 | 91.07 | 20625.3
00015 | 90.95 | 19514.5
00050 | 93.05 | 15297.3
00100 | 83.23 | 12273.4
00200 | 77.02 | 8779.8
00300 | 74.22 | 6571.8
00400 | 72.67 | 5643.3
00500 | 71.66 | 4943.7
00700 | 69.32 | 4037.9
01000 | 69.34 | 3219.7
01300 | 67.00 | 2784.4
01700 | 67.22 | 2365.6
02500 | 65.83 | 1956.4
03500 | 65.55 | 1689.6
05000 | 65.71 | 1475.8
09000 | 75.54 | 1128.5
20000 | 64.59 | 1118.8

#### Loaded Latency Test - 256B Access Size, Write-Only (Non-Temporal)
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 342.34 | 7587.3
00002 | 348.73 | 7534.3
00008 | 331.89 | 7692.8
00015 | 179.79 | 7614.0
00050 | 86.25 | 3599.4
00100 | 82.80 | 2006.3
00200 | 73.56 | 1577.5
00300 | 92.73 | 1117.4
00400 | 78.66 | 1170.0
00500 | 67.32 | 1255.7
00700 | 69.19 | 1137.5
01000 | 66.64 | 1116.1
01300 | 65.68 | 1095.0
01700 | 66.79 | 1050.1
02500 | 65.51 | 1040.1
03500 | 65.59 | 1021.0
05000 | 65.50 | 1008.9
09000 | 65.86 | 989.5
20000 | 65.66 | 982.8

#### Loaded Latency Test - 256B Access Size, 3:1 R:W
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 146.38 | 21193.0
00002 | 144.62 | 21274.0
00008 | 144.09 | 21221.2
00015 | 152.06 | 20338.0
00050 | 134.65 | 20393.6
00100 | 138.99 | 18133.8
00200 | 115.85 | 15901.9
00300 | 97.55 | 12548.8
00400 | 87.58 | 10218.9
00500 | 82.75 | 8678.2
00700 | 76.77 | 7066.3
01000 | 73.26 | 5656.2
01300 | 71.22 | 4617.6
01700 | 68.43 | 3870.5
02500 | 67.30 | 2995.2
03500 | 66.30 | 2430.3
05000 | 67.14 | 1970.5
09000 | 67.60 | 1514.6
20000 | 64.77 | 1245.7


#### Loaded Latency Test - 256B Access Size, 2:1 R:W
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 218.67 | 19857.0
00002 | 218.82 | 19950.0
00008 | 179.13 | 21026.7
00015 | 164.64 | 21355.6
00050 | 148.78 | 19509.4
00100 | 136.21 | 16807.9
00200 | 129.88 | 13034.0
00300 | 108.08 | 10589.0
00400 | 90.97 | 8101.5
00500 | 101.73 | 6437.6
00700 | 106.34 | 4764.3
01000 | 102.77 | 3583.4
01300 | 90.75 | 3167.7
01700 | 78.44 | 2890.4
02500 | 73.96 | 2300.6
03500 | 71.09 | 1951.1
05000 | 71.28 | 1632.5
09000 | 73.94 | 1276.8
20000 | 74.32 | 1045.4


#### Loaded Latency Test - 256B Access Size, 1:1 R:W
Inject Delay | Latency (ns) | Bandwidth (MB/s)
------------ | ------------- | -------------
00000 | 263.46 | 22084.8
00002 | 275.85 | 22004.3
00008 | 258.58 | 22500.9
00015 | 257.07 | 22349.6
00050 | 220.51 | 22635.4
00100 | 251.10 | 22303.8
00200 | 211.32 | 20220.2
00300 | 138.26 | 15052.3
00400 | 109.43 | 12215.8
00500 | 97.14 | 10158.6
00700 | 88.73 | 7641.4
01000 | 80.62 | 5695.4
01300 | 79.53 | 4531.7
01700 | 71.81 | 3833.8
02500 | 69.11 | 2939.9
03500 | 67.79 | 2389.8
05000 | 66.75 | 1973.9
09000 | 66.10 | 1533.1
20000 | 66.39 | 1218.6

![Graph](https://github.com/CohenDavis/acs-cohen-tawhid/blob/master/Project3/Bandwidth%20vs%20Latency%20-%2064B.PNG)

![Graph](https://github.com/CohenDavis/acs-cohen-tawhid/blob/master/Project3/Bandwidth%20vs%20Latency%20-%20256B.PNG)

### Storage

### Direct is set to 1 and uses 2GB test file

#### Loaded Latency Test - 4KB Access Size, Read-Only
IO Depth | Latency (micro second) | Bandwidth (KB/s)
------------ | ------------- | -------------
00001 | 188.19 | 20520
00008 | 394.66 | 79606
00064 | 2500.35| 100727
00256 | 9371.98 | 105268
00512 | 21930 | 88832
01024 | 57630 | 67829

IOPS for highest bandwidth - 26317

#### Loaded Latency Test - 4KB Access Size, Write-Only
IO Depth | Latency (micro second) | Bandwidth (KB/s)
------------ | ------------- | -------------
00001 | 284.65 | 13481
00008 | 906.90 | 30918
00064 | 6808.18| 32373
00256 | 26808.26 | 32681
00512 | 63170 | 28799
01024 | 127720 | 28201

IOPS for highest bandwidth - 8170

#### Loaded Latency Test - 4KB Access Size, Read and Write
IO Depth | Latency (micro second) | Bandwidth (KB/s)
------------ | ------------- | -------------
00001 | 193.28 | 7521
00008 | 675.62 | 18981
00064 | 5199.64| 21644
00256 | 23259.84 | 18764
00512 | 41940 | 20825
01024 | 100830 | 17925

IOPS for highest bandwidth - 5411

#### Loaded Latency Test - 32KB Access Size, Read-Only
IO Depth | Latency (micro second) | Bandwidth (KB/s)
------------ | ------------- | -------------
00001 | 222.11 | 139438
00008 | 512.08 | 493331
00064 | 3684.70| 550433
00256 | 14643.45 | 546133
00512 | 30070 | 525865
01024 | 67580 | 458193

IOPS for highest bandwidth - 17201

#### Loaded Latency Test - 32KB Access Size, Write-Only
IO Depth | Latency (micro second) | Bandwidth (KB/s)
------------ | ------------- | -------------
00001 | 338.12 | 84658
00008 | 1164.24 | 173218
00064 | 9185.24| 171574
00256 | 33590 | 195120
00512 | 71580 | 184365
01024 | 136570 | 176527

IOPS for highest bandwidth - 6097

#### Loaded Latency Test - 32KB Access Size, Read and Write
IO Depth | Latency (micro second) | Bandwidth (KB/s)
------------ | ------------- | -------------
00001 | 245.76 | 47144
00008 | 666.26 | 128175
00064 | 5317.41 | 134601
00256 | 22801.72 | 127739
00512 | 43300 | 133319
01024 | 94220 | 119320

IOPS for highest bandwidth - 4206

# Analysis 


# Conclusions
