// Distance Vector Routing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <limits>

using namespace std;

//Define a large number to represent infinity (for unreachable routes)
const int INF = 1e9;

// Router class to simulate individual router in the network
class Router {
public:
    string name; // router's name
    map<string, pair<int, string>> routingTable;
    map<string, int> neighbors; // maps neighbor name to direct cost

    // Constructor which sets the router's name and adds a self-route
    Router(string n) : name(n) {

        routingTable[n] = { 0, n };
    }

    // Add a direct neighbor and initialize routing table with direct connection
    void AddNeighbor(string neighborName, int cost) {
        neighbors[neighborName] = cost;
        routingTable[neighborName] = { cost, neighborName };
    }

    // Receive a routing table from a neighbor and update own table if shorter paths are found
    void receiveTable(const Router& neighbor) {
        for (const auto& entry : neighbor.routingTable) {
            string dest = entry.first;
            int newCost = entry.second.first + neighbors[neighbor.name];
            if (routingTable.find(dest) == routingTable.end() || newCost < routingTable[dest].first) {
                routingTable[dest] = { newCost, neighbor.name };
            }
        }
    }

    // Print the router table
    void printTable() {
        cout << "Routing Table for " << name << ":\n";
        cout << "Destination\tCost\tNext Hop\n";
        for (const auto& entry : routingTable) {
            cout << entry.first << "\t\t" << entry.second.first << "\t" << entry.second.second << endl;
        }
        cout << "--------------------------------\n";
    }
};
int main()
{
    // Create 4 routers and give them names
    Router R1("R!"), R2("R2"), R3("R3"), R4("R4");

    //Setup neighbors
    R1.AddNeighbor("R2", 1);
    R1.AddNeighbor("R4", 2);

    R2.AddNeighbor("R1", 1);
    R2.AddNeighbor("R3", 1);

    R3.AddNeighbor("R2", 1);
    R3.AddNeighbor("R4", 1);

    R4.AddNeighbor("R1", 2);
    R4.AddNeighbor("R3", 1);

    //store routers in vector
    vector<Router*> routers = { &R1, &R2, &R3, &R4 };

    //Run multiple rounds of updates
    const int rounds = 3;
    for (int round = 1; round <= rounds; ++round) {
        cout << "==== Round " << round << " ====\n\n";

        // Each router sends its table to its neighbors
        for (Router* sender : routers) {
            for (Router* receiver : routers) {
                if (sender->neighbors.find(receiver->name) != sender->neighbors.end()) {
                    receiver->receiveTable(*sender);
                }
            }
        }

        //print routing tables after each round
        for (Router* r : routers) {
            r->printTable();
        }
    }
    return 0;
}
