# CS509 Laboratory Repository

## Repository Overview

This repository contains the implementation for the CS509 laboratory work.

For Assignment 4, the repository implements graph algorithms for sparse graphs using CSR (Compressed Sparse Row) representation.

The Assignment 4 individual algorithms implemented are:

* Greedy Vertex Coloring using Welsh-Powell ordering

* PageRank

Both algorithms use the CSR graph representation already implemented in Assignment 1. The adjacency-list graph is converted to CSR before the corresponding algorithm is executed.

The implementation is written in C++ and includes separate source files, drivers, test cases, and a common wrapper for compilation and execution.

The buddy tasks specified in the assignment are not included in this implementation.

---

## Student

| **Student Name**    | Naman Kaundal                  |
| ------------------- | ------------------------------ |
| **Entry Number**    | 2026AIM1010                    |
| **Programme**       | M.Tech Artificial Intelligence |
| **Course**          | CS509                          |
| **Assignment**      | Assignment 4                   |
| **Assignment Mode** | Individual                     |

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

CS509_2026AIM1010/

│

├── assignment_01/

│   └── 02_CSR_Graph/

│       └── src/

│           ├── csr_graph.cpp

│           └── csr_graph.h

│

├── assignment_04/

│   │

│   ├── 01_Graph_Coloring/

│   │   ├── src/

│   │   │   ├── graph_coloring.cpp

│   │   │   └── graph_coloring.h

│   │   │

│   │   ├── driver/

│   │   │   └── driver_graph_coloring.cpp

│   │   │

│   │   └── tests/

│   │       ├── color_10.txt

│   │       ├── color_100.txt

│   │       ├── color_10000.txt

│   │       ├── color_50000.txt

│   │       └── color_100000.txt

│   │

│   ├── 02_PageRank/

│   │   ├── src/

│   │   │   ├── pagerank.cpp

│   │   │   └── pagerank.h

│   │   │

│   │   ├── driver/

│   │   │   └── driver_pagerank.cpp

│   │   │

│   │   └── tests/

│   │       ├── pagerank_10.txt

│   │       ├── pagerank_100.txt

│   │       ├── pagerank_1000.txt

│   │       ├── pagerank_10000.txt

│   │       └── pagerank_50000.txt

│   │

│   └── readme.md

│

├── common_wrapper/

│   └── wrapper.cpp

│

└── README.md

The CSR graph implementation used by both Assignment 4 algorithms is reused from the CSR implementation already present in Assignment 1.

The adjacency-list graph is converted to CSR before the corresponding algorithm is executed. The CSR conversion is treated as preprocessing and is not included in the measured algorithm execution time.

---

# Common Wrapper: Build and Usage

The common wrapper provides a single menu-based interface for compiling and running the algorithms in the repository.

For Assignment 4, the wrapper provides:

```
4. Assignment 4

1. Graph Coloring
2. PageRank
0. Back
```

The user can select the algorithm and then select the required test case.

## Compilation

From the repository root:

```
g++ -std=c++17 -O2 common_wrapper/wrapper.cpp -o wrapper
```

## Execution

```
./wrapper
```

Select:

```
4. Assignment 4
```

and then select either:

```
1. Graph Coloring
```

or:

```
2. PageRank
```

The wrapper provides the available `.txt` test files for the selected algorithm.

---

# Assignment 04 – Graph Coloring and PageRank

## Assignment Mode

Individual implementation.

The assignment requires the implementation of:

* Greedy Vertex Coloring using Welsh-Powell ordering

* PageRank

The buddy tasks are not included in this implementation.

Both algorithms operate on graphs represented using CSR after the input adjacency-list graph has been converted using the existing Assignment 1 CSR implementation.

## Objective

The objective of Assignment 4 is to implement graph algorithms efficiently for sparse graphs using CSR representation.

The two implemented algorithms are:

* Greedy Vertex Coloring

* PageRank

The algorithms are implemented separately and are tested on graphs of different sizes.

The execution time of each algorithm is measured separately from input processing and CSR conversion.

---

## Algorithm / Approach

### Greedy Vertex Coloring

The Graph Coloring implementation uses the required Greedy Welsh-Powell approach.

The implementation:

1. Calculates the degree of every vertex using the CSR `rowPtr` array.

2. Creates an ordering of vertices based on decreasing degree.

3. Processes vertices according to this ordering.

4. Assigns the smallest available color to each vertex.

5. Checks the colors of neighbouring vertices using the CSR adjacency information.

6. Verifies that no adjacent vertices have the same color.

7. Reports the number of colors used and whether the resulting coloring is valid.

The Welsh-Powell ordering is based on processing vertices in non-increasing order of degree.

The CSR representation is used directly by the coloring algorithm.

### PageRank

The PageRank implementation uses the iterative PageRank algorithm.

The implementation:

1. Initializes every vertex with rank `1 / V`.

2. Uses the damping factor provided in the input.

3. Calculates new PageRank values from the ranks of the previous iteration.

4. Distributes the rank of each vertex among its outgoing neighbours.

5. Handles dangling vertices separately.

6. Calculates the total change between the old and new rank vectors.

7. Stops when the change is less than or equal to the specified tolerance.

8. Stops at the maximum number of iterations if convergence is not reached.

9. Reports the final rank of every vertex, the sum of ranks, number of iterations, convergence status, and execution time.

The PageRank implementation uses simultaneous updates, meaning that the new rank vector is calculated using the values from the previous iteration.

---

## Input Format

### Graph Coloring

The Graph Coloring input is an unweighted undirected adjacency-list graph.

The first line contains:

```
V E
```

where `V` is the number of vertices and `E` is the number of undirected edges.

Each following line contains:

```
vertex degree neighbour1 neighbour2 ...
```

The complete format is:

```
V E

u0 degree neighbour1 neighbour2 ...

u1 degree neighbour1 neighbour2 ...

...

u(V-1) degree neighbour1 neighbour2 ...
```

For an undirected graph, an edge is present in the adjacency lists of both of its endpoints.

The value of `E` represents the number of undirected edges.

The graph is read as an adjacency list and then converted to CSR using the existing Assignment 1 CSR implementation.

### PageRank

The PageRank input is a directed adjacency-list graph.

The first line contains:

```
V E
```

where `V` is the number of vertices and `E` is the number of directed edges.

Each following line contains:

```
vertex outdegree neighbour1 neighbour2 ...
```

The complete graph section is:

```
V E

u0 outdegree neighbour1 neighbour2 ...

u1 outdegree neighbour1 neighbour2 ...

...

u(V-1) outdegree neighbour1 neighbour2 ...
```

For PageRank, an edge from `u` to `v` does not imply an edge from `v` to `u`.

The graph section is followed by the PageRank parameters:

```
DAMPING 0.85
TOLERANCE 0.0001
MAX_ITERATIONS 100
```

The damping factor must be between 0 and 1.

The tolerance specifies the convergence threshold.

`MAX_ITERATIONS` specifies the maximum number of PageRank iterations.

---

## Helper Functions / CSR Conversion

The input graph is initially stored as an adjacency list.

Before either Assignment 4 algorithm is executed, the adjacency-list representation is converted into CSR format using the existing CSR graph implementation from Assignment 1.

The CSR representation consists of:

* `rowPtr`

* `colIdx`

* `weights`

For Graph Coloring, the algorithm uses the CSR row pointer and neighbour information.

For PageRank, the algorithm uses the CSR row pointer and outgoing neighbour information.

The Assignment 1 CSR implementation is reused directly. No separate CSR conversion implementation is created for Assignment 4.

The CSR conversion is treated as preprocessing and is not included in the measured algorithm execution time.

The algorithm timer starts only after CSR conversion has been completed.

---

## File Structure

### Graph Coloring

```
assignment_04/01_Graph_Coloring/

│

├── src/

│   ├── graph_coloring.cpp

│   └── graph_coloring.h

│

├── driver/

│   └── driver_graph_coloring.cpp

│

└── tests/

    ├── color_10.txt

    ├── color_100.txt

    ├── color_10000.txt

    ├── color_50000.txt

    └── color_100000.txt
```

`graph_coloring.cpp` contains the core Greedy Welsh-Powell graph coloring implementation.

`graph_coloring.h` contains the declarations and result structure required by the implementation.

`driver_graph_coloring.cpp` reads the input graph, uses the existing Assignment 1 CSR conversion, calls the Graph Coloring algorithm, and reports the colors and execution time.

### PageRank

```
assignment_04/02_PageRank/

│

├── src/

│   ├── pagerank.cpp

│   └── pagerank.h

│

├── driver/

│   └── driver_pagerank.cpp

│

└── tests/

    ├── pagerank_10.txt

    ├── pagerank_100.txt

    ├── pagerank_1000.txt

    ├── pagerank_10000.txt

    └── pagerank_50000.txt
```

`pagerank.cpp` contains the core iterative PageRank implementation.

`pagerank.h` contains the declarations and result structure required by the implementation.

`driver_pagerank.cpp` reads the input graph and PageRank parameters, uses the existing Assignment 1 CSR conversion, calls PageRank, and reports the final ranks and execution time.

### Test Files

Graph Coloring test cases:

```
assignment_04/01_Graph_Coloring/tests/

├── color_10.txt
├── color_100.txt
├── color_10000.txt
├── color_50000.txt
└── color_100000.txt
```

The required Graph Coloring graph sizes are:

* 10 vertices

* 100 vertices

* 10,000 vertices

* 50,000 vertices

* 100,000 vertices

PageRank test cases:

```
assignment_04/02_PageRank/tests/

├── pagerank_10.txt
├── pagerank_100.txt
├── pagerank_1000.txt
├── pagerank_10000.txt
└── pagerank_50000.txt
```

The required PageRank graph sizes are:

* 10 vertices

* 100 vertices

* 1,000 vertices

* 10,000 vertices

* 50,000 vertices

---

## Compilation

### Graph Coloring

From the repository root:

```
g++ -std=c++17 -O2 \
  assignment_01/02_CSR_Graph/src/csr_graph.cpp \
  assignment_04/01_Graph_Coloring/src/graph_coloring.cpp \
  assignment_04/01_Graph_Coloring/driver/driver_graph_coloring.cpp \
  -o graph_coloring
```

### PageRank

```
g++ -std=c++17 -O2 \
  assignment_01/02_CSR_Graph/src/csr_graph.cpp \
  assignment_04/02_PageRank/src/pagerank.cpp \
  assignment_04/02_PageRank/driver/driver_pagerank.cpp \
  -o pagerank
```

---

## Execution

Run the commands from the repository root.

### Graph Coloring

For the 10-vertex test case:

```
./graph_coloring assignment_04/01_Graph_Coloring/tests/color_10.txt
```

For the other test cases, replace the filename with the required test file.

The program reports:

* Color assigned to every vertex

* Number of colors used

* Whether the coloring is valid

* Algorithm execution time

### PageRank

For the 10-vertex test case:

```
./pagerank assignment_04/02_PageRank/tests/pagerank_10.txt
```

For the other test cases, replace the filename with the required test file.

The program reports:

* Damping factor

* PageRank of every vertex

* Sum of ranks

* Number of iterations

* Convergence status

* Algorithm execution time

Only the algorithm execution time is measured.

File reading, input parsing, CSR conversion, and result printing are outside the timed region.

---

## Test Cases and Result Table

### Graph Coloring Test Cases

| Test Case        | Vertices |
| ---------------- | -------: |
| color_10.txt     |       10 |
| color_100.txt    |      100 |
| color_10000.txt  |   10,000 |
| color_50000.txt  |   50,000 |
| color_100000.txt |  100,000 |

### Graph Coloring Results

| Test File        | Vertices |    Colors Used | Valid          |      Time (ms) |
| ---------------- | -------: | -------------: | -------------- | -------------: |
| color_10.txt     |       10 | To be recorded | To be recorded | To be recorded |
| color_100.txt    |      100 | To be recorded | To be recorded | To be recorded |
| color_10000.txt  |   10,000 | To be recorded | To be recorded | To be recorded |
| color_50000.txt  |   50,000 | To be recorded | To be recorded | To be recorded |
| color_100000.txt |  100,000 | To be recorded | To be recorded | To be recorded |

### PageRank Test Cases

| Test Case          | Vertices |
| ------------------ | -------: |
| pagerank_10.txt    |       10 |
| pagerank_100.txt   |      100 |
| pagerank_1000.txt  |    1,000 |
| pagerank_10000.txt |   10,000 |
| pagerank_50000.txt |   50,000 |

### PageRank Results

| Test File          | Vertices |     Iterations | Converged      |      Time (ms) |
| ------------------ | -------: | -------------: | -------------- | -------------: |
| pagerank_10.txt    |       10 | To be recorded | To be recorded | To be recorded |
| pagerank_100.txt   |      100 | To be recorded | To be recorded | To be recorded |
| pagerank_1000.txt  |    1,000 | To be recorded | To be recorded | To be recorded |
| pagerank_10000.txt |   10,000 | To be recorded | To be recorded | To be recorded |
| pagerank_50000.txt |   50,000 | To be recorded | To be recorded | To be recorded |

The actual execution times depend on the system and runtime conditions and should be recorded after running the final implementation.

---

## Complexity

### Greedy Vertex Coloring

The implementation first calculates the degree of every vertex using CSR.

The vertices are then sorted by decreasing degree.

The sorting operation requires:

**Time complexity:** O(V log V)

The coloring phase checks the neighbours of each vertex. For a sparse graph, this is based on the number of adjacency entries.

The overall coloring phase is approximately:

**Time complexity:** O(V log V + E × C)

where `C` is the number of colors considered during the greedy coloring process.

**Space complexity:** O(V + E)

The CSR representation requires storage proportional to the vertices and adjacency entries, while the coloring algorithm requires additional storage for degree, ordering, and color arrays.

### PageRank

Each PageRank iteration processes the vertices and their outgoing edges.

For a graph with `V` vertices and `E` directed edges:

**Time complexity:** O(I × (V + E))

where `I` is the number of iterations performed.

**Space complexity:** O(V + E)

The CSR graph requires O(V + E) storage, while the PageRank algorithm maintains the current and new rank vectors.

---

## References

* CS509 Assignment 4 specification — Greedy Vertex Coloring using Welsh-Powell ordering.

* CS509 Assignment 4 specification — PageRank algorithm, convergence, damping factor, dangling vertices, input/output format, and required test cases.

* CS509 Assignment 1 — CSR Graph implementation reused for Assignment 4.
