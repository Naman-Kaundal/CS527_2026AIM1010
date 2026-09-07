# CS509 - Assignment 01

## Repository Overview

This repository contains the implementation for **Assignment 01** of the CS509 laboratory assignments.

Assignment 01 covers matrix multiplication (GEMM) and graph representation using Compressed Sparse Row (CSR) format.

The implementation is written in C++ and includes separate source files, drivers, test cases, and a common wrapper for compilation and execution.

---

## Student

**Student Name:** Naman Kaundal
**Entry Number:** 2026AIM1010
**Programme:** M.Tech Artificial Intelligence
**Course:** CS509

---

## Language and Environment

* **Programming Language:** C++
* **Standard:** C++17
* **Compiler:** GNU G++
* **Optimization:** `-O2`
* **Warnings:** `-Wall`
* **Operating System:** Windows
* **Shell used for execution:** PowerShell
* **Editor:** Visual Studio Code

Compilation example:

```powershell
g++ -std=c++17 -O2 -Wall source_file.cpp driver_file.cpp -o program.exe
```

---

## Directory Structure

```text
CS509_2026AIM1010/
│
├── assignment_01/
│   ├── 01_GEMM/
│   │   ├── src/
│   │   ├── driver/
│   │   └── tests/
│   │
│   └── 02_CSR_Graph/
│       ├── src/
│       ├── driver/
│       └── tests/
│
├── common_wrapper/
│   └── wrapper.cpp
│
└── README.md
```

---

## Common Wrapper: Build and Usage

The common wrapper provides a single interface for compiling and running the assignments.

It provides the following options:

```text
1. Compile all assignments
2. Run GEMM
3. Run CSR Graph
4. Run Bellman-Ford
5. Run Floyd-Warshall
0. Exit
```

### Compilation

From the repository root:

```powershell
g++ -std=c++17 -O2 -Wall .\common_wrapper\wrapper.cpp -o .\wrapper.exe
```

### Execution

```powershell
.\wrapper.exe
```

Select option `1` to compile all assignment executables.

The wrapper generates:

```text
gemm.exe
csr.exe
bellman_ford.exe
floyd_warshall.exe
```

---

## Assignment Mode

Individual implementation.

## Objective

The assignment implements:

1. General Matrix-Matrix Multiplication (GEMM)
2. A blocked version of GEMM
3. Conversion of an adjacency-list graph representation into CSR format

---

# Assignment 01 - GEMM

## Objective

The GEMM implementation performs matrix multiplication and compares a straightforward implementation with a blocked implementation.

Both implementations compute:

```text
C = A × B
```

for compatible matrices.

## Algorithm / Approach

### Simple GEMM

The conventional triple-loop matrix multiplication approach is used.

For every output element `C[i][j]`, the implementation accumulates:

```text
A[i][k] × B[k][j]
```

over all valid `k`.

### Blocking GEMM

The matrix multiplication is divided into smaller blocks.

Blocking improves cache locality by operating on portions of the matrices that can remain in the processor cache for longer.

---

## Input Format

The GEMM test files contain the matrix dimensions followed by matrix elements according to the assignment specification.

Test files:

```text
gemm_test_01.txt
gemm_test_02.txt
gemm_test_03.txt
gemm_test_04.txt
gemm_test_05.txt
gemm_test_06.txt
```

---

## File Structure

```text
assignment_01/01_GEMM/
├── src/
│   └── gemm.cpp
├── driver/
│   └── driver_gemm.cpp
└── tests/
    ├── gemm_test_01.txt
    ├── gemm_test_02.txt
    ├── gemm_test_03.txt
    ├── gemm_test_04.txt
    ├── gemm_test_05.txt
    └── gemm_test_06.txt
```

---

## Compilation

```powershell
g++ -std=c++17 -O2 -Wall `
.\assignment_01\01_GEMM\src\gemm.cpp `
.\assignment_01\01_GEMM\driver\driver_gemm.cpp `
-o .\gemm.exe
```

## Execution

```powershell
.\gemm.exe "assignment_01/01_GEMM/tests/gemm_test_01.txt"
```

The same command can be used with the other test files.

---

## Test Cases and Result Table

| Test Case          | Dimensions (M×K×N) | Simple GEMM | Blocking GEMM |
| ------------------- | :-----------------: | ----------: | ------------: |
| `gemm_test_01.txt` | 2×3×2               |     0.001 ms |       0.001 ms |
| `gemm_test_02.txt` | 3×3×3               |     0.001 ms |       0.001 ms |
| `gemm_test_03.txt` | 100×150×80          |     0.86 ms  |       0.88 ms  |
| `gemm_test_04.txt` | 200×200×200         |     6.53 ms  |       5.80 ms  |
| `gemm_test_05.txt` | 500×500×500         |   196.35 ms  |     110.74 ms  |
| `gemm_test_06.txt` | 600×300×400         |    91.37 ms  |      61.16 ms  |

The runtimes are machine-dependent and may vary between executions.

---

## Complexity

For matrices of compatible dimensions, conventional GEMM has:

```text
Time Complexity: O(N³)
```

for square `N × N` matrices.

The blocked implementation has the same asymptotic complexity but can provide improved cache utilization.

---

## References

* Course assignment specification
* Standard matrix multiplication algorithm
* Standard cache-blocking technique

---

# Assignment 01 - CSR Graph

## Objective

The objective is to convert an adjacency-list graph representation into Compressed Sparse Row (CSR) format.

CSR stores graph information using:

* `rowPtr`
* `colIdx`
* `weights`

This representation provides compact storage and efficient sequential traversal of outgoing edges.

---

## Algorithm / Approach

The adjacency-list representation is first loaded.

The CSR conversion then constructs:

```text
rowPtr
colIdx
weights
```

`rowPtr[u]` and `rowPtr[u+1]` identify the range of edges belonging to vertex `u`.

The conversion is treated as preprocessing.

---

## Input Format

The graph input contains the number of vertices and edges followed by adjacency-list information for every vertex.

---

## File Structure

```text
assignment_01/02_CSR_Graph/
├── src/
│   ├── csr_graph.cpp
│   └── csr_graph.h
├── driver/
│   └── driver_csr.cpp
└── tests/
    ├── csr_10.txt
    ├── csr_100.txt
    ├── csr_10000.txt
    ├── csr_50000.txt
    └── csr_100000.txt
```

---

## Compilation

```powershell
g++ -std=c++17 -O2 -Wall `
.\assignment_01\02_CSR_Graph\src\csr_graph.cpp `
.\assignment_01\02_CSR_Graph\driver\driver_csr.cpp `
-o .\csr.exe
```

## Execution

```powershell
.\csr.exe "assignment_01/02_CSR_Graph/tests/csr_10.txt"
```

---

## Test Cases and Result Table

| Test Case        | Vertices | Edges (input) | CSR entries | Conversion time |
| ----------------- | -------: | -------------: | -----------: | ----------------: |
| `csr_10.txt`     |       10 |             20 |           40 |        0.0036 ms |
| `csr_100.txt`    |      100 |            200 |          400 |        0.0067 ms |
| `csr_10000.txt`  |   10,000 |         20,000 |       40,000 |        0.544 ms  |
| `csr_50000.txt`  |   50,000 |        100,000 |      200,000 |        2.338 ms  |
| `csr_100000.txt` |  100,000 |        200,000 |      400,000 |        4.734 ms  |

`N/A` is used because the current CSR driver does not report a timing value.

---

## Complexity

CSR conversion requires processing every vertex and every edge.

```text
Time Complexity: O(V + E)
Space Complexity: O(V + E)
```

---

# Notes

* All algorithms were implemented in C++17.
* Compilation was performed using GNU G++.
* `-O2` optimization was used for benchmark compilation.
* Runtime measurements are machine-dependent.
* `N/A` indicates that a reliable runtime was not obtained during testing and is intentionally not replaced with an estimated value.
* The common wrapper provides a unified compilation and execution interface.