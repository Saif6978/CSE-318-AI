# Max-Cut via GRASP

Implementation and experimental evaluation of five heuristics for the **Maximum Cut (Max-Cut)** problem: Randomized Construction, Greedy Construction, Semi-Greedy Construction, Local Search, and **GRASP** (Greedy Randomized Adaptive Search Procedure).

Coursework: CSE 318, Assignment-02.
Student ID: **2205119**

---

## Table of Contents

- [Problem Statement](#problem-statement)
- [Repository Contents](#repository-contents)
- [Input Format](#input-format)
- [Algorithms](#algorithms)
  - [1. Randomized Construction](#1-randomized-construction)
  - [2. Greedy Construction](#2-greedy-construction)
  - [3. Semi-Greedy Construction](#3-semi-greedy-construction)
  - [4. Local Search](#4-local-search)
  - [5. GRASP](#5-grasp)
- [Code Structure](#code-structure)
- [How to Build and Run](#how-to-build-and-run)
  - [Option 1 — Single graph mode](#option-1--single-graph-mode)
  - [Option 2 — Full benchmark mode](#option-2--full-benchmark-mode)
- [Output Files](#output-files)
- [Time Complexity](#time-complexity)
- [Results Summary](#results-summary)
- [Strengths and Limitations](#strengths-and-limitations)
- [Future Improvements](#future-improvements)

---

## Problem Statement

Given an undirected, edge-weighted graph `G = (V, E)` with weights `w(u, v)` on each edge, the **Maximum Cut problem** asks for a partition of the vertex set into two disjoint, non-empty subsets `X` and `Y` such that the total weight of edges crossing between `X` and `Y` is maximized:

```
Cut(X, Y) = Σ w(u, v)   for every edge (u, v) with u ∈ X, v ∈ Y
```

Max-Cut is **NP-hard**, so for large graphs we rely on heuristics and metaheuristics (like GRASP) rather than exact algorithms.

---

## Repository Contents

| File | Description |
|---|---|
| `2205119.cpp` | Full C++ implementation of all five algorithms plus the benchmarking driver |
| `2205119.csv` | Results of running all algorithms on the 54 benchmark graphs |
| `2205119.pdf` | Written report describing algorithms, methodology, and analysis |
| `README.md` | This file |

> The benchmark graph files themselves (`g1.rud` … `g54.rud`, the standard Max-Cut benchmark set) are expected in a folder such as `graph_GRASP/set1/` relative to the executable — see [How to Build and Run](#how-to-build-and-run).

---

## Input Format

Each graph is provided as a plain text file:

```
n m
V1 V2 W1
V1 V2 W2
...
```

- Line 1: `n` = number of vertices, `m` = number of edges.
- Each of the next `m` lines: an edge `(V1, V2)` with integer weight `W`.
- Vertices are 1-indexed. Graphs are undirected.

Example (5 vertices, 7 edges):

```
5 7
1 2 5
1 5 6
2 3 9
2 5 10
3 4 15
3 5 2
4 5 10
```

---

## Algorithms

### 1. Randomized Construction

Every vertex is placed independently into partition `X` or `Y` with probability ½ each — no structural information about the graph is used at all. Because a single run has high variance, the algorithm is repeated `n` times (30 in this project) and the **average** cut weight is reported as the estimate of expected performance.

- **Purpose:** a baseline / lower-bound reference point for judging the other heuristics.
- **Code:** `randomizedMaxCut()` (single trial), `randomizedAverageCut()` (averages over trials).

### 2. Greedy Construction

A fully deterministic, locally-optimal construction:

1. Find the edge `(u, v)` with the **maximum weight** in the graph. Seed `X = {u}`, `Y = {v}`.
2. For every remaining unassigned vertex `z`, maintain running sums:
   - `σX(z)` = total weight of edges from `z` to vertices currently in `X`
   - `σY(z)` = total weight of edges from `z` to vertices currently in `Y`
3. At each step, pick the unassigned vertex with the **largest** `max(σX, σY)` value (i.e., the vertex most strongly "pulled" toward one side) and place it on whichever side it is more strongly connected to.
4. Repeat until every vertex is assigned.

Because it always takes the best immediate choice, Greedy is fast, but early decisions are irrevocable — a poor early placement can never be corrected later.

- **Code:** `findMaximumEdge()`, `greedyMaxCut()`.

### 3. Semi-Greedy Construction

Semi-Greedy generalizes Greedy by injecting **controlled randomness** into vertex selection, using a **Restricted Candidate List (RCL)** built with the *value-based* method:

1. Seed the same way as Greedy (max-weight edge → `X`, `Y`).
2. For each unassigned vertex `v`, compute `value(v) = max(σX(v), σY(v))`.
3. Compute `wmin = min(value)` and `wmax = max(value)` over all currently unassigned vertices.
4. Compute the cutoff threshold:

   ```
   μ = wmin + α · (wmax − wmin),   0 ≤ α ≤ 1
   ```

5. Build the RCL = { all unassigned vertices `v` with `value(v) ≥ μ` }.
6. Pick **one** vertex from the RCL **uniformly at random**, and assign it to the side it's more strongly connected to (same rule as Greedy).
7. Update `σX`/`σY` sums for remaining vertices and repeat until all vertices are assigned.

`α` controls the exploitation/exploration trade-off:
- `α = 0` → pure random selection among all non-negative candidates (maximum diversification)
- `α = 1` → pure greedy selection (only the single best candidate qualifies)
- This project uses **α = 0.5** as a balanced default.

Because construction is randomized, running Semi-Greedy repeatedly yields a *diverse* set of good starting solutions — which is exactly what GRASP needs.

- **Code:** `semiGreedyMaxCut()`.

### 4. Local Search

Local Search takes a **feasible partition** (typically from Semi-Greedy) and iteratively improves it using a simple 1-vertex-flip neighborhood:

- For a vertex `v` currently in side `S`, define the gain of moving it to the other side:

  ```
  δ(v) = σ_S(v) − σ_S̄(v)     if v ∈ S
  δ(v) = σ_S̄(v) − σ_S(v)     if v ∈ S̄
  ```

  (`σ_S(v)` = weight of edges from `v` to same-side neighbors, `σ_S̄(v)` = weight of edges from `v` to opposite-side neighbors.)

- At each iteration, evaluate `δ(v)` for **every** vertex, and flip the vertex with the **largest positive** `δ(v)` (best-improvement / steepest-ascent strategy).
- Update the `σX`/`σY` running sums incrementally for the flipped vertex's neighbors.
- Stop when no vertex has `δ(v) > 0` — the current partition is a **local optimum** with respect to single-vertex moves.

Local Search always increases or preserves the cut value, but the final result is highly dependent on the initial (Semi-Greedy) solution it starts from — hence the motivation for GRASP.

- **Code:** `localSearchMaxCut()`.

### 5. GRASP

**GRASP (Greedy Randomized Adaptive Search Procedure)** repeatedly combines Semi-Greedy construction with Local Search refinement, keeping the best solution found:

```
BestCut ← −∞
for i = 1 to MaxIterations:
    x ← Semi-Greedy construction (randomized, with parameter α)
    x ← Local Search(x)
    if Cut(x) > BestCut:
        BestCut ← Cut(x)
return BestCut
```

Each iteration explores a different region of the search space (via a different random Semi-Greedy start) and thoroughly exploits that region (via Local Search until convergence). Over many iterations this combination reliably escapes the poor local optima that trap any single Local Search run, and in this project's experiments it produced the best cut value on every single benchmark graph.

- **Code:** `graspMaxCut()` — this project uses **100 iterations** by default.

---

## Code Structure

`2205119.cpp` is organized as follows:

```
Edge struct           → simple (u, v, weight) tuple
Graph class           → adjacency-list representation (vector<vector<pair<int,int>>>)
calculateCutWeight()   → computes the cut weight of a given partition
randomizedMaxCut()     → one random-trial cut
randomizedAverageCut() → averages randomizedMaxCut() over many trials
findMaximumEdge()      → finds the max-weight edge (used to seed Greedy/Semi-Greedy)
greedyMaxCut()         → deterministic greedy construction
semiGreedyMaxCut()     → randomized greedy construction with RCL (α-parameterized)
localSearchMaxCut()    → 1-flip steepest-ascent local search
graspMaxCut()          → Semi-Greedy + Local Search, repeated, keeping the best
readGraph()            → parses a .rud-style input file into a Graph
knownBestMap           → known best/upper-bound values for 24 reference graphs
benchmark()            → runs all algorithms on all 54 graphs and writes a CSV
main()                 → interactive menu: single-graph mode or full-benchmark mode
```

All graph data is stored as a weighted adjacency list for `O(1)` amortized edge iteration; running `σX`/`σY` sums (rather than recomputing sums from scratch every step) keep Greedy/Semi-Greedy/Local-Search efficient.

---

## How to Build and Run

### Requirements

- A C++17-capable compiler (e.g. `g++`, `clang++`).
- The 54 benchmark graph files (standard `.rud` Max-Cut benchmark instances, e.g. `g1.rud`, `g2.rud`, ... `g54.rud`).

### Build

```bash
g++ -O2 -std=c++17 -o maxcut 2205119.cpp
```

### Option 1 — Single graph mode

Run the executable and choose option `1` to test all algorithms on one graph file:

```bash
./maxcut
```

```
=========================================
          MAX-CUT USING GRASP
=========================================
1. Run algorithms on a single graph
2. Run benchmark on all graphs
0. Exit
=========================================
Enter choice: 1

Enter graph file path: graph_GRASP/set1/g1.rud
```

This prints, for the chosen graph:
- Randomized average cut (1000 trials)
- Greedy cut
- Semi-Greedy average & best cut (30 runs, α = 0.5)
- Local Search average & best cut (30 runs, starting from Semi-Greedy)
- GRASP best cut (100 iterations)

### Option 2 — Full benchmark mode

Choose option `2` to automatically process all 54 graphs and write a results CSV:

```
Enter choice: 2
```

By default this expects graphs at `../graph_GRASP/set1/g{1..54}.rud` (relative to where the executable is run) and writes results to `output.txt`. Adjust the folder path in `main()`'s call to `benchmark(...)` (or the default arguments of `benchmark()` itself) if your directory layout differs, e.g.:

```cpp
benchmark(54, "graph_GRASP/set1/", "2205119_output.csv");
```

Default experimental configuration used throughout:

| Parameter | Value |
|---|---|
| Randomized trials | 30 |
| Semi-Greedy runs | 30 |
| Local Search runs | 30 (each started from a fresh Semi-Greedy solution) |
| GRASP iterations | 100 |
| Semi-Greedy α | 0.5 |
| Random seed | 42 |

---

## Output Files

`benchmark()` writes a CSV with one row per graph and the following columns:

```
Name, V, E, Randomized_avg, Greedy,
SemiGreedy_avg, SemiGreedy_best, SemiGreedy_alpha,
LocalSearch_avg, LocalSearch_best, LocalSearch_reps,
GRASP_best, GRASP_iterations, KnownBestOrUpperBound
```

`KnownBestOrUpperBound` is filled in only for the 24 graphs with a published reference value (see the `knownBestMap` table in the source), letting you directly compare GRASP's output against the literature.

---

## Time Complexity

| Algorithm | Complexity | Notes |
|---|---|---|
| Randomized | `O(V)` per trial | Each vertex assigned independently |
| Greedy | `O(V² + E)` | Quadratic scan to pick the next best vertex at each of the `V` steps |
| Semi-Greedy | `O(V² + E)` | Same structure as Greedy, plus RCL construction |
| Local Search | `O(k·E)` | `k` = number of improving iterations until convergence |
| GRASP | `O(I·(V² + k·E))` | `I` = number of GRASP iterations |

(`V` = vertices, `E` = edges.)

---

## Results Summary

Across all 54 benchmark graphs, the algorithms show a consistent quality ordering:

```
Randomized  <  Greedy  <  Semi-Greedy  ≲  Local Search  <  GRASP
```

- **Randomized** consistently produces the weakest cuts since it ignores graph structure entirely.
- **Greedy** substantially improves on Randomized via deterministic locally-optimal choices, but can get stuck due to irreversible early decisions.
- **Semi-Greedy** matches or slightly exceeds Greedy on average by exploring multiple constructive paths.
- **Local Search**, applied on top of Semi-Greedy, gives a further consistent boost by repeatedly relocating vertices with positive gain until reaching a local optimum.
- **GRASP** — repeated Semi-Greedy + Local Search, keeping the best result — achieves the best cut value on essentially every graph tested, and gets closest to the known best/upper-bound values (e.g. within a few percent on graphs like G1–G3, G11–G13, G22–G24). On some easier instances (e.g. G48, G49) every heuristic reaches the same optimal value.

For full per-graph numbers, see `2205119.csv`; for discussion, plots, and per-group analysis (G1–G11, G12–G22, G23–G33, G34–G44, G45–G54), see `2205119.pdf`.

---

## Strengths and Limitations

**Randomized** — trivially fast and simple, but ignores graph structure and has high run-to-run variance.

**Greedy** — fast, deterministic, much better than random, but early decisions can't be revised and it can get trapped in poor configurations.

**Semi-Greedy** — balances greediness with randomness via the RCL, giving diverse and generally strong starting solutions; performance is sensitive to the choice of `α`.

**Local Search** — guaranteed to never worsen a solution and converges quickly, but is a pure hill-climber that cannot escape local optima and depends heavily on its starting point.

**GRASP** — combines exploration (via repeated randomized construction) with exploitation (via local search), consistently yielding the best results of all five methods, at the cost of the highest total computation time and the need to tune both `α` and the iteration count.

---

## Future Improvements

- **Reactive GRASP** — adaptively tune `α` based on recent iteration performance instead of using a fixed value.
- **Path Relinking** — intensify search by exploring paths between pairs of elite (high-quality) solutions found across iterations.
- **Parallel GRASP** — run independent GRASP iterations concurrently since they don't share state.
- **Hybridization** — combine with Tabu Search or Simulated Annealing to further escape local optima.

---

## Author

Saif Al Islam — Student ID: 2205119
