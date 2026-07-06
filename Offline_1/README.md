# Weighted A* Search for the N-Puzzle (15-Puzzle)

## Overview

This project implements the **A\*** and **Weighted A\*** search algorithms to solve the **N-Puzzle** problem.

The program supports:

- Solvability checking
- Multiple heuristic functions
- Adjustable heuristic weights
- Interactive puzzle solving
- Benchmarking multiple puzzle instances
- Performance analysis

The benchmark reports:

- Solution length
- Nodes explored
- Nodes expanded
- Execution time

---

# Project Structure

```text
2205119.cpp
Board.hpp
Heuristic.hpp
InversionCounter.hpp
input.txt
2205119.pdf
README.md
```

---

# File Description

---

## 1. `Board.hpp`

Defines the **Board** class.

Each object represents one puzzle configuration.

### Data Members

```cpp
int rows;
vector<vector<int>> board;
```

### Responsibilities

- Store puzzle state
- Print board
- Generate neighboring boards
- Locate tile positions
- Count inversions
- Generate unique board key
- Compare board equality

### Main Functions

| Function | Purpose |
|----------|---------|
| `printBoard()` | Prints puzzle |
| `getNextBoards()` | Generates all legal moves |
| `getPosition()` | Finds tile position |
| `getKey()` | Returns string representation for hashing |
| `getInversionCount()` | Counts inversions |
| `operator==()` | Compares two boards |

---

## 2. `Heuristic.hpp`

Defines the heuristic framework.

---

### Base Class

```cpp
class Heuristic
```

Provides

```cpp
calculate(Board current, Board goal)
```

implemented by derived classes.

---

### Implemented Heuristics

#### Hamming Distance

Counts misplaced tiles.

Formula

```text
Number of misplaced tiles
```

---

#### Manhattan Distance

Sum of row and column distances.

Formula

```text
|x1-x2| + |y1-y2|
```

Properties

- Admissible
- Consistent

---

#### Euclidean Distance

Straight-line distance.

Formula

```text
sqrt(dx²+dy²)
```

---

#### Linear Conflict

Extends Manhattan distance.

Adds

```text
+2
```

for every conflicting tile pair in the same row or column.

More informed than Manhattan.

---

#### Custom Heuristic

Implements

```text
max(Linear Conflict, Manhattan)
```

Can be easily modified for experimentation.

---

## 3. `InversionCounter.hpp`

Implements inversion counting.

Purpose

- Determine puzzle solvability

Uses

```text
Merge Sort
```

instead of

```text
O(n²)
```

comparison.

### Complexity

```text
O(n log n)
```

---

## 4. `2205119.cpp`

Contains the complete implementation.

Major classes include

---

### Puzzle

Represents the puzzle.

Responsibilities

- Store initial board
- Generate goal board
- Check solvability

Main functions

```cpp
generateTargetBoard()
isSolvable()
```

---

### State

Represents one search node.

Stores

```cpp
Board
Cost (g)
Parent pointer
```

Functions

```cpp
getPriority()
getNeighbour()
getAncestors()
```

---

### CompareState

Comparator for

```cpp
priority_queue
```

Orders states using

```text
f(n)=g(n)+W×h(n)
```

---

### Solver

Implements

```text
Weighted A*
```

Responsibilities

- Search
- Expand nodes
- Track explored nodes
- Track expanded nodes
- Measure runtime
- Print solution path

Returns

```cpp
Result
```

---

### Result

Stores

```cpp
bool solved
int explored
int expanded
int steps
double timeTaken
```

Returned after every search.

---

# Algorithm

The evaluation function is

```text
f(n)=g(n)+W×h(n)
```

where

```text
g(n)
```

Current path cost.

```text
h(n)
```

Heuristic estimate.

```text
W
```

Weight.

Supported weights

```text
1.0
1.2
2.0
5.0
```

When

```text
W=1
```

the algorithm behaves as normal A*.

Larger weights make the search greedier and faster but may produce non-optimal solutions.

---

# Solvability Check

Before searching, every puzzle is tested.

For odd board sizes

```text
Parity = inversion count
```

For even board sizes

```text
Parity = inversion count + blank row from bottom
```

If

```text
Initial parity == Goal parity
```

the puzzle is solvable.

Otherwise

```text
Search is skipped.
```

---

# Features

The program supports

- Interactive puzzle solving
- Benchmark mode
- Multiple heuristics
- Adjustable heuristic weights
- Runtime measurement
- Solution path printing
- Automatic solvability detection

---

# Interactive Mode

Menu

```text
1. Solve a puzzle
2. Benchmark
```

When solving manually

1. Enter board size
2. Enter puzzle
3. Select heuristic
4. Program solves using

```text
Weight 1
Weight 1.2
Weight 2
Weight 5
```

For every run it prints

- Solution path
- Explored nodes
- Expanded nodes
- Steps
- Time taken

---

# Benchmark Mode

Reads puzzles from

```text
input.txt
```

Runs all puzzles using

```text
Linear Conflict heuristic
```

with

```text
Weight = 1
Weight = 1.2
Weight = 2
Weight = 5
```

Outputs results into

```text
output.txt
```

Metrics collected

- Steps
- Explored nodes
- Expanded nodes
- Execution time

---

# Input Format

Interactive

Example

```text
4

1 2 3 4
5 6 7 8
9 10 11 12
13 14 0 15
```

---

Benchmark

Each puzzle begins with

```text
4
```

followed by

```text
16 numbers
```

Example

```text
4
11 1 12 8
2 3 7 4
13 5 0 10
15 14 6 9

4
14 5 4 8
1 7 3 10
15 13 6 2
0 9 11 12
```

---

# Output

Interactive output includes

```text
Solution Path

Explored Nodes

Expanded Nodes

Steps Needed

Time Taken
```

Benchmark output

```text
Puzzle #

Weight

Steps

Explored

Expanded

Time(ms)
```

---

# Complexity Analysis

Let

```text
b
```

be branching factor

and

```text
d
```

solution depth.

Worst-case

| Operation | Complexity |
|------------|------------|
| Time | O(b^d) |
| Space | O(b^d) |

Actual performance depends heavily on

- heuristic quality
- puzzle difficulty
- heuristic weight

---

# Memory Management

Dynamic memory is allocated for every generated search state.

Memory is released after every search using

```cpp
delete
```

to avoid leaks.

Memory checking performed using

```text
AddressSanitizer
```

Compile flag

```bash
-fsanitize=address
```

Used to detect

- Memory leaks
- Invalid access
- Double delete
- Heap corruption

---

# Compilation

Compile

```bash
g++ -std=c++17 -fsanitize=address -g -o puzzle_solver 2205119.cpp
```

---

# Run

Interactive

```bash
./puzzle_solver
```

Benchmark

Place all puzzles in

```text
input.txt
```

Run

```bash
./puzzle_solver
```

Select

```text
2
```

Output is generated in

```text
output.txt
```

---

# Experimental Evaluation

The benchmark compares four different heuristic weights.

Metrics include

- Solution cost
- Nodes explored
- Nodes expanded
- Runtime

This allows observation of the trade-off between

- Optimality
- Search effort
- Execution time

as the heuristic weight increases.

---

# Author

**Student ID:** 2205119

**Language:** C++

**Algorithm:** A* Search / Weighted A*

**Problem:** N-Puzzle (15-Puzzle)

**Data Structures Used:**

- Priority Queue
- Hash Map
- Vector
- Graph Search
- Merge Sort (Inversion Counting)
