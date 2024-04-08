---
marp: false
theme: beamer
paginate: true
---

# Seminar for Development of Practical Simulation Softwares (HPC basics)

## 平野 敏行 (Toshiyuki HIRANO)

## `t-hirano [at] iis.u-tokyo.ac.jp`

## 2023/04/18

本資料は marp(<https://marp.app/>)で作成されています。

---

# Introduction

---

# Aims

- learn basics of the HPC-programing
  - basics of HPC hardware
  - basics of prallel programing
- HOMEWORK(basic excercise)

---

# HOMEWORK(basic excercise)

---

# Goals of the basic excercise

- usage of the Linux system and MPI/OpenMP
  - treat files and directories on the Linux system
  - edit and display text files
- C/C++ programing
  - output datas to terminal
  - read and write binary files
  - allocate and release dynamic memories
  - compile and run
  - write Makefile
- parallel processing
  - MPI/OpenMP
  - prepare for application exercises

---

# 宿題-基礎演習(Homework; Basic excercise)

- 以下を満たすプログラムを作成しなさい:
  Create a program that satisfies the following:

  - バイナリファイルで与えられた行列 A, B の積 C を計算する。
    The program calculates the product, C, of the matrices A and B given as the binary file.
  - 行列 C を指定されたフォーマットでファイルに出力する。
    The program output the matrix, C, to a binary file in the specified format.

- 最新情報・ヒントは wiki を参照すること
  See the wiki for the last information and hints.
  - <https://gitlab.com/ut-sdpss/2023/lecture/-/wikis/基礎演習課題>
  - <https://gitlab.com/ut-sdpss/2023/lecture/-/wikis/BasicExercise>

---

# 注意事項(Notes)

- 行列の次元はファイルに記録されているのでハードコーディングしないこと
  Since the dimension of the matrix is recorded in the file, should not be hard-coded.
- 倍精度(FP64)で計算・出力すること
  Use double precision (FP64).
- MPI および OpenMP で並列計算すること
  Use parallel computing by using MPI and OpenMP
  - BLAS などの行列演算ライブラリを使用しないこと
    NOT use linear algebra packages such as the BLAS.
    - テストに使用することは可
    - サンプルは用意してあります
- dead line: 2023/5/23 (wiki を参照; see the wiki pages)
  - スケーラビリティのテスト(excel ファイル)も添付のこと

---

# Spec of matrix file

- 先頭から 32bit 符号付き整数(int)で行数、列数が順に格納される
  The number of rows and columns are sequentially stored with a 32-bit signed integer (int) from the top
- その後、行列の値が倍精度浮動小数点型(double)で値が格納される
  After that, the matrix elements are stored in double precision floating point type - 行優先(row-oriented) - eg.) (0, 0), (1, 0), (2, 0), … (N-1, 0), (1, 0), …, (N-1, N-1)

# Remark

- サンプルコードは仕様に則っていません。
  Sample code does not follow the specification.

---

# Outline of the High Performance Computing (HPC)

---

# super computer

- 最新技術が搭載された最高性能のコンピュータ
  The highest performance computer equipped with the latest technology
  - 高性能計算: High Performance Computing
  - 基本構成(CPU, memory, disk, OS etc.)は PC と同じ
  - 高価: expensive
  - 最近の流行は分散並列型(distributed memory machine)

---

# Wisteria system @UT

- <https://www.cc.u-tokyo.ac.jp/supercomputer/wisteria/service/index.php>

![width:15cm drop-shadow center](./img/wisteria_intro.png)

---

# Top500 (<http://top500.org/>) (1/2)

    R_peak: 理論性能値(theorotical maximum performance; calculated)
    R_max: 実効性能値(determine by HPL benchmark)

![width:17cm drop-shadow center](./img/TOP500_202211_1.png)

---

# Top500 (2/2)

![width:22cm drop-shadow center](./img/TOP500_202211_2.png)

---

# Linux がスパコン TOP500 で OS シェア 100％に

- <https://www.zdnet.com/article/linux-totally-dominates-supercomputers/>
- <https://japan.zdnet.com/article/35110755/>

![bg right:60%](./TOP500_201711_linux.png)

---

# HPC programing

ハードウェアの性能を十分発揮させるために

---

# HPC performance

## FLOPS

- Floating Point Operations Per Second
- 1 秒間に浮動小数点演算(Floating Point Operations)が何回実行できるか
  - (theoritical) FLOPS = クロック周波数(clocks) x コア数(cores) x クロックあたりの浮動小数点演算数(FLOPS/clocks=op)
  - クロック周波数: 1 秒あたりの処理回数
  - 例えば iMac (Intel Core i5 2.8 GHz Quad-core)
    - 2.8 GHz x 4 core x 16 op = 179.2 GFLOPS

---

# 浮動小数点数(Floating Point)

- Numeric expression in computer
  - IEEE 754
- 種類

<style scoped>
  table { table-layout: fixed; width: 100%; display:table; font-size: 20px; }
</style>

|                  | 情報量 (bit)     | 備考                         |
| :--------------- | :--------------- | :--------------------------- |
| 単精度 (FP32)    | 32 (= 4 octet)   | Single Precision; SP; float  |
| 倍精度 (FP64)    | 64 (= 8 octet)   | Double Precision; DP; double |
| 4 倍精度 (FP128) | 128 (= 16 octet) | Quad Presicion               |
| 半精度 (FP16)    | 16 (= 2 octet)   | half                         |
| FP8              | 8 (= 1 octet)    |                              |

---

# 様々な CPU のクロックあたりの浮動小数点演算数

    SSE: ストリーミングSIMD拡張命令(Streaming SIMD Extensions)
    SIMD: single instruction multiple data
    FMA: 積和演算(fused multiply-add)

<style scoped>
  table { table-layout: fixed; width: 100%; display:table; font-size: 20px; }
</style>

| CPU                   |                   | 備考                |
| :-------------------- | ----------------: | ------------------- |
| Intel Core2, ~Nehalem |  4 DP FLOPS/Clock | SSE2(add)+SSE2(mul) |
| Intel Sandy Bridge~   |  8 DP FLOPS/Clock | AVX                 |
| Intel Haswell~        | 16 DP FLOPS/Clock | AVX2                |
| AMD Ryzen             |  8 DP FLOPs/Cycle | 4-wide FMA          |
| Intel Xeon SkylakeSP~ | 32 DP FLOPs/Clock | AVX512              |

---

# CPU の浮動小数点演算能力

<style scoped>
  table { table-layout: fixed; width: 100%; display:table; font-size: 20px; }
</style>

| 名称                  |            | 備考                        |
| :-------------------- | ---------: | --------------------------- |
| Pentium (300 MHz)     | 300 MFLOPS | 1993; 1 F/C × 300MHz        |
| Pentium III (1.4 GHz) | 2.1 GFLOPS | 1999; 1.5 F/C × 1.4 GHz     |
| Pentium 4 (3.8 GHz)   | 7.6 GFLOPS | 2000; 2 F/C × 3.8 GHz       |
| Core2Duo              |  27 GFLOPS | 2006; 1.5 F/C ×2.33 GHz ×2  |
| Core i7(Sandy Bridge) | 158 GFLOPS | 2011; 8 F/C ×3.3 GHz ×6     |
| Core i7(Haswell)      | 384 GFLOPS | 2013; 16 F/C × 3.0 GHz × 8  |
| Core i7(Broadwell)    | 480 GFLOPS | 2014; 16 F/C × 3.0 GHz × 10 |
| Apple M1              | 2.6 TFLOPS | 2020                        |

- <https://dench.flatlib.jp/opengl/cpuflops>

---

# GPU の浮動小数点演算能力

<style scoped>
  table { table-layout: fixed; width: 100%; display:table; font-size: 20px; }
</style>

| 名称                    |                        | 備考 |
| :---------------------- | :--------------------- | ---- |
| NVIDIA GeForce RTX 3090 | SP(FP32): 35.58 TFLOPS |      |
|                         | DP(FP64): 556.0 GFLOPS | 1:16 |
| NVIDIA A100             | SP(FP32): 19.49 TFLOPS |      |
|                         | DP(FP64): 9.746 TFLOPS | 1:2  |
| AMD Radeon RX 6600      | SP(FP32): 8.928 TFLOPS |      |
|                         | DP(FP64): 558.0 GFLOPS | 1:16 |

- <https://www.techpowerup.com/gpu-specs/>
- <https://dench.flatlib.jp/opengl/gpuflops>

---

# 様々なハードの浮動小数点演算能力

| 名称             |              | 備考    |
| :--------------- | -----------: | ------- |
| Apple A8         |   115 GFLOPS | iPhone6 |
| PS4              |  1.84 TFLOPS |         |
| PS5              | 10.28 TFLOPS |         |
| 地球シミュレータ | 35.86 TFLOPS | 初代    |
| 京               | 10.51 PFLOPS |         |
| Summit           | 143.5 PFLOPS |         |
| 富岳             |   415 PFLOPS |         |

---

# Memory Bandwidth

- 単位時間あたりに転送できるデータ量
  - 理論バンド幅 = DRAM クロック周波数(clock) x 1 クロックあたりのデータ転送回数(cycle) x メモリバンド幅 (bandwidth: 8 byte) x CPU メモリチャンネル数 (channels)
    - DDR3-1600:
      (DRAM クロック周波数 x 1 クロックあたりのデータ転送回数) = 1600
    - iMac (Intel Core i5-5575R, DDR3)
      - 1867 MHz x 8 x 2 = 29872 MB/s = 29.9 GB/s
    - Reedbush 1node (DDR4-2400) 153.6 GB/s
    - Oakbridge-CX 1node 281.6 GB/s
      - 計算ノード間: 100 Gbps = (100/8) GB/s = 12.5 GB/s
- 単純な計算を大量に行う場合は、メモリバンド幅が性能を決める
  When performing simple calculations in large quantities, the memory bandwidth determines the performance.

---

# Byte per FLOPS (通称 B/F 値)

- 1 回の浮動小数点演算の間にアクセスできるデータ量
  - SR16000: 512 GB/s / 980.48 GFLOPS = 0.52
  - FX10: 85 GB/s / 236.5 GFLOPS = 0.36
  - Reedbush: 153.6 GB/s / (2.1x16x36) GFLOPS = 0.127
  - Oakbridge-CX: 281.6 GB/s / 4.8384 (=2.7 _ 56 _ 32) TFLOPS = 0.05820
- 参考
  - 倍精度実数(double)は 8 octet(byte):  
    3 度の読み書き(e.g. c=a\*b)で 8 x 3 = 24 octet(byte) - B/F 値 24 以上必要 - FX10: 0.36 / 24 = 0.015 (98.5% CPU は遊んでる) - Reedbush: 0.127 / 24 = 0.0053 (99.5% CPU は遊んでる!) - Oakbridge-CX: 0.05820 / 24 = 0.002425 (99.76% CPU は遊んでる!)
- CPU さえ速ければ、コア数さえ多ければ、単純に速いわけではない！

---

# ルーフラインモデル

- メモリバンド幅(データ転送量)によって演算性能に上限
- 演算性能[FLOPS] = min(理論演算性能[FLOPS],
  プログラム演算強度[FLOPS/Byte] × メモリバンド幅[Byte/sec])

![auto center](./LoofLineModel.png)

---

# 階層メモリ構造(Hierarchical memory structure)

| 名称                       | 記憶容量 | アクセス速度(遅延) | 転送速度(帯域) |
| :------------------------- | -------: | -----------------: | -------------: |
| レジスタ register (on CPU) |     byte |                 ns |           GB/s |
| キャッシュ cache (on CPU)  |  kB ~ MB |              10 ns |           GB/s |
| (メイン)メモリ memory      |  MG ~ GB |             100 ns |        3~ GB/s |
| ハードディスク HDD         |  GB ~ TB |              10 ms |       100 MB/s |

cf.) <https://colin-scott.github.io/personal_website/research/interactive_latency.html>

- キャッシュを効率的に使わないと遅い
  It is slow if you do not use cash efficiently

---

# メモリ上のデータ格納構造: data structure in memory

- データはまとまって取り扱われる(=cache line)
  - 連続したデータは近く(キャッシュ内)に存在する確率が高い
    - cache hit
  - 不連続データアクセスは cache miss を引き起こしやすい
- (C/C++言語)One dimensional array is continuous data
  - うまく活用することで高速化が期待できる

---

# memory access in matrix-matrix multiplication

![80% center](./行列メモリアクセス.png)

---

# Performance Tuning (Single Process)

- CPU へ如何にうまくデータを送り込ませるかがポイント
- 転送量(transfer) < 演算量(computing) の場合
  - データを使いまわして高速化 → ブロック化
  - eg.) matrix-matrix multiplication
    - data: $N^2$
    - computation $N^3$
- 転送量(transfer) > 演算量(computing) の場合
  - 高速化は難しい
    - 余計に計算する(メモリ転送量を減らす)ことも一考
  - eg.) matrix-vector multiplication
  - eg.) Householder triple diagonalization
    - 行列-ベクトル積が必要 → 帯行列にする

---

# parallel computing

---

# なぜ並列化が必要なのか

- “The Free Lunch Is Over”
  - <http://www.gotw.ca/publications/concurrency-ddj.htm>

![width:12cm](./CPU.png)

---

# The Free Lunch Is Over

1. クロックが上がるとソフトウェアのパフォーマンスも勝手に向上
   Improve software performance arbitrarily as clock rises
1. クロック上昇の限界
   Limit of clock rise
1. CPU を複数使用するしかない
   Only have to use multiple CPUs
1. 並列処理のプログラムを書かねばパフォーマンスが上がらず
   Performance does not rise unless you write a parallel processing program!

---

![auto center](./img/pokemon-cpu.jpg)

---

![auto center](./img/dospara-cpu.jpg)

---

# 並列化プログラミングの心構え

- 本当に並列化が必要か Is it really necessary to parallelize?
  - まずは単体動作でのチューニングをすべき
    First we should tune on standalone operation
- どこを並列化すべきか Where should we parallelize?
  - Pareto principle (80:20 の法則)
  - プロファイラ等を使い、どの関数・ループが処理に時間がかかるかを見つける
    Using a profiler, find out which function or loop takes time to process
  - 思い込みは禁物 Never imagined
- 並列化したらなんでも速くなると思ったら大間違い
  Will your program become faster if you parallelize? No, it's a big mistake!

---

# (並列)性能評価指標 (1/4)

## 台数効果 Number Effect (高速化率 Acceleration rate)

$$
S_P = \frac{T_S}{T_P}
$$

- $T_S$ : 1 台(serial)での実行時間
- $T_P$ : 複数台($P$台; parallel)での実行時間
- どれだけ早く計算できるようになったかを示す指標
  - $S_P = P$ が理想的(多くは $S_P < P$)
  - $S_P>P$ は super linear speedup とよばれる
  - キャッシュヒットなどによって高速化されたケースなど

---

# (並列)性能評価指標 (2/4)

## 並列化効率 Parallelization efficiency

$$
E_P = \frac{S_P}{P} \times 100
$$

- 並列化がどれだけ上手に行われているかを示す指標

---

# (並列)性能評価指標 (3/4)

## アムダールの法則 Amdahl's law

- 1 台での実行時間$T_S$のうち、並列化ができる割合(並列化率)を$a$とすると、
  $P$台での並列実行時間$T_P$は

  $$
  T_P = \frac{T_S}{P} \cdot a + T_S (1-a)
  $$

- したがって台数効果は

  $$
  S_P = \frac{T_S}{T_P} = \frac{1}{(a / P +(1- a))}
  $$

- 無限台使っても(P→∞), 台数効果は$1/(1-a)$しか出ない

---

# (並列)性能評価指標 (4/4)

## アムダールの法則のポイント

- 全体の 90%を並列化しても、1/(1-0.9)=10 倍で飽和する

![center](./Amdahl'sLaw.png)

- 約 100 万並列で性能を出す(並列化効率 90%以上)には並列化率はいくら必要か？

---

# 並列化は万能か&quest;&excl;

- Processing that becomes faster by parallelization / that does not get faster
- "並列化出来る処理"と"頑張っても並列化できない処理"とがある

![auto center](./Amdahl_point.png)

---

# スケーラビリティ(並列性能向上)の評価

## Strong Scaling

- 問題規模は一定
- プロセス数を増加
- 並列数が多くなると達成は困難
  - cf. アムダールの法則

## Weak Scaling

- 1 プロセスあたりの問題規模を一定
- プロセス数を増加

![bg right:30% width:100%](./scalability.png)

---

# 基礎演習での Excel シートの使い方

How to use the excel sheets in the basic excercise

- 水色のセルに数値を代入すれば自動的に計算されるようになっていますが、必要に応じて変更してもかまいません。
  The calculation is done automatically by assigning values to the light blue cells, but you may change this if necessary.
- シートは2枚(Strong scaling, Weak Scaling)あるので、忘れないようにしてください。
  There are two sheets (Strong Scaling, Weak Scaling), so please do not forget them.

---

# Process and Threads

- Process
  - OS から独立したリソース(CPU, メモリ空間 等)を割り当てられる
    The independent computer resource is assigned by the OS
  - 1 つ以上のスレッドを持つ
    Process has one and more threads
- Thread
  - 実行単位; execution unit
  - 各スレッドはプロセス内メモリを共有する
    Each thread shares the process memory
- see
  - Task Manager (@Windows)
  - Activity Monitor(@MacOS), top/htop (@Mac, UNIX)

---

# 並列プログラミングの方法 1- Method of parallel programming

## Multi-process

- プロセス間でデータのやりとりをする仕組み
  Data is exchanged between processes
- プロセス間でメモリ空間は(基本的には)共有できない
  Memory space can not be shared between processes
- 別の計算機上にあるプロセスとも通信できる
  It can also communicate with processes on another computer
- eg.) MPI(Message Passing Interface)

---

# 並列プログラミングの方法 2 - Method of parallel programming

## Multi-threads

- プロセス内部で複数スレッドが並列動作
  Multiple threads operate in parallel within the process
- プロセスのメモリ空間を複数スレッドで共有できる
  Memory space can be shared between threads in the process
- 排他処理が必要
  Exclusive processing required
- 同一システム上でしか動作しない
  It works only on the same system
- eg.) pthread(POSIX thread), OpenMP

---

# MPI の特徴 Features of MPI

- ライブラリ規格の一つ One of the library standards
  - プログラミング言語、コンパイラに依存しない
    It is independent of programming language and compiler
  - API(Application Programing Interface)を標準化
  - 実装がまちまち
- 大規模計算が可能 Large scale calculation is possible
  - 高速ネットワークを介したプロセス間通信が可能
    Enable interprocess communication via high-speed network
- プログラミングの自由度が高い Free parallel comunication programing
  - 通信処理を自由にプログラミングすることで最適化が可能
    Optimize by freely programming communication processing
  - 裏を返せばプログラミングが大変

---

# MPI の実装 Implementation

- MPICH
  - Argonne National Laboratory で開発
  - MPICH1, MPICH2 など
- OpenMPI
  - Open-source
  - 最近の Linux ディストリビューションで採用されつつある
- MPI presented by the vender
  - optimized MPI by the vender
  - MPICH2 がベースが多い

---

# MPI プログラミングの作法 Rules

- Initialize
  - 使う資源(リソース)を確保・準備する
  - All processes need to call
  - MPI_Init()関数
- Finalize
  - 使った資源(リソース)を返す
  - 返さないとゾンビ(ずっと居残るプロセス)になる場合も
  - All processes need to call
  - MPI_Finalize()関数

---

# MPI 関数の性質 Characters

## 通信 Communication

- 集団通信 Collective communication
  - 全プロセスが通信に参加 (全プロセスが呼ばなければ止まる)
- 1 対 1 通信
  - 通信に関与するプロセスのみが関数を呼ぶ

## Blocking / non-Blocking

- Blocking
  - 通信が完了するまで次の処理を待つ
    Wait for next processing until communication is completed
- non-Blocking
  - 通信しながら別の処理が可能
    Different processing is possible while communicating

---

# 主な MPI 関数 (初期化)

## MPI_Init

```c++
#include <mpi.h>
int MPI_Init(int *argc, char **argv);
```

- MPI 環境を起動・初期化する

```text
    argc: コマンドライン引数の総数
    argv: 引数の文字列を指すポインタ配列
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (後始末)

## MPI_Finalize

```c++
#include <mpi.h>
int MPI_Finalize();
```

- MPI 環境の終了処理を行う

---

# 主な MPI 関数 (ユーティリティ: 1/2)

## MPI_Comm_size

```c++
#include <mpi.h>
int MPI_Comm_size(MPI_Comm comm, int *size);
```

- コミュニケータに含まれる全プロセスの数を返す
- コミュニケータには全 MPI プロセスを表す定義済みコミュニケータ`MPI_COMM_WORLD`が使用できる

```text
    comm: (in) コミュニケータ
    size: (out) プロセスの総数
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (ユーティリティ: 2/2)

## MPI_Comm_rank

```c++
#include <mpi.h>
int MPI_Comm_rank(MPI_Comm comm, int *rank);
```

- コミュニケータ内の自身のプロセスランクを返す
  - ランクは 0 から始まる

```text
    comm: (in) コミュニケータ
    rank: (out) ランク
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (全体通信: 1/2)

## MPI_Bcast

```c++
#include <mpi.h>
int MPI_Bcast(void* buf, int count, MPI_Datatype datatype,
int root, MPI_Comm comm);
```

- root から comm の全プロセスに対して broadcast する

```text
    buf: (in)  送信バッファのアドレス
    count: (in) 送信する数
    datatype: (in) データ型
    root: (in) 送信元ランク
    comm: (in) コミュニケータ
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (全体通信: 2/2)

## MPI_Allreduce

```c++
#include <mpi.h>

int MPI_Allreduce(void* sendbuf, void* recvbuf, int count,
MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
```

- 集計した後、結果を全プロセスへ送信する

```text
    sendbuf: (in) 送信バッファのアドレス
    recvbuf: (out) 受信バッファのアドレス
    count: (in) 送信する数
    datatype: (in) データ型
    MPI_Op: (in) 演算オペレータ
    comm: (in) コミュニケータ
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (単体通信: 1/4)

## MPI_Send

```c++
#include <mpi.h>

int MPI_Send(void* buf, int count, MPI_Datatype datatype,
int dest, int tag, MPI_Comm comm);
```

- dest プロセスへデータを送る

```text
    buf: (in) 送信バッファのアドレス
    count: (in) 送信する数
    datatype: (in) データ型
    dest: (in) 送信先ランク
    tag: (in) タグ
    comm: (in) コミュニケータ
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (単体通信: 2/4)

## MPI_Recv

```c++
#include <mpi.h>

int MPI_Recv(void* buf, int count, MPI_Datatype datatype,
int source, int tag, MPI_Comm comm, MPI_Status* status);
```

- source プロセスからのデータを受け取る

```text
    buf: (out) 受信バッファのアドレス
    count: (in) 受信する数
    datatype: (in) データ型
    source: (in) 送信元ランク
    tag: (in) タグ, comm: (in) コミュニケータ
    status: (out) ステータス情報
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (単体通信: 3/4)

## MPI_Isend

```c++
#include <mpi.h>

int MPI_Isend(void* buf, int count, MPI_Datatype datatype,
int dest, int tag, MPI_Comm comm, MPI_Request* request);
```

- dest プロセスへデータを送る

```text
    buf: (in) 送信バッファのアドレス
    count: (in) 送信する数
    datatype: (in) データ型
    dest: (in) 送信先ランク
    tag: (in) タグ, comm: (in) コミュニケータ
    request: (out) リクエストハンドル
```

- 戻り値: MPI_Success(正常)

---

# 主な MPI 関数 (単体通信: 4/4)

## MPI_Irecv

```c++
#include <mpi.h>

int MPI_Recv(void* buf, int count, MPI_Datatype datatype,
int source, int tag, MPI_Comm comm, MPI_Request* request);
```

- source プロセスからのデータを受け取る

```text
    buf: (out) 受信バッファのアドレス
    count: (in) 受信する数
    datatype: (in) データ型
    source: (in) 送信元ランク
    tag: (in) タグ
    comm: (in) コミュニケータ
    request: (out) リクエストハンドル
```

---

# 主な MPI 関数 (通信その他)

## MPI_Barrier

```c++
#include <mpi.h>

int MPI_Barrier(MPI_Comm comm);
```

- 同期をとる

```text
comm: (in) コミュニケータ
```

---

# 主な MPI 関数 (通信その他)

## MPI_Wait

```c++
#include <mpi.h>

int MPI_Wait(MPI_Request* request, MPI_Status* status);
```

- 変数の通信待ち処理を行う

```text
    request: (in) リクエストハンドル
    status: (out) 受信状態
```

---

# 主な MPI 関数 (通信その他)

## MPI_Wtime

```c++
#include <mpi.h>

double MPI_Wtime();
```

- ある時刻からの秒数を返す
- とある処理の両端で計測し、その差分を計算すると経過時間がわかる

---

# MPI データ型

| C/C++ data type | MPI data type | C/C++ data type | MPI data type     |
| :-------------- | :------------ | :-------------- | :---------------- |
| char            | MPI_CHAR      | unsigned char   | MPI_UNSIGNED_CHAR |
| int             | MPI_INT       | unsigned int    | MPI_UNSIGNED_INT  |
| long            | MPI_LONG      | unsigned long   | MPI_UNSIGNED_LONG |
| float           | MPI_FLOAT     | double          | MPI_DOUBLE        |

---

# MPI サンプルコード(1/2)

```c++
#include <iostream>
#include <unistd.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
```

---

# MPI サンプルコード(2/2)

```c++
    char hostname[256];
    for (int i = 0; i < size; ++i) {
        if (i == rank) {
            gethostname(hostname, sizeof(hostname));
            std::cout << "rank=" << i << ", hostname=" << hostname << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
```

---

# MPI プログラミングのコツ

- コンパイラは専用のもの(mpicxx, mpifort など)を使う
  - コンパイル・ビルドに必要なライブラリやインクルードパスを自動的に設定してくれる
- 実行は実装によって異なる
  - mpirun? mpiexec?
  - 実装毎に環境変数も変わる
- 基本的にデバッグは難しい
  - 逐次(シリアル)版でバグは潰しておく
  - デバッガに頼らず、何かに出力するようにした方が無難
  - gdb オプションも時には使える
- プロファイル
  - gprof なら GMON_OUT_PREFIX 環境変数を使うと良い

---

# comments for MPI

- MPI もソフトウェア
  - バグは少なからずある
  - なるべく実績のある(よく使われる)API を使う
- MPI-1 を使った方が良い(場合がある)
  - MPI-2 以上は多機能な反面、システムによって挙動が異なる場合がある
  - MPI-1 で(やりたいことは)基本的に実現可能
    - 可変長配列を送るときは、はじめに配列数を転送するなど工夫する。
- 非同期通信が必ずしも良いとは限らない
  - デバッグ作業は格段に難しくなる
  - MPI_Test(), MPI_Wait()が呼ばれて初めて通信を開始する実装がある

---

# MPI Appendix

## 派生データ型

- 構造体を通信したいときに、オリジナルのデータ型を作成できる
  - MPI_Type_create_struct (MPI_Type_struct)
  - MPI_Type_Commit

## MPI I/O

- 巨大なファイルを各プロセスが同時に読み書きする仕組み
  - MPI_File_open, MPI_File_close
  - MPI_File_set_view
  - MPI_File_write_all

---

# OpenMP

---

# OpenMP の特徴

- C/C++および Fortran プログラミング言語をサポートする API(Application Program Interface)
  - 指示文(pragma なので非対応コンパイラでも問題なし)
  - 専用のライブラリをリンク
  - 環境変数で動作を制御
- 共有メモリ型並列計算機上で動作する
- 並列処理する箇所を明示する必要がある
  - 自動並列化ではない
- データ分割を指示しなくても良い
  - プログラミングが楽
  - 裏を返せば、処理がブラックボックス化
- 最近は GPU コードも吐けるように

---

# OpenMP の書き方(C/C++)

- 並列実行

```c++
#pragma omp parallel
{
…
}
```

- 並列実行(for ループ)

```c++
#pragma omp parallel for
for (int i = 0; i < 10; ++i) {
…
#pragma omp critical (name) ← クリティカルリージョン
    {
      …
    }
}
```

---

# OpenMP サンプル(1/2)

```c++
#include <iostream>
#include <omp.h>

int main()
{
    std::cout << "# of procs: " << omp_get_num_procs() << std::endl;
    std::cout << "max threads: " << omp_get_max_threads() << std::endl;

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        std::cout << "thread #: " << id << std::endl;
    }
```

---

# OpenMP サンプル(2/2)

```c++
    int sum = 0;
#pragma omp parallel for
    for (int i = 0; i < 10000; ++i) {

#pragma omp atomic
        sum += i;
    }

    std::cout << "sum=" << sum << std::endl;

    return 0;
}

```

---

# 代表的な OpenMP pragma (1)

## ブロックを並列化

```c++
#pragma omp parallel
{
...
}
```

---

# 代表的な OpenMP pragma (2)

## for ループを並列化 (for を分割処理)

```c++
#pragma omp parallel
{
#pragma omp for
    for (int i = 0; i < 100; ++i) {
    ...
    }
}
```

## for ループを並列化 (parallel と一緒に指定)

```c++
#pragma omp parallel for
for (int i = 0; i < 100; ++i) {
...
}
```

---

# 代表的な OpenMP pragma (3)

## section を並行に実行

```c++
#pragma omp parallel sections
{
#pragma omp section
    {
    ...
    }

#pragma omp section
    {
    ...
    }
}
```

---

# 代表的な OpenMP pragma (4)

## 1 つのスレッドだけが実行

```c++
#pragma omp parallel
{
#pragma omp single
    {
    ...
    }
}
```

---

# 代表的な OpenMP pragma (5)

## 直後のブロックを排他的に処理

```c++
#pragma omp parallel
{
#pragma omp critical
    {
    ...
    }
}
```

---

# 代表的な OpenMP pragma (6)

## スレッドの同期を取る

```c++
#pragma omp parallel
{
#pragma omp barrier

}
```

---

# 代表的な OpenMP pragma (7)

## 共有変数のメモリの一貫性を保つ

```c++
#pragma omp parallel
{
#pragma omp flush
}
```

---

## OpenMP ライブラリ関数

- don't forget include <omp.h>

```c++
#include <omp.h>
```

| functions             | processing                       |
| :-------------------- | :------------------------------- |
| omp_get_num_procs()   | プロセッサの数を返す             |
| omp_get_max_threads() | 実行可能なスレッドの最大数を取得 |
| omp_get_num_threads() | 実行しているスレッド数を取得     |
| omp_get_thread_num()  | 実行しているスレッド番号を取得   |
| omp_get_wtime()       | ある時刻からの秒数を取得         |

---

# OpenMP 注意点

ビルド時は多くの場合コンパイルオプションが必要

- gnu compiler

```bash
gcc -fopenmp
```

- intel compiler

```bash
icpc –openmp
```

- 共有変数か private 変数かを意識すること
  - `#omp parallel` 文の前にある変数は共有変数
- 環境変数に注意
  - `OMP_NUM_THREADS`: 並列スレッド数を設定する
  - `OMP_SCHEDULE`: 並列動作を指定
- コンパイラによって実装・挙動が異なる場合がある

---

# MPI/OpenMP ハイブリッド並列

## Flat MPI

- ノード間は MPI ノード内も MPI
- ノード内のメモリが共有できない(プロセスあたりのメモリ量が少ない)
- MPI のコードだけを書けばよい

## Hybrid

- ノード間は MPI / ノード内は OpenMP
- ノード内メモリをプロセスが占有できる
- 2 種類の並列コードを書かないといけない

---

# 最近の話題(Recent Topics)

## CPU affinity

- OMP_PLACES
- OMP_PROC_BIND
- GOMP_CPU_AFFINITY
- KMP_AFFINITY
- mpiexec --report-bindings

---

# 参考文献

## MPI

- 青山幸也 著, MPI 虎の巻, <https://www.hpci-office.jp/events/seminars/seminar_texts>
- 片桐孝洋 著, スパコンプログラミング入門: 並列処理と MPI の学習 ISBN-13: 978-4130624534

## OpenMP

- OpenMP 入門 <http://www.isus.jp/article/openmp-special/getting-started-with-openmp/>
- 北山 洋幸 著, OpenMP 入門―マルチコア CPU 時代の並列プログラミング ISBN-13: 978-4798023434
- <https://www.openmp.org/wp-content/uploads/OpenMPRef-5.2-1121-JA.pdf>

---

# 演習環境の構築 Setup your working environment

---

# 概要

- (log in the ECCS machine (iMac@電子計算機室))
- open a terminal
- login to the super-computer system by using ssh

2 つのアカウント (ECCS とスパコン) の違いに注意! (iMac@電子計算機室)

---

# ssh 接続の仕組み

- 暗号化の必要性
  - インターネットにおけるデータの盗聴・なりすましの危険
- 公開鍵方式
  - 秘密鍵で暗号化したデータ → 公開鍵でしか復号できない
  - 公開鍵で暗号化したデータ → 秘密鍵でしか復号できない

![width:15cm](./ssh-connection.png)

- <https://qiita.com/tag1216/items/5d06bad7468f731f590e>

---

# ssh 予備知識 1 (advance knowledge part 1)

## file format

- 秘密鍵・公開鍵ともにテキストファイル
  Both private and public keys are in text file format.
  - <https://qiita.com/ponsuke0531/items/7b34347213660c80bc95>
- public key in OpenSSH format

```text
ssh-rsa AAAA...
```

- private key in OpenSSH format

```text
-----BEGIN RSA PRIVATE KEY-----
...
-----END RSA PRIVATE KEY-----
```

---

# ssh 予備知識 2 (advance knowledge part 2)

## default file location

Be careful to overwrite files!

- UNIX(MacOS)/Linux
  - ~/.ssh/id_rsa -> private key
  - ~/.ssh/id_rsa.pub -> public key
  - ~/.ssh/config -> config file
- Windows
  The location of the files depends on the application.

---

# ssh 予備知識 3 (advance knowledge part 3)

## file location of the public key in remote

`~/.ssh/authorized_keys` に置く

- 1 行につき 1 ホスト
- 間違えると認証できない(**接続できなくなる**)ので注意。

- 置くのは **公開鍵(public key)**
  - 管理者(知らない人)に見られても大丈夫

---

# ssh 補足 (supplemental information)

# ssh config file

ホスト毎の鍵の管理や多段接続、ポート転送が簡単に使えるようになる。

- <https://qiita.com/passol78/items/2ad123e39efeb1a5286b>

---

# making ssh-key (MacOS, Linux etc.)

- open a terminal
- run ssh-keygen

```bash
ssh-keygen -t rsa
```

- created files
  - `$HOME/.ssh/id_rsa`
    - the secret key
    - 誰にも見せないこと DONOT show to anyone
    - メール等で送らないこと DONOT e-maile
  - `$HOME/.ssh/id_rsa.pub`
    - public key (見られても OK)

---

# ターミナルソフトの準備

## Windows

- putty: <https://ice.hotmint.com/putty/index.html>
- RLogin: <http://nanno.dip.jp/softlib/man/rlogin/>
- TeraTerm: <http://ttssh2.osdn.jp>
- WSL2 (Windows Subsystem for Linux) + Windows Terminal
  - <https://dev.classmethod.jp/articles/linux-beginner-wsl2-windowsterminal-setup/>

## MacOS

- OS 添付のターミナル(Application/Utility/Termina.app)
- iTerm2: <https://www.iterm2.com>

---

# making ssh-key (Windows)

- cf.) <https://www.xlsoft.com/jp/blog/blog/2019/07/25/post-6946/>
- ターミナルソフトとして putty や RLogin などを用意
  - putty: <https://ice.hotmint.com/putty/index.html>
  - RLogin: <http://nanno.dip.jp/softlib/man/rlogin/>
- putty の場合
  - puttygen を利用 (<https://ja.osdn.net/projects/winscp/wiki/ui_puttygen>)

---

# making ssh-key (Windows; putty)

## ダウンロード

zip ファイルを入手します。

- 本家 [https://www.chiark.greenend.org.uk/~sgtatham/putty/]
- 日本語版おすすめ [https://ice.hotmint.com/putty/index.html]

## インストール

zip ファイルを展開して、任意の場所にコピー(または移動)します。.exe ファイルが実行ファイルです。

---

# making ssh-key (Windows; putty での ssh 鍵作成 1)

## puttygen.exe を実行します

- Windows10 のバージョンやセキュリティソフトによってはセキュリティダイアログが出る場合があります。

![](./img/puttygen1.png)

---

# making ssh-key (Windows; putty での ssh 鍵作成 2)

## [生成]をクリックします

ダイアログ上でマウスをランダムに動かします。

![](./img/puttygen2.png)

---

# making ssh-key (Windows; putty での ssh 鍵作成 3)

## 鍵が作成されます

- ssh の公開鍵が選択されます。

![](./img/puttygen3.png)

---

# making ssh-key (Windows; putty での ssh 鍵作成 4)

## パスフレーズを入力します

- パスワードとは違います。好きに入力してください(覚えておく必要はあります)。

![](./img/puttygen4.png)

---

# making ssh-key (Windows; putty での ssh 鍵作成 5)

## 鍵を保存します

### [公開鍵の保存]をクリックして、公開鍵を好きな名前(例えば id_rsa.pub)で保存します

### [秘密鍵の保存] をクリックして、秘密鍵を好きな名前(例えば id_rsa)で保存します

- 秘密鍵を保存しておけば、後でペアとなる公開鍵を再作成することもできます。

**_ 注意！ _**
putty の秘密鍵は特殊フォーマット(.ppk)で保存されます。そのままでは作成した秘密鍵を Linux や MacOS にコピーしても使えません。puttygen のメニュー[変換]→[OpenSSH 形式へエクスポート(最新)]で保存したものを利用してください。

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 1)

## ダウンロード

zip ファイルを入手します。

- RLogin: <http://nanno.dip.jp/softlib/man/rlogin/>

## インストール

zip ファイルを展開して、任意の場所にコピー(または移動)します。.exe ファイルが実行ファイルです。

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 2)

- [ファイル] -> [サーバーに接続]
  "Server Select"ダイアログが開く

- [新規] をクリック
  "Server New Entry"ダイアログが開く

![width:12cm](./img/RLogin1.png)

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 3)

- 左側のツリーから"サーバー/プロトコル"を選択する

![width:12cm](./img/RLogin2.png)

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 4)

- SSH グループから"認証キー"をクリックする
  "認証キー"ダイアログが開く

![width:12cm](./img/RLogin3.png)

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 5)

- 新規グループで(任意の名前を入力後)作成ボタンをクリックする
  SSH 鍵の作成ダイアログが開く

![width:12cm](./img/RLogin4.png)

- パスフレーズを入力して OK をクリックする
  SSH 鍵の作成ダイアログが閉じる

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 6)

- "認証キー"ダイアログの認証キーリストから作成した項目を選択し、公開鍵ボタンをクリックする
  "Public Key"ダイアログが開く。表示される文字列が公開鍵となる。
  全て選択してコピー(Ctrl+C)し、メモ帳などにペースト(Ctrl-V)して利用する。

![width:12cm](./img/RLogin5.png)

- OK を押して"Public Key"ダイアログを閉じる。

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 7)

- ファイルグループのエクスポートボタンをクリックする。
  "SSH 鍵ファイルの保存"ダイアログが開く

![width:12cm](./img/RLogin6.png)

- 任意の保存する場所を選択し、(必要であれば)パスフレーズを入力して OK をクリックする。
  SSH 秘密鍵が保存される。後でインポートすると公開鍵が表示できる。

---

# making ssh-key (Windows; RLogin での ssh 鍵作成 8)

- OK をクリックして"SSH 鍵ファイルの保存"ダイアログを閉じる。
  以降、SSH 秘密鍵は作成できたので適宜ダイアログを閉じて良い。

- 作成した SSH 秘密鍵は
  "Server New Entry"ダイアログ -> "サーバー" -> "ホスト設定" -> "ssh 認証鍵"
  で選択して利用する。

---

# register the public ssh key (Oakbridge) (1/2)

- procedure
  - open your web browser
  - open the following URL
    - 利用支援ポータル (<https://wisteria-www.cc.u-tokyo.ac.jp/>)
  - submit your account (利用者番号) and password
  - change password
    - DO NOT close the browser before you can log-in the portal site by using another browser.
      新しいパスワードでログインできるかどうか確かめる
      - OK なら前のブラウザウィンドウ(タブ)を閉じる
      - NG なら落ち着いて再度挑戦する

---

# register the public ssh key (Oakbridge) (2/2)

- procedure (continued)

  - submit your public ssh key
    左側の [SSH 公開鍵登録] から公開鍵を登録
    - [MacOS] `open ~/.ssh` to open the folder by Finder
    - DO NOT close the browser before you can log-in the server by ssh!
      ssh でのログインが確認できるまでブラウザを閉じないこと

- manual
  - [利用支援ポータル] -> [ドキュメント閲覧] -> [Oakbridge-CX 利用手引書]

---

# login to the super computer

- type in your terminal

```bash
ssh <supercomputer account>@obcx.cc.u-tokyo.ac.jp
```

- パスフレーズが聞かれた場合は、設定したパスフレーズを入れる
  When a passphrase is asked, put the passphrase that you set

---

# transmit files (scp 1/2)

```bash
scp <from> <to>
```

- cp コマンドと同様の使い方 (第４文型: SVOO)
  - -r オプションで(サブ)ディレクトリも一緒に
- How to specify the location (filepath)
  - [[account@]server:]directory.../filename
  - サーバー名を省略した場合はローカルマシンが想定
    If the server name is omitted, the local machine is assumed

---

# transmit files (scp 2/2)

- from local machie to remote

```bash
scp ./sample.c  xxxx@obcx.cc.u-tokyo.ac.jp:somewhere
```

- from remote machine to local

```bash
scp xxxx@obcx.cc.u-tokyo.ac.jp:sample.c ./somewhere
```

---

# transmit files (scp/sftp using GUI)

## Windows

- FileZilla (無償)
- WinSCP (無償)

## MacOS

- FileZilla (無償)
- Transmit (有償)
- Forklift (有償)

---

# How to use the batch system

see users guide in <https://obcx-www.cc.u-tokyo.ac.jp/>

- available queue
  - lecture2 (in the class)
  - lecture (outside of the class)

|                               | command          |
| ----------------------------- | ---------------- |
| ジョブの投入 submit job       | pjsub \<script\> |
| 状況確認 check your jobs      | pjstat           |
| ジョブの削除 delete your jobs | pjdel \<job ID\> |
