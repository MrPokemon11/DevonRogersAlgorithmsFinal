// DevonRogersAlgorithmsFinal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <map>

using namespace std;

//create vectors with visited and unvisited nodes (follow Lecture 9)

struct node {
    char id;
    //the character A is 65; make nodes using for loop
    bool isChargingStation;
    int shortestDistFromSource = -1;//shortest distance from source; make sure that the source is equal to 0!
        //costs for unvisited nodes are initially -1, aka infinity
    node* prevNode; //previous node; set at the same time as shortestDistFromSource
};

//the edges from one node to another and its weight
struct adjNode {
    char id;
    int cost;

    adjNode* link;
};

/*
Plan:
- have network be linked list of nodes?
    - 
*/

struct network {
    char ID;

    vector<char> connections;
    vector<int> weights;

    network* link;
};

//adjacency list
adjNode *adjacencyList[23];

bool checkIsCharger(char nodeID) {
    //set nodeID to the current node ID pulled from the stream
    if (nodeID == 'H' || nodeID == 'K' || nodeID == 'Q' || nodeID == 'T') {
        //make it a charging station
        return true;
    }
    else {
        //make it not a charging station
        return false;
    }
}

adjNode* adjFirst, * adjLast, * newAdj, *adjCurr;

void loadNetwork() {

    string networkDat;
    ifstream networkFile("DevonRogersAlgorithmsFinalSpreadsheet.csv");

    if (networkFile.ifstream::is_open()) {
        //do file loading

        int currentLine = 0;

        while (getline(networkFile,networkDat)) {

            //newAdj = new adjNode;

            vector<string> tempArray;
            vector<char> adjTemp;
            vector<int> costTemp;

            std::istringstream iss(networkDat);
            string s;
            while (std::getline(iss, s, ',')) {
                tempArray.push_back(s);

            }

            std::istringstream adjNodes(tempArray[1]);
            string adj;
            while (std::getline(adjNodes, adj, ';')) {
                adjTemp.push_back(adj[0]);
            }

            std::istringstream adjCosts(tempArray[2]);
            string w;
            while (std::getline(adjCosts, w, ';')) {
                costTemp.push_back(stoi(w));
            }


            for (int i = 0; i < adjTemp.size();i++) {
                newAdj = new adjNode;

                newAdj->id = adjTemp[i];
                newAdj->cost = costTemp[i];

                newAdj->link = nullptr;

                //from page 274/275 of the Malik C++ textbook
                if (adjFirst == nullptr) {
                    adjFirst = newAdj;
                    adjLast = newAdj;
                }
                else {
                    adjLast->link = newAdj;
                    adjLast = newAdj;
                }
            }

            adjacencyList[currentLine] = adjFirst;

            //keep this at the end of the while loop
            adjFirst = nullptr;
            adjLast = nullptr;

            currentLine++;
        }


    }
    else {
        cout << "Network not found!\n";
    }

}

//reminder to self: look at the pseudocode in lecture 10, should be very helpful

//dijkstra's algorithm; i just don't want to type that every time
void djAlgorithm() {
    int currCost = 0;//the total weights of the edges to reach the current node 
    int bestCost = -1;
    bool isUnvisited = false;
    //if a node is unvisited, make the shortest distance -1


    //check
    if (currCost < bestCost || isUnvisited) {
        bestCost = currCost;//since this is the shortest path so far, make it the best cost

        //update the value of prevNode
    }
}

//prints all data from the given line
void printData() {

    for (int i = 0; i < 23;i++) {

        adjCurr = adjacencyList[i];

        while (adjCurr != NULL) {
            cout << adjCurr->id << ", " << adjCurr->cost << endl;
            adjCurr = adjCurr->link;
        }
        cout << endl;
    }


}

int main()
{
    //load network
    loadNetwork();

    //debug: print the network
    printData();


    //get source node

    //if the source node *is* a charging station, you still need to find the distance to the other 3 '-_-

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
