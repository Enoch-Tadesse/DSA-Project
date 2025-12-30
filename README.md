## Build and Run

This project can be compiled using either **Clang** or **GCC**.

### Using Clang
```bash
clang main.cpp airline_graph.cpp -o airline_cli
```

### Using GCC
```bash
g++ main.cpp airline_graph.cpp -o airline_cli
```

### Run
```bash
./airline_cli
```


# Airline Network Graph Algorithms

This project models an airline network as a **directed graph**, where airports are nodes and flights are directed edges with associated distance, time, and cost.  
It provides a collection of graph algorithms to analyze connectivity, reachability, and optimal routing in the network.

---

## Graph Representation

- **Node**: Represents an airport (name, city, code)
- **Edge**: Represents a directed flight (destination, distance, time, cost)
- **Graph**: `unordered_map<string, vector<Edge>>` adjacency list

---

## Implemented Algorithms

### 1. Shortest Path with User Priority
**Function:** `findShortestPath`

Computes the shortest path between two airports using a modified Dijkstra’s algorithm.  
The optimization criteria (distance, time, cost) are prioritized based on user input.

---

### 2. Strongly Connected Components (SCC)
**Function:** `findSCC`

Uses **Tarjan’s algorithm** to find strongly connected components in the airline network.  
Airports in the same SCC are mutually reachable.

---

### 3. Path Existence Check
**Function:** `pathExists`

Determines whether a path exists between two airports using **Breadth-First Search (BFS)**.

---

### 4. k-th Shortest Path
**Function:** `findKthShortestPath`

Finds the k-th shortest simple path between two airports using **Yen’s algorithm**, with Dijkstra as a subroutine.

---

### 5. Most Critical Edges
**Function:** `mostCriticalEdges`

Identifies flight routes whose removal increases the number of SCCs the most, indicating structurally critical connections.

---

### 6. Hub Connection Suggestion
**Function:** `suggestHubConnection`

Suggests two major hubs to connect by selecting the two largest SCCs in the network.

---

### 7. Reachable Airports
**Function:** `reachableNodes`

Returns all airports reachable from a given starting airport using **BFS**.

---

### 8. Ensuring Full Reachability from a Capital
**Function:** `findMissingReachableComponents`

Determines the minimum set of airport codes that must be directly connected from a given **capital airport** so that all airports become reachable.

**Approach:**
- Compress the graph into SCCs
- Build the SCC Directed Acyclic Graph (DAG)
- Perform BFS from the capital’s SCC
- Identify unreachable SCCs with zero indegree

This guarantees minimum additional connections.

---

## Technologies Used

- C++ (STL)
- Graph Algorithms:
  - BFS / DFS
  - Dijkstra
  - Tarjan’s SCC
  - Yen’s k-shortest paths

---

## Notes

- The graph is not mutated during analysis functions.
- All algorithms run in linear or near-linear time relative to the size of the graph.
- Designed for educational and analytical purposes in a Data Structures & Algorithms context.

---

## Author

Airline Graph Algorithms – DSA Project
Visit [Github](https://github.com/Enoch-Tadesse/DSA-Project) for more info.
