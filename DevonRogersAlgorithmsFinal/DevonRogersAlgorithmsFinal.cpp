// DevonRogersAlgorithmsFinal.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream> //used for console interaction
#include <vector> //used for data storage
#include <string> //used for the strings
#include <sstream> //used to split strings into smaller strings
#include <fstream> //used to read files
#include <chrono> //used for timing

using namespace std;

//convert a given character into an index
int charToID(char in) {
	in = toupper(in);
	int out = in - 65; //65 is the character code for a capital A; since the nodes are stored in alphabetical order, index 0 = node A, so you can remove 65 from the node ID to get the index
	return out;
}

//convert a given index to a character
char IDToChar(int in) {
	char out = in + 65; //65 is the character code for a capital A; since the nodes are stored in alphabetical order, index 0 = node A, so you can add 65 to the index to get the Node ID
	return out;
}

struct node {
	char id; //the id of the node
	bool isChargingStation; //whether or not a given node is a charging station
	int shortestDistFromSource = 999999; //the shortest distance from the start point to this node
	//costs for unvisited nodes are initially a very high number
	char prevNode; //id of previous node; set at the same time as shortestDistFromSource
};

//the edges from one node to another and its weight
struct adjNode {
	char id; // the end point of an edge
	int cost; // the cost to traverse the edge

	adjNode* link; //the next edge in alphabetical order by destination
};

//adjacency list
adjNode* adjacencyList[23];
//list of nodes
node* networkGraph[23];

//return whether the passed node contains a charger
bool checkIsCharger(char nodeID) {
	//check whether the passed in nodes is one of the ones with a charger
	if (nodeID == 'H' || nodeID == 'K' || nodeID == 'Q' || nodeID == 'T') {
		//if it is, make it a charging station
		return true;
	}
	else {
		//if it isn't, make it not a charging station
		return false;
	}
}
//initializaton of pointers
adjNode* adjFirst, * adjLast, * newAdj, * adjCurr;
node* newNode;

//load the graph from the external file
bool loadNetwork() {

	string networkDat; //the current line that has been pulled from the file
	ifstream networkFile("DevonRogersAlgorithmsFinalSpreadsheet.csv"); //the file

	//if the file is found, open it
	if (networkFile.ifstream::is_open()) {

		int currentLine = 0; //the current line of the document, 0-indexed

		while (getline(networkFile, networkDat)) {

			newNode = new node; // make a new node object

			vector<string> tempArray; //holds the different sections of a line pulled from the file
			vector<char> adjTemp;     //holds the neighbors of the current node, taken from the second section of the current line of the file; stands for "adjacent temp"
			vector<int> costTemp;     //holds the costs of visiting the neighbors of the vurrent node, taken from the third section of the current line of the file; stands for "cost temp"

			std::istringstream iss(networkDat);
			string s;
			while (std::getline(iss, s, ',')) {
				tempArray.push_back(s); //separate the sections of the current line of the file at the commas, then put them into tempArray

			}

			std::istringstream adjNodes(tempArray[1]); //the second section of the current line
			string adj; //the string taken out of the second section of the current line; stands for "adjacent"
			while (std::getline(adjNodes, adj, ';')) {
				adjTemp.push_back(adj[0]); //separate the second section of the current line of the file at the semicolons, then put them into adjTemp
			}

			std::istringstream adjCosts(tempArray[2]); //the third section of the current line
			string w; //the string taken out of the third section of the current line; stands for "weight"
			while (std::getline(adjCosts, w, ';')) {
				costTemp.push_back(stoi(w)); //separate the thid section of the current line of the file at the semicolons, then put them into costTemp
			}

			newNode->id = tempArray[0][0]; //set the id of newNode to the first section of the current line of the file
			newNode->isChargingStation = checkIsCharger(tempArray[0][0]); //set isChargingStation equal to the return value

			networkGraph[currentLine] = newNode; //add newNode to the networkGraph array at index "currentLine"

			//for each neihbouring node:
			for (int i = 0; i < adjTemp.size();i++) {
				newAdj = new adjNode; //make a new adjacency node

				newAdj->id = adjTemp[i];    //add the ID of the neighboring node to the new adjacency node
				newAdj->cost = costTemp[i]; //add the traversal cost to the new adjacency node

				newAdj->link = nullptr; //set the link of the new adjacency node to a null pointer

				//from page 274/275 of the Malik C++ textbook
				//if adjFirst is a null pointer, make it and adjLast equal to the new adjacency node
				if (adjFirst == nullptr) {
					adjFirst = newAdj;
					adjLast = newAdj;
				}
				else {
					//otherwise, set the link of the adjNode that adjLast points to to newAdj and update adjLast
					adjLast->link = newAdj;
					adjLast = newAdj;
				}
			}
			//set the pointer at index currentLine of the array adjacencyList to equal adjFirst
			adjacencyList[currentLine] = adjFirst;

			//reset adjFirst and adjLast to a null pointer to prevent memory leakage due to reused pointers
			adjFirst = nullptr;
			adjLast = nullptr;

			//advance the value of currentLine
			currentLine++;
		}
	}
	else {
		//if no file is found, notify the user
		cout << "Network file not found!\n";
		return false;
	}
	networkFile.close(); //close the network file when you're done with it to save resources
	return true;
}

//dijkstra's algorithm;
void dijkstrasAlgorithm(char startPoint) {
	cout << "Algorithm started, please wait...\n";                 //notify the user that the algorithm has started
	std::chrono::time_point<std::chrono::system_clock> start, end; //initialze timer
	start = std::chrono::system_clock::now();                      //start a stopwatch

	//create vectors with visited and unvisited nodes (follow Lecture 9)
	vector<char> visited;   //the set of visited vertices (initially empty)
	vector<char> unvisited; //the queue

	for (int i = 0; i < 23; i++) {
		//initialize the vector of unvisited nodes
		unvisited.push_back(i + 65); //the queue initially contains all vertices

		if (IDToChar(i) == startPoint) {
			networkGraph[i]->shortestDistFromSource = 0; //distance to source vertex is 0
		}
		else {
			networkGraph[i]->shortestDistFromSource = 999999; // set all other distances to 999999 (a ridiculously high number, because INFINITY wasn't cooperating
		}
	}
	//while the queue is not empty
	while (!unvisited.empty()) {

		//select the element of [unvisited] with the min. distance
		int nextEle = NULL; //the index of "unvisited" that the next element resides at
		int currMinDist = 999999; //set the current minimum distance to be very high
		for (int t = 0; t < unvisited.size(); t++) {
			int distTest = networkGraph[charToID(unvisited[t])]->shortestDistFromSource;
			if (distTest < currMinDist) {
				currMinDist = distTest;
				nextEle = t;
			}
		}

		//add [nextEle] to the list of visited vertices
		visited.push_back(unvisited[nextEle]);

		//using a while loop, check each neighbor of the current element via adjacencyList
		adjCurr = adjacencyList[charToID(unvisited[nextEle])];
		while (adjCurr != nullptr) {

			//how the below if statement works: 
			//if shortestDistFromSource of the node with the ID within adjCurr is greater than the shortestDistFromSource of the node indicated by nextEle
			// plus the cost within adjCurr, set the shortestDistFromSource within the node with the id within adjCurr to to shortestDistFromSource of the node indicated by nextEle
			// plus the cost within adjCurr and set the prevNode of the node referenced by adjCurr to the node referenced by nextEle

			//if new shortest path found
			if (networkGraph[charToID(adjCurr->id)]->shortestDistFromSource > networkGraph[charToID(unvisited[nextEle])]->shortestDistFromSource + adjCurr->cost) {
				//set new value of shortest path
				networkGraph[charToID(adjCurr->id)]->shortestDistFromSource = networkGraph[charToID(unvisited[nextEle])]->shortestDistFromSource + adjCurr->cost;
				//update the previous vertex
				networkGraph[charToID(adjCurr->id)]->prevNode = networkGraph[charToID(unvisited[nextEle])]->id;

			}
			//advance pointer
			adjCurr = adjCurr->link;
		}

		//we won't visit [nextEle] again
		unvisited.erase(unvisited.begin() + nextEle);

	}

	end = std::chrono::system_clock::now(); //stop the stopwatch

	std::chrono::duration<double> elapsed_seconds = end - start; //calculate how long the stopwatch ran for

	cout << "Algorithm finished! Time taken: " << elapsed_seconds.count() << "s.\n"; //print how long the stopwatch ran for
}

//prints all data from the second and third columns of the csv file; for debug use only
void printData() {

	for (int i = 0; i < 23;i++) {
		//set adjCurr to the pointer at the current index of adjacencyList
		adjCurr = adjacencyList[i];

		//while there are still neighbors to print
		while (adjCurr != NULL) {
			cout << adjCurr->id << ", " << adjCurr->cost << endl; //print the node that is connected to and the cost
			adjCurr = adjCurr->link; //proceed to the next neighbor
		}
		cout << endl;
	}
}

//prints the minimum distance from the start point to each charging station; the stations are: H, K, Q, T
void printStationCosts(char start) {
	cout << "Shortest distance to each station from Node " << start << ": \n";
	cout << "H: " << networkGraph[charToID('H')]->shortestDistFromSource << endl;
	cout << "K: " << networkGraph[charToID('K')]->shortestDistFromSource << endl;
	cout << "Q: " << networkGraph[charToID('Q')]->shortestDistFromSource << endl;
	cout << "T: " << networkGraph[charToID('T')]->shortestDistFromSource << endl;
	cout << endl;
}

void printMostEfficientRoute() {
	char mostEfficient = 'H'; //the first charging station
	char stations[] = { 'K', 'Q', 'T' }; //the other charging stations
	for (int n = 0; n < 3; n++) {
		//if the shortest distance from the source to the current most efficient is greater than that of the station at index n in stations, set mostEfficient to the station at index n
		int netIndex = charToID(stations[n]);
		if (networkGraph[charToID(mostEfficient)]->shortestDistFromSource > networkGraph[netIndex]->shortestDistFromSource) {
			mostEfficient = stations[n];
		}
	}

	vector<char> mostEfficientRoute; //the most efficient route to take, from end to start
	mostEfficientRoute.push_back(mostEfficient); //add the closest station
	int distCheck = networkGraph[charToID(mostEfficient)]->shortestDistFromSource; //set distCheck to the distance from the charging station to the starting point
	//while you have not yet reached the start point
	while (distCheck > 0) {
		char curr = mostEfficientRoute.back(); //set the current node to the last one in the list
		char next = networkGraph[charToID(curr)]->prevNode; //set the next node to the node before the current node 

		mostEfficientRoute.push_back(next); //add the next node to the most efficient route

		distCheck = networkGraph[charToID(next)]->shortestDistFromSource; //update distCheck
	}

	cout << "Most efficient route: ";
	//print the most efficient route. since the vector is backwards, print it from back to front.
	for (auto it = mostEfficientRoute.rbegin(); it != mostEfficientRoute.rend(); it++) {
		if (it != mostEfficientRoute.rbegin()) {
			cout << " -> "; // if this is the first loop, do not print an arrow
		}
		cout << *it;
	}
	cout << endl;
}

int main()
{
	//whether or not the csv file has successfully loaded
	bool isLoaded = false;

	//load network from file
	isLoaded = loadNetwork();

	//debug: print the network
	//printData();

	bool running = true;
	//while the user is using the program and the file has successfully loaded:
	while (running && isLoaded) {
		//get start point from user
		cout << "Please enter the starting point: ";
		char ans;
		cin >> ans;

		ans = toupper(ans);
		//if the user does not input a valid node
		if (ans < 'A' || ans > 'W') {
			cout << "That's is not a valid node! Valid nodes are anything from A to W.\n";
			continue; //restart the loop
		}

		//run djikstra's algorithm
		dijkstrasAlgorithm(ans);

		//print the cost to travel from the starting node to each charging station
		printStationCosts(ans);

		//print the most efficient route
		printMostEfficientRoute();

		//if the user is done, end the program, otherwise loop
		cout << "Would you like to test another starting point? Y/N ";
		cin >> ans;
		if (toupper(ans) != 'Y') {
			running = false;
		}
	}
	//if the file successfully loaded, print "see you next time"
	if (isLoaded) {
		std::cout << "See you next time!\n";
	}
	else {
		//otherwise, ask the user to make sure that the csv file is accessible and then end the program
		cout << "Please make sure that DevonRogersAlgorithmsFinalSpreadsheet.csv exists and try again.\n";
	}
}


