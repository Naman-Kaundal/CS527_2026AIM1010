# CS509 Laboratory Repository

## Repository Overview

This repository contains the implementation for the CS509 laboratory work.

For Assignment 3, the repository implements Minimum Spanning Tree (MST) algorithms for weighted undirected graphs using CSR (Compressed Sparse Row) representation.

The Assignment 3 algorithms implemented are:

* Kruskal's Algorithm
* Prim's Algorithm

Both algorithms use the same graph inputs so that their MST results and execution times can be compared.

The implementation is written in C++ and includes separate source files, drivers, test cases, and a common wrapper for compilation and execution.

---

## Student 
| **Student Name** | Naman Kaundal |
| **Entry Number** | 2026AIM1010 |
| **Programme** | M.Tech Artificial Intelligence |
| **Course** | CS509 |
| **Assignment** | Assignment 3 |
| **Assignment Mode** | Individual |

---

## Language and Environment

* **Programming Language:** C++
* **Standard:** C++17
* **Compiler:** GNU G++
* **Optimization:** `-O2`
* **Operating System:** Ubuntu through WSL
* **Shell:** Ubuntu Terminal
* **Editor:** Visual Studio Code

All commands in this README are intended to be executed from the repository root.

---

## Directory Structure

```
CS509_2026AIM1010/
│
├── assignment_03/
│   ├── 01_Kruskal/
│   │   ├── src/
│   │   │   ├── kruskal.cpp
│   │   │   └── kruskal.h
│   │   │
│   │   └── driver/
│   │       └── driver_kruskal.cpp
│   │
│   ├── 02_Prim/
│   │   ├── src/
│   │   │   ├── prim.cpp
│   │   │   └── prim.h
│   │   │
│   │   └── driver/
│   │       └── driver_prim.cpp
│   │
│   └── tests/
│       ├── mst_10.txt
│       ├── mst_100.txt
│       ├── mst_10000.txt
│       ├── mst_50000.txt
│       └── mst_100000.txt
│
├── common_wrapper/
│   └── wrapper.cpp
│
└── README.md
```

The CSR graph implementation used by the MST algorithms is reused from the CSR implementation already present in the repository. The adjacency-list-to-CSR conversion is performed before the MST algorithm is timed.

---

# Common Wrapper: Build and Usage

The common wrapper provides a single menu-based interface for compiling and running the algorithms in the repository.

For Assignment 3, the wrapper provides:

```
3. Assignment 3

1. Kruskal
2. Prim
0. Back
```

The user can select the algorithm and then select the required test case.

## Compilation

From the repository root:

```bash
g++ -std=c++17 -O2 common_wrapper/wrapper.cpp -o wrapper
```

## Execution

```bash
./wrapper
```

Select:

```
3. Assignment 3
```

and then select either:

```
1. Kruskal
```

or:

```
2. Prim
```

The wrapper automatically detects the `.txt` test files present in:

```
assignment_03/tests/
```

---

# Assignment 03 – Minimum Spanning Tree (MST)

## Assignment Mode

Individual implementation.

The assignment requires the implementation and comparison of:

* Kruskal's Algorithm
* Prim's Algorithm

Both algorithms are executed on the same weighted undirected graph inputs. The Assignment 3 specification requires the same graph files to be used for both algorithms so that their results and execution times can be compared.

## Objective

The objective is to implement Minimum Spanning Tree algorithms for a connected, weighted, undirected graph.

A Minimum Spanning Tree connects all vertices using exactly `V - 1` edges while minimizing the total sum of edge weights. The selected edges must not form a cycle.

The two implemented algorithms are:

* Kruskal's Algorithm
* Prim's Algorithm

Both algorithms should produce the same minimum total MST weight for the same input graph. If multiple MSTs exist, the selected edge sets may differ while the minimum total weight remains the same.

---

## Algorithm / Approach

### Kruskal's Algorithm

Kruskal's algorithm is a greedy MST algorithm.

The implementation:

1. Extracts the graph edges.
2. Sorts the edges in non-decreasing order of weight.
3. Processes the edges in sorted order.
4. Adds an edge only if its endpoints currently belong to different components.
5. Uses a Disjoint Set Union (DSU / Union-Find) structure for cycle detection.
6. Stops after `V - 1` edges have been selected.

This follows the required approach of sorting all graph edges and selecting edges that do not create a cycle.

### Prim's Algorithm

Prim's algorithm is also a greedy MST algorithm.

The implementation:

1. Starts from vertex 0.
2. Maintains the vertices already included in the MST.
3. Selects the minimum-weight edge connecting the current tree to an unvisited vertex.
4. Adds the selected vertex to the MST.
5. Continues until all vertices are included.

The assignment recommends starting from vertex 0 for reproducibility and using a minimum-priority queue for an efficient implementation.

---

## Input Format

The input is a weighted undirected adjacency-list graph.

The first line contains:

```
V E
```

where `V` = number of vertices and `E` = number of undirected edges.

Each following line contains the adjacency list of one vertex:

```
u degree neighbor1 weight1 neighbor2 weight2 ...
```

The complete format is:

```
V E
u0 degree neighbor1 weight1 neighbor2 weight2 ...
u1 degree neighbor1 weight1 neighbor2 weight2 ...
...
u(V-1) degree neighbor1 weight1 neighbor2 weight2 ...
```

For an undirected graph, every edge appears in the adjacency lists of both of its endpoints with the same weight, while `E` counts each undirected edge only once. The graph must be connected.

The same input file is used for both Kruskal and Prim.

The provided test cases use integer edge weights.

---

## Helper Functions / CSR Conversion

The input graph is initially stored as an adjacency list.

Before calling either MST algorithm, the adjacency-list representation is converted into CSR format using the existing CSR graph implementation.

The CSR representation consists of:

* `rowPtr`
* `colIdx`
* `weights`

The conversion is treated as preprocessing and is not included in the measured MST execution time.

The Assignment 3 specification explicitly requires graph inputs to be converted to CSR before the algorithm is called, and the CSR conversion time must not be included in algorithm timing.

---

## File Structure

### Kruskal

```
assignment_03/01_Kruskal/
│
├── src/
│   ├── kruskal.cpp
│   └── kruskal.h
│
└── driver/
    └── driver_kruskal.cpp
```

`kruskal.cpp` contains the core Kruskal MST implementation.

`kruskal.h` contains the declarations required by the implementation.

`driver_kruskal.cpp` reads the input file, prepares the CSR graph, calls Kruskal's algorithm, and reports the result and execution time.

### Prim

```
assignment_03/02_Prim/
│
├── src/
│   ├── prim.cpp
│   └── prim.h
│
└── driver/
    └── driver_prim.cpp
```

`prim.cpp` contains the core Prim MST implementation.

`prim.h` contains the declarations required by the implementation.

`driver_prim.cpp` reads the input file, prepares the CSR graph, calls Prim's algorithm, and reports the result and execution time.

### Test Files

```
assignment_03/tests/
├── mst_10.txt
├── mst_100.txt
├── mst_10000.txt
├── mst_50000.txt
└── mst_100000.txt
```

The required MST graph sizes are 10, 100, 10,000, 50,000 and 100,000 vertices. The same graph files are used for both MST algorithms.

---

## Compilation

### Kruskal

From the repository root:

```bash
g++ -std=c++17 -O2 \
  assignment_01/02_CSR_Graph/src/csr_graph.cpp \
  assignment_03/01_Kruskal/src/kruskal.cpp \
  assignment_03/01_Kruskal/driver/driver_kruskal.cpp \
  -o kruskal
```

### Prim

```bash
g++ -std=c++17 -O2 \
  assignment_01/02_CSR_Graph/src/csr_graph.cpp \
  assignment_03/02_Prim/src/prim.cpp \
  assignment_03/02_Prim/driver/driver_prim.cpp \
  -o prim
```

## Execution

Run the commands from the repository root. In the execution command, replace the test-case filename with the desired test case file you want to run.

### Kruskal

```bash
./kruskal assignment_03/tests/mst_10.txt
```

### Prim

```bash
./prim assignment_03/tests/mst_10.txt
```

The program reports:

* Number of MST edges
* Total MST weight
* Algorithm execution time

Only the algorithm execution time is measured. File reading, parsing, CSR conversion, and result printing are outside the timed region, as required by the assignment.

---

## Test Cases and Result Table

The following test cases were used for both Kruskal and Prim:

| Test Case | Vertices |
|---|---|
| mst_10.txt | 10 |
| mst_100.txt | 100 |
| mst_10000.txt | 10,000 |
| mst_50000.txt | 50,000 |
| mst_100000.txt | 100,000 |

### Kruskal Results

| Test File | Vertices | MST Edges | Total MST Weight | Time (ms) |
|---|---|---|---|---|
| mst_10.txt | 10 | 9 | 27.00 | 0.00 |
| mst_100.txt | 100 | 99 | 2160.00 | 0.02 |
| mst_10000.txt | 10,000 | 9,999 | 200115.00 | 1.88 |
| mst_50000.txt | 50,000 | 49,999 | 1013030.00 | 10.08 |
| mst_100000.txt | 100,000 | 99,999 | 2023168.00 | 20.09 |

### Prim Results

| Test File | Vertices | MST Edges | Total MST Weight | Time (ms) |
|---|---|---|---|---|
| mst_10.txt | 10 | 9 | 27.00 | 0.00 |
| mst_100.txt | 100 | 99 | 2160.00 | 0.03 |
| mst_10000.txt | 10,000 | 9,999 | 200115.00 | 2.77 |
| mst_50000.txt | 50,000 | 49,999 | 1013030.00 | 15.72 |
| mst_100000.txt | 100,000 | 99,999 | 2023168.00 | 43.51 |

Both algorithms produced the same total MST weight for every test case, satisfying the correctness requirement that the MST weight must match when both algorithms are run on the same graph.

For the recorded runs, Kruskal's algorithm was faster than Prim's algorithm on the larger test cases.

The measured execution time can vary depending on the system and runtime conditions.

---

## Complexity

### Kruskal's Algorithm

The main operation is sorting the graph edges.

**Time complexity:** O(E log E)

The Disjoint Set Union operations are approximately constant amortized time when path compression and union by rank/size are used.

**Space complexity:** O(V + E) for storing the graph, edge information, and DSU structures.

### Prim's Algorithm

Using a min-priority queue with the CSR graph:

**Time complexity:** O(E log V)

**Space complexity:** O(V + E)

The CSR graph itself requires storage proportional to the number of vertices and adjacency entries.

---

## References
* CS509 Assignment 3 specification — Minimum Spanning Tree (MST), Kruskal's Algorithm, Prim's Algorithm and CSR requirements.
* CS509 Assignment 3 specification — weighted undirected graph input/output format and MST requirements.