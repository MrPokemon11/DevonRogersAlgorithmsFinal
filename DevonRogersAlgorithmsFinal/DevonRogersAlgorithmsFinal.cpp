// DevonRogersAlgorithmsFinal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <map> // unneeded? (remove line if unused, otherwise remove this comment)

using namespace std;

//convert a given character into an index
int charToID(char in) {
    in = toupper(in);
    int out = in - 65;
    return out;
}

// \/ potentially unneeded? /\

//convert a given index to a character
char IDToChar(int in) {
    char out = in + 65;
    return out;
}




struct node {
    char id;
    //the character A is 65; make nodes using for loop
    bool isChargingStation;
    int shortestDistFromSource = -1;//shortest distance from source; make sure that the source is equal to 0!
        //costs for unvisited nodes are initially -1, and later set to infinity
    char prevNode; //previous node; set at the same time as shortestDistFromSource
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

//useless?
struct network {
    char ID;

    vector<char> connections;
    vector<int> weights;

    network* link;
};
//if the network struct goes unused remove it (it probably will go unused)

//adjacency list
adjNode *adjacencyList[23];
//list of nodes
node* graph[23];

//return whether the passed node contains a charger
bool checkIsCharger(char nodeID) {
    //set nodeID to the ID that was passed in
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
node* newNode;

bool loadNetwork() {

    string networkDat;
    ifstream networkFile("DevonRogersAlgorithmsFinalSpreadsheet.csv");

    if (networkFile.ifstream::is_open()) {
        //do file loading

        int currentLine = 0;

        while (getline(networkFile,networkDat)) {

            newNode = new node;

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

            newNode->id = tempArray[0][0];
            newNode->isChargingStation = checkIsCharger(tempArray[0][0]);

            graph[currentLine] = newNode;

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
        cout << "Network file not found!\n";
        return false;
    }
    return true;
}

//reminder to self: look at the pseudocode in lecture 10, should be very helpful

//dijkstra's algorithm; i just don't want to type that every time
void djAlgorithm(char startPoint) {
    //create vectors with visited and unvisited nodes (follow Lecture 9)
    vector<char> visited; //the set of visited vertices (initially empty)
    vector<char> unvisited; //the queue

    for (int i = 0; i < 23; i++) {
        //initialize the vector of unvisited nodes
        unvisited.push_back(i + 65); //the queue initially contains all vertices

        if (i + 65 == startPoint) {
            graph[i]->shortestDistFromSource = 0; //distance to source vertex is 0
        }
        else {
            graph[i]->shortestDistFromSource = INFINITY; // set all other distances to infinity (aka -1)
        }
    }
    //while the queue is not empty
    while (!unvisited.empty()) {

        //select the element of [unvisited] with the min. distance
        int nextEle;
        int currMinDist = INFINITY;
        for (auto t : unvisited) {
            int distTest = graph[unvisited[t] - 65]->shortestDistFromSource;
            if (distTest < currMinDist) {
                currMinDist = distTest;
            }
        }


    }

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

//prints all data from the csv file
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

//prints the minimum distance from the start point to each charging station; the stations are: H, K, Q, T
void printStationCosts() {
    cout << "Shortest distance to each station: \n";
    cout << "H: " << graph['H' - 65]->shortestDistFromSource << endl;
    cout << "K: " << graph['K' - 65]->shortestDistFromSource << endl;
    cout << "Q: " << graph['Q' - 65]->shortestDistFromSource << endl;
    cout << "T: " << graph['T' - 65]->shortestDistFromSource << endl;
}

int main()
{
    bool isLoaded = false;

    //load network
    isLoaded = loadNetwork();

    //debug: print the network
    printData();


    bool running = true;
    while (running && isLoaded) {
     //get source node
        cout << "Please enter the starting point: ";
        char ans;
        cin >> ans;

        ans = toupper(ans);
        if (ans < 'A' || ans > 'W') {
            cout << "That's is not a valid node! Valid nodes are anything from A to W.\n";
            continue;
        }

        //note: if the source node *is* a charging station, you still need to find the distance to the other 3 '-_-

        //run djikstra's algorithm
        djAlgorithm(ans);

        //if the user is done, end the program
        cout << "Would you like to test another starting point? Y/N ";
        cin >> ans;
        if (toupper(ans) != 'y') {
            running = false;
        }
    }
    if (isLoaded) {
        std::cout << "See you next time!\n";
    }
    else {
        cout << "Make sure that DevonRogersAlgorithmsFinalSpreadsheet.csv exists and please try again.\n";
    }

}

/* Personal notes
* - optimal path output "pseudocode"
*   - make vector of characters
*   - push_back working from most optimal charging station to start point
*   - print vector from back to front
* 
* - conflicted: have prevNode be a reference or a character?
*   - in favor: already making an array that contains pointers
* 
* ... conflict ended
   

*/
