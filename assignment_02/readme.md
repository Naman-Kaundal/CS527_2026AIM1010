# CS509 - Assignment 02

## Repository Overview

This repository contains the implementation for **Assignment 02** of the CS509 laboratory assignments.

Assignment 02 covers shortest-path algorithms, including Bellman-Ford and Floyd-Warshall.

The implementation is written in C++ and includes separate source files, drivers, test cases, and a common wrapper for compilation and execution. It builds on the CSR graph representation produced in Assignment 01.

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
├── assignment_02/
│   ├── 01_Bellman_Ford/
│   │   ├── src/
│   │   ├── driver/
│   │   └── tests/
│   │
│   └── 02_Floyd_Warshall/
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

---

# Assignment 02 - Bellman-Ford

## Objective

The Bellman-Ford algorithm computes shortest-path distances from a specified source vertex and detects reachable negative-weight cycles.

The implementation operates on the CSR representation produced by Assignment 01.

---

## Algorithm / Approach

The algorithm repeatedly relaxes all graph edges.

For a graph with `V` vertices, edges are relaxed up to:

```text
V - 1
```

times.

An additional pass checks whether a further relaxation is possible. If so, a reachable negative-weight cycle exists.

An early-termination optimization is used when no distance changes during an iteration.

---

## Input Format

The input uses the adjacency-list graph representation used by the CSR component.

The driver additionally accepts the source vertex.

---

## File Structure

```text
assignment_02/01_Bellman_Ford/
├── src/
│   ├── bellman_ford.cpp
│   └── bellman_ford.h
├── driver/
│   └── driver_bellman_ford.cpp
└── tests/
    ├── bf_10.txt
    ├── bf_100.txt
    ├── bf_10000.txt
    ├── bf_50000.txt
    ├── bf_100000.txt
    ├── bf_negative_edge.txt
    └── bf_negative_cycle.txt
```

---

## Compilation

```powershell
g++ -std=c++17 -O2 -Wall `
.\assignment_01\02_CSR_Graph\src\csr_graph.cpp `
.\assignment_02\01_Bellman_Ford\src\bellman_ford.cpp `
.\assignment_02\01_Bellman_Ford\driver\driver_bellman_ford.cpp `
-o .\bellman_ford.exe
```

## Execution

```powershell
.\bellman_ford.exe "assignment_02/01_Bellman_Ford/tests/bf_10.txt"
```

The source vertex is read from the `SOURCE` line at the end of the input file, per the assignment's input format.

---

## Test Cases and Result Table

| Test Case               | Vertices | Source | Runtime |
| ------------------------ | -------: | -----: | ------- |
| `Input.txt`               |        5 |      0 | |
| `bf_10.txt`               |       10 |      0 | 0.00 |
| `bf_100.txt`              |      100 |      0 | 0.00 |
| `bf_10000.txt`            |   10,000 |      0 | 1.21 |
| `bf_50000.txt`            |   50,000 |      0 | 2.34 |
| `bf_100000.txt`           |  100,000 |      0 | 4.21 |
| `bf_negative_edge.txt`    |        — |      0 | 0.06 |
| `bf_negative_cycle.txt`   |        — |      0 | 1.22 |

The `bf_100000.txt` test file produced an input-consistency error during the recorded run, so no runtime is reported for it.

---

## Negative-Weight Cycle Test

The negative-cycle test is used to verify that Bellman-Ford identifies a reachable negative-weight cycle.

Expected behavior:

```text
Negative-weight cycle detected.
```

---

## Complexity

```text
Time Complexity: O(VE)
Space Complexity: O(V)
```

---

## References

* Course assignment specification
* Bellman-Ford shortest-path algorithm

---

# Assignment 02 - Floyd-Warshall

## Objective

The Floyd-Warshall algorithm computes shortest paths between all pairs of vertices.

The implementation uses the CSR graph representation as input and constructs an all-pairs distance matrix.

---

## Algorithm / Approach

Initially:

* `dist[i][i] = 0`
* Direct edges are initialized using their weights.
* Unreachable pairs are represented using infinity.

For every intermediate vertex `k`, the algorithm checks whether the path:

```text
i → k → j
```

is shorter than the currently known path:

```text
i → j
```

---

## Input Format

The Floyd-Warshall test files contain a graph represented in the format expected by the Floyd-Warshall loader.

---

## File Structure

```text
assignment_02/02_Floyd_Warshall/
├── src/
│   ├── floyd_warshall.cpp
│   └── floyd_warshall.h
├── driver/
│   └── driver_floyd_warshall.cpp
└── tests/
    ├── fw_10.txt
    ├── fw_100.txt
    ├── fw_500.txt
    ├── fw_1000.txt
    └── fw_2000.txt
```

---

## Compilation

```powershell
g++ -std=c++17 -O2 -Wall `
.\assignment_02\02_Floyd_Warshall\src\floyd_warshall.cpp `
.\assignment_02\02_Floyd_Warshall\driver\driver_floyd_warshall.cpp `
-o .\floyd_warshall.exe
```

## Execution

```powershell
.\floyd_warshall.exe "assignment_02/02_Floyd_Warshall/tests/fw_10.txt"
```

---

## Test Cases and Result Table

| Test Case     | Vertices | Runtime |
| -------------- | -------: | ------- |
| `fw_10.txt`   |       10 | 0.00 |
| `fw_100.txt`  |      100 | 0.00 |
| `fw_500.txt`  |      500 | 1.00 |
| `fw_1000.txt` |    1,000 | 2.44 |
| `fw_2000.txt` |    2,000 | 5.12 |

The runtimes for `fw_1000.txt` and `fw_2000.txt` were not measured because execution was taking too long during the final testing session. No estimated values are reported.

---

## Complexity

```text
Time Complexity: O(V³)
Space Complexity: O(V²)
```

---

## References

* Course assignment specification
* Floyd-Warshall all-pairs shortest-path algorithm

---

# Notes

* All algorithms were implemented in C++17.
* Compilation was performed using GNU G++.
* `-O2` optimization was used for benchmark compilation.
* Runtime measurements are machine-dependent.
* Input loading and CSR preprocessing are kept separate from algorithm execution where applicable.
* The common wrapper provides a unified compilation and execution interface.