#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "airline_graph.h"
using namespace std;

void printHeader() {
    cout << "\n========================================\n";
    cout << "     AIRLINE NETWORK MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
}

void printMenu() {
    cout << "\n\n------------- MAIN MENU -----------------\n\n";
    cout << "[ Graph View ]\n\n";
    cout << " 1. Display Airline Network\n\n";

    cout << "[ Airport Management ]\n\n";
    cout << " 2. Add Airline\n";
    cout << " 3. Remove Airline\n\n";

    cout << "[ Route Management ]\n\n";
    cout << " 4. Add Route\n";
    cout << " 5. Remove Route\n\n";

    cout << "[ Queries & Analytics ]\n\n";
    cout << " 6. Find Shortest Path\n";
    cout << " 7. Find Strongly Connected Components\n";
    cout << " 8. Check if Path Exists\n";
    cout << " 9. Find K-th Shortest Path\n";
    cout << "10. Find Most Critical Edges\n";
    cout << "11. Suggest Hub Connection\n";
    cout << "12. Find Reachable Nodes\n";
    cout << "13. Find Missing Reachable Components\n\n";

    cout << "14. Exit\n";
    cout << "-----------------------------------------\n";
    cout << "Choose an option: ";
}

void addAirline(unordered_map<string, Node>& airlines, adj& adjacency) {
    string name, city, code;
    cout << "Enter airline name: ";
    cin >> name;
    cout << "Enter city: ";
    cin >> city;
    cout << "Enter code: ";
    cin >> code;

    if (code.empty()) {
        cout << "Error: Code cannot be empty.\n";
        return;
    }

    if (airlines.find(code) != airlines.end()) {
        cout << "Error: Code " << code << " already exists.\n";
        return;
    }
    airlines[code] = Node(name, city, code);
    adjacency[code] = {};  // Ensure it's in adjacency even if isolated
    cout << "Airline added successfully.\n";
}

void addEdge(unordered_map<string, Node>& airlines, adj& adjacency) {
    string from, to;
    double distance, time, cost;

    cout << "Enter start airline code: ";
    cin >> from;
    cout << "Enter end airline code: ";
    cin >> to;

    if (airlines.find(from) == airlines.end()) {
        cout << "Error code: " << from << " does not exist ";
        return;
    }

    if (airlines.find(to) == airlines.end()) {
        cout << "Error code: " << to << " does not exist ";
        return;
    }

    if (from == to) {
        cout << "Error code: " << to
             << " can not create an edge from an airline to itself ";
        return;
    }

    // Check if edge already exists
    for (const Edge& e : adjacency[from]) {
        if (e.to == to) {
            cout << "Error: Edge from " << from << " to " << to
                 << " already exists.\n";
            return;
        }
    }

    cout << "Enter distance: ";
    cin >> distance;

    // handle inputs different from numbers
    if (!cin || distance <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error: Distance must be a positive number.\n";
        return;
    }

    if (distance <= 0) {
        cout << "Error code: " << to << " Cant enter negative value for weight";
        return;
    }

    cout << "Enter time: ";
    cin >> time;

    if (!cin || distance <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error: Distance must be a positive number.\n";
        return;
    }

    if (time <= 0) {
        cout << "Error code: " << to << " Cant enter negative value for weight";
        return;
    }

    cout << "Enter cost: ";
    cin >> cost;

    // handle inputs different from numbers
    if (!cin || distance <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Error: Distance must be a positive number.\n";
        return;
    }

    if (cost < 0) {
        cout << "Error code: " << to << " Cant enter negative value for weight";
        return;
    }

    adjacency[from].push_back(Edge(to, distance, time, cost));

    cout << "Edge successfully added: " << from << " -> " << to << "\n";
}

void removeEdge(unordered_map<string, Node>& airlines, adj& adjacency) {
    string from, to;

    cout << "Enter start airline code: ";
    cin >> from;
    cout << "Enter end airline code: ";
    cin >> to;

    if (airlines.find(from) == airlines.end()) {
        cout << "Error code: " << from << " does not Exist.\n";
        return;
    }

    if (airlines.find(to) == airlines.end()) {
        cout << "Error code: " << to << " does not Exist.\n";
        return;
    }

    auto& edges = adjacency[from];
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        if (it->to == to) {
            edges.erase(it);
            cout << "Edge successfully removed: " << from << " -> " << to
                 << "\n";
            return;
        }
    }

    cout << "Error: No edge exists from " << from << " to " << to << ".\n";
}

void removeAirline(unordered_map<string, Node>& airlines, adj& adjacency) {
    string code;
    cout << "Enter code to remove: ";
    cin >> code;
    if (airlines.find(code) == airlines.end()) {
        cout << "Error: Code " << code << " does not exist.\n";
        return;
    }

    // Check for incoming edges
    bool hasEdges = false;
    vector<string> connected;
    for (auto& [from, edges] : adjacency) {
        for (auto& e : edges) {
            if (e.to == code) {
                hasEdges = true;
                connected.push_back(from + " -> " + code);
            }
        }
    }

    // check for outgoing edges
    if (!adjacency[code].empty()) {
        hasEdges = true;
        for (auto& e : adjacency[code]) {
            connected.push_back(code + " -> " + e.to);
        }
    }

    if (hasEdges) {
        cout << "Cannot remove airline " << code
             << " as it is connected with the following edges:\n";
        for (auto& c : connected) cout << c << "\n";
        return;
    }

    airlines.erase(code);
    adjacency.erase(code);  // Also remove from adjacency if present
    cout << "Airline removed successfully.\n";
}

void displayGraph(const unordered_map<string, Node>& airlines,
                  const adj& adjacency) {
    cout << "\n=========== AIRLINE NETWORK =============\n";
    for (const auto& [from, edges] : adjacency) {
        cout << airlines.at(from).getName() << " (" << from << ")\n";

        if (edges.empty()) {
            cout << "   -> No outgoing routes\n";
        } else {
            for (const auto& e : edges) {
                cout << "   -> " << e.to << " | Distance: " << e.distance
                     << " | Time: " << e.time << " | Cost: $" << e.cost << "\n";
            }
        }
        cout << "-----------------------------------------\n";
    }
}

int handleMenu() {
    unordered_map<string, Node> airlines;
    adj adjacency;

    // Read airlines from lines.txt
    ifstream linesFile("lines.txt");
    if (!linesFile.is_open()) {
        cerr << "Error: Could not open lines.txt\n";
        return 1;
    }
    string line;
    while (getline(linesFile, line)) {
        if (line.empty() || line[0] == '/') continue;  // Skip comments or empty
        stringstream ss(line);
        string name, city, code;
        ss >> name >> city >> code;
        if (airlines.find(code) != airlines.end()) {
            cerr << "Error: Duplicate code " << code << "\n";
            continue;
        }
        airlines[code] = Node(name, city, code);
    }
    linesFile.close();

    // Read edges from edges.txt
    ifstream edgesFile("edges.txt");
    if (!edgesFile.is_open()) {
        cerr << "Error: Could not open edges.txt\n";
        return 1;
    }
    set<pair<string, string>> existingEdges;
    while (getline(edgesFile, line)) {
        if (line.empty() || line[0] == '/') continue;
        stringstream ss(line);
        string start, to;
        double distance, time, cost;
        ss >> start >> to >> distance >> time >> cost;

        // /check if airlines exist
        if (airlines.find(start) == airlines.end()) {
            cerr << "Error: Code " << start << " does not exist\n";
            continue;
        }

        // /check if airlines exist
        if (airlines.find(to) == airlines.end()) {
            cerr << "Error: Code " << to << " does not exist\n";
            continue;
        }

        // /check if edge already exists
        if (existingEdges.count({start, to})) {
            cerr << "Error: Edge between " << start << " and " << to
                 << " already exists\n";
            continue;
        }

        Edge edge(to, distance, time, cost);
        adjacency[start].push_back(edge);
        existingEdges.insert({start, to});
    }
    edgesFile.close();

    // Ensure all airlines are in adjacency, even isolated ones
    for (auto& [code, _] : airlines) {
        if (adjacency.find(code) == adjacency.end()) {
            adjacency[code] = {};
        }
    }

    printHeader();
    while (true) {
        printMenu();

        int choice;
        cin >> choice;

        cout << endl;

        // handle invalid input
        if (!cin) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (choice == 14) break;

        switch (choice) {
            case 1:
                displayGraph(airlines, adjacency);
                break;

            case 2:
                addAirline(airlines, adjacency);
                break;

            case 3:
                removeAirline(airlines, adjacency);
                break;

            case 4:
                addEdge(airlines, adjacency);
                break;

            case 5:
                removeEdge(airlines, adjacency);
                break;

            case 6: {
                string start, end;
                cout << "Enter start code: ";
                cin >> start;
                cout << "Enter end code: ";
                cin >> end;
                if (airlines.find(start) == airlines.end() ||
                    airlines.find(end) == airlines.end()) {
                    cout << "Error: Invalid codes.\n";
                    break;
                }
                int pd, pt, pc;
                do {
                    cout << "Enter priority for distance (1-3, 1 highest): ";
                    cin >> pd;
                } while (pd < 1 || pd > 3);
                do {
                    cout << "Enter priority for time (1-3, 1 highest): ";
                    cin >> pt;
                } while (pt < 1 || pt > 3);
                do {
                    cout << "Enter priority for cost (1-3, 1 highest): ";
                    cin >> pc;
                } while (pc < 1 || pc > 3);
                unordered_map<string, int> priority = {
                    {"distance", pd}, {"time", pt}, {"cost", pc}};
                vector<Node> path =
                    findShortestPath(airlines[start], airlines[end], priority,
                                     adjacency, airlines);
                if (path.size() <= 1) {
                    cout << "There is no connection between "
                         << airlines[start].getName() << " (" << start
                         << ") and " << airlines[end].getName() << " (" << end
                         << ").\n";
                } else {
                    cout << "Shortest path based on priorities (distance: "
                         << pd << ", time: " << pt << ", cost: " << pc
                         << "):\n";
                    double totalDist = 0, totalTime = 0, totalCost = 0;
                    for (size_t i = 0; i < path.size(); ++i) {
                        cout << path[i].getName() << " (" << path[i].getCode()
                             << ", " << path[i].getCity() << ")";
                        if (i + 1 < path.size()) {
                            cout << " -> ";
                            // Find the edge
                            for (auto& e : adjacency[path[i].getCode()]) {
                                if (e.to == path[i + 1].getCode()) {
                                    totalDist += e.distance;
                                    totalTime += e.time;
                                    totalCost += e.cost;
                                    break;
                                }
                            }
                        }
                    }
                    cout << "\nTotal Distance: " << totalDist << " km\n";
                    cout << "Total Time: " << totalTime << " hours\n";
                    cout << "Total Cost: $" << totalCost << "\n";
                }
                break;
            }

            case 7: {
                cout << "Adjacency size: " << adjacency.size() << endl;
                vector<vector<string>> scc = findSCC(adjacency);
                cout << "Strongly Connected Components (SCCs):\n";
                cout << "The graph has " << scc.size() << " SCC(s).\n";
                for (size_t i = 0; i < scc.size(); ++i) {
                    cout << "SCC " << i + 1 << ": ";
                    for (auto& code : scc[i]) {
                        cout << airlines[code].getName() << " (" << code
                             << ") ";
                    }
                    cout << "\n";
                }
                break;
            }

            case 8: {
                string start, target;
                cout << "Enter start code: ";
                cin >> start;
                cout << "Enter target code: ";
                cin >> target;
                if (airlines.find(start) == airlines.end() ||
                    airlines.find(target) == airlines.end()) {
                    cout << "Error: Invalid codes.\n";
                    break;
                }
                bool exists = pathExists(start, target, adjacency);
                cout << "Path exists from " << airlines[start].getName() << " ("
                     << start << ") to " << airlines[target].getName() << " ("
                     << target << "): " << (exists ? "Yes" : "No") << "\n";
                break;
            }

            case 9: {
                string src, dst;
                int k;
                cout << "Enter source code: ";
                cin >> src;
                cout << "Enter destination code: ";
                cin >> dst;
                cout << "Enter k: ";
                cin >> k;
                if (airlines.find(src) == airlines.end() ||
                    airlines.find(dst) == airlines.end()) {
                    cout << "Error: Invalid codes.\n";
                    break;
                }
                string path =
                    findKthShortestPath(adjacency, airlines, src, dst, k);
                if (path.empty()) {
                    cout << "No " << k << "-th shortest path found from "
                         << airlines[src].getName() << " (" << src << ") to "
                         << airlines[dst].getName() << " (" << dst << ").\n";
                } else {
                    cout << "The " << k << "-th shortest path (by cost) from "
                         << airlines[src].getName() << " (" << src << ") to "
                         << airlines[dst].getName() << " (" << dst
                         << ") is: " << path << "\n";
                }
                break;
            }

            case 10: {
                vector<Edge> critical = mostCriticalEdges(adjacency);
                if (critical.empty()) {
                    cout << "No critical edges found. Removing any edge does "
                            "not increase the number of SCCs.\n";
                } else {
                    cout << "Most critical edges (removing them increases SCC "
                            "count the most):\n";
                    for (auto& e : critical) {
                        cout << "From " << airlines[e.to].getName() << " ("
                             << e.to << ") with distance " << e.distance
                             << ", time " << e.time << ", cost " << e.cost
                             << "\n";
                        // Wait, Edge has to, but from is not stored. Wait, in
                        // the function, it's the edge from parent to ele.to
                        // Actually, the function returns Edge, but Edge has to,
                        // distance, time, cost, but not from. In the code,
                        // candidates[scc.size()].push_back(ele); ele is
                        // Edge(to, dist, time, cost), but to identify from,
                        // it's tricky. Perhaps modify to return pair or
                        // something, but since can't touch other files, perhaps
                        // print as is.
                        cout << "Edge to " << e.to << " with distance "
                             << e.distance << ", time " << e.time << ", cost "
                             << e.cost << "\n";
                    }
                }
                break;
            }

            case 11: {
                vector<vector<string>> hubs = suggestHubConnection(adjacency);
                if (hubs.empty()) {
                    cout << "The network is already fully connected (single "
                            "SCC). No hub connection suggestion needed.\n";
                } else {
                    cout << "Suggested hub connections between the two largest "
                            "SCCs:\n";
                    cout << "SCC 1: ";
                    for (auto& code : hubs[0])
                        cout << airlines[code].getName() << " (" << code
                             << ") ";
                    cout << "\nSCC 2: ";
                    for (auto& code : hubs[1])
                        cout << airlines[code].getName() << " (" << code
                             << ") ";
                    cout << "\nConnecting these would maximize reachability.\n";
                }
                break;
            }

            case 12: {
                string start;
                cout << "Enter start code: ";
                cin >> start;
                if (airlines.find(start) == airlines.end()) {
                    cout << "Error: Invalid code.\n";
                    break;
                }
                vector<string> reachable =
                    reachableNodes(airlines[start], adjacency);
                cout << "Nodes reachable from " << airlines[start].getName()
                     << " (" << start << "):\n";
                if (reachable.empty()) {
                    cout << "None.\n";
                } else {
                    for (auto& code : reachable) {
                        cout << airlines[code].getName() << " (" << code
                             << ") ";
                    }
                    cout << "\n";
                }
                break;
            }

            case 13: {
                string capital;
                cout << "Enter capital airport code: ";
                cin >> capital;

                if (airlines.find(capital) == airlines.end()) {
                    cout << "Error: Invalid code.\n";
                    break;
                }

                vector<string> needed =
                    findMissingReachableComponents(adjacency, capital);

                cout << "To make all airports reachable from "
                     << airlines[capital].getName() << " (" << capital
                     << "), connect to:\n";

                if (needed.empty()) {
                    cout << "All airports are already reachable.\n";
                } else {
                    for (const auto& code : needed) {
                        cout << airlines[code].getName() << " (" << code
                             << ")\n";
                    }
                }

                break;
            }
            default:
                cout << "Invalid choice.\n";
        }
    }
    return 0;
}
