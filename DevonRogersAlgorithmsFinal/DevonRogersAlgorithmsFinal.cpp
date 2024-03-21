// DevonRogersAlgorithmsFinal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

//create vectors with visited and unvisited nodes (follow Lecture 9)

struct node {
    char id;
    //the character A is 65; make nodes using for loop
    bool isChargingStation;
    int shortestDistFromSource = -1;//shortest distance from source; make sure that the source is equal to 0!
        //costs for unvisited nodes are initially -1, aka infinity
    node* prevFromSource; //previous node; set at the same time as shortestDistFromSource
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



bool loadNetwork() {

    string networkDat;
    ifstream networkFile("DevonRogersAlgorithmsFinalSpreadsheet.csv");

    if (networkFile.ifstream::is_open()) {

    }
    else {
        cout << "Network not found!\n";
    }

}

//dijkstra's algorithm; i just don't want to type that every time
void djAlgorithm() {
    int currCost = 0;//the total weights of the edges to reach the current node 
    int bestCost = -1;
    bool isUnvisited = false;
    //if a node is unvisited, make the shortest distance -1


    //check
    if (currCost < bestCost || isUnvisited) {
        bestCost = currCost;//since this is the shortest path so far, make it the best cost
    }
}

int main()
{


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
