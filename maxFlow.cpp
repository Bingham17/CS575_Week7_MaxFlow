#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
using namespace std;

/*
Name: Christopher Bingham
Email: cbingha2@binghamton.edu
Assignment: Week 7 - Edmonds-Karp (Max Flow) Algorithm
*/


//INFINITY Global Constant
const int GRAPH_SIZE = 100000;
class Edge;
class Node;
class Graph;

class Node {
    /*
    A graph's node
    Attributes:
        id <int>: Specific node indentity
        visited <int>: Flag whether node has been visited
        edges <vector<Edges>>: All outgoing edges from the node
        residualEdge <Edge*>: Edge we came from; Used for backtracking 
    */
    public:
        int id;
        int visited;
        Edge *edges;
        Edge *residualEdge;

};

class Edge {
    /*
    Edge of a graph from one node to another
    Attributes:
        from <Node*>: Start edge node
        to <Node*> End edge node
        capacity <int>: Capacity of the edge
        next <Edge*>: Next edge linked
        reverse <Edge*>: Reverse edge linked
    */
    public:
        Node *from;
        Node *to;
        int capacity;
        Edge *next;
        Edge *reverse;
};

class Graph {
    /*
    Graph representation for max flow
    Attributes:
        numNodes <int>: Number of nodes in graph
        numMen, numWomen <int>: Number of men/women used in bipartite graph
        nodes <vector<Node>>: Vector of all graph nodes
        start <Node*>: Pointer to the start node
        finish <Node*>: Pointer to the finishing node*/
    public:
        int numNodes;
        int numMen, numWomen;
        Node *nodes;
        Node *start;
        Node *finish;
};

void addEdge(Graph *graph, Node *n1, Node *n2, int cap) {
    /*
    Add the directed forward edge and setj up the reverse edge for backtracking
    Parameters:
        graph <Graph*>: Graph to add the edge
        n1 <Node*>: Start edge Node
        n2 <Node*>: End edge Node
        cap <int>: Edge capacity
    Returns:
        None
    */
    Edge *forwardEdge;
    Edge *backEdge;

    forwardEdge = (Edge*)malloc(sizeof(Edge));
    backEdge = (Edge*)malloc(sizeof(Edge));

    forwardEdge->from = n1;
    forwardEdge->to = n2;
    forwardEdge->capacity = cap;
    forwardEdge->next = n1->edges;
    n1->edges = forwardEdge;

    backEdge->from = n2;
    backEdge->to = n1;
    backEdge->capacity = 0;
    backEdge->next = n2->edges;
    n2->edges = backEdge;

    forwardEdge->reverse = backEdge;
    backEdge->reverse = forwardEdge;

}

Graph *readInputFile(ifstream &inputFile) {
    /*
    Description
    */
    Graph *graph;
    Edge *edge;
    int numNodes, numEdges;
    int fromNode, toNode, cap;
    int i;

    inputFile >> numNodes;
    inputFile >> numEdges;

    graph = (Graph*)malloc(sizeof(Graph));

    graph->numNodes = numNodes;
    graph->nodes = (Node*)malloc(numNodes * sizeof(Node));

    for (i = 0; i < numNodes; i++) {
        graph->nodes[i].id = i;
        graph->nodes[i].edges = NULL;
    }

    for (i = 0; i < numEdges; i++) {
        inputFile >> fromNode;
        inputFile >> toNode;
        inputFile >> cap;
        addEdge(graph, &graph->nodes[fromNode], &graph->nodes[toNode], cap);
    }

    graph->start = &graph->nodes[0];
    graph->finish = &graph->nodes[numNodes - 1];

    return graph;
}

int bfs(Graph *graph, Edge **path) {
    /*
    Description
    */
    int i;
    Node *nodeQueue[GRAPH_SIZE];
    int qStart, qEnd;

    for(i = 0; i < graph->numNodes; i++) {
        graph->nodes[i].visited = 0;
    }

    qStart = qEnd = 0;

    graph->nodes[0].visited = 1;
    graph->nodes[0].residualEdge = NULL;
    nodeQueue[qEnd++] = &graph->nodes[0];

    while((qStart != qEnd) && (graph->finish->visited == 0)) {
        Node *node = nodeQueue[qStart++];
        Edge *edge = node->edges;

        while(edge != NULL) {
            if((edge->capacity) > 0 && (edge->to->visited == 0)) {
                edge->to->visited = 1;
                edge->to->residualEdge = edge;
                nodeQueue[qEnd++] = edge->to;
            }
            edge = edge->next;
        }
    }

    if(graph->finish->visited) {
        Edge *edge;
        Node *node;

        node = graph->finish;

        i = 0;
        do {
            path[i++] = edge = node->residualEdge;
            if(edge != NULL) {
                node = edge->from;
            }
        } while(edge != NULL);

        return 1;
    }
    
    return 0;
}

int pathCapacity(Graph *graph, Edge **path) {
    /*
    Description
    */
    int i;
    int minCapacity = -1;

    for(i = 0; path[i] != NULL; i++) {
        if(minCapacity == -1 || path[i]->capacity < minCapacity) {
            minCapacity = path[i]->capacity;
        }
    }

    for(i = 0; path[i] != NULL; i++) {
        path[i]->capacity -= minCapacity;
        path[i]->reverse->capacity += minCapacity;
    }

    return minCapacity;
}

int edmondsKarp(Graph *graph) {
    /*
    Description
    */
   Edge *path[1024];
   int totalCapacity = 0;

   while(bfs(graph, path)) {
        int cap;
        cap = pathCapacity(graph, path);
        totalCapacity += cap;
   }

   return totalCapacity;
}




int main(int argc, char *argv[]) {
    /*
    Main function to execute the Edmonds-Karp (Max Flow) algorithm.
    CommandLine Usage:
        - ./maxFlow <input_file>
    Command line arguments:
        - argv[1]: The name of the input file containing the integers to be sorted.
        - If no command line argument is provided, the user will be prompted to enter the file name.
    Returns:
        - 0: Successful execution
        - 1: Error in opening the input file
    */

    //Read the input file and build the vertices and edges
    ifstream inputFile;
    Graph *graph;
    int flow;

    if (argc != 2) {
        string fileName;
        cout << "Enter File Name: " << endl;
        cin >> fileName;
        inputFile.open(fileName);
        if(!inputFile) {
            cout << "Cannot Open File: " << fileName << endl;
            cout << "Commandline Use: ./maxFlow <inputFile>" << endl;
            return 1;
        }
            graph = readInputFile(inputFile);
            flow = edmondsKarp(graph);
    } else {
            inputFile.open(argv[1]);
            if(!inputFile) {
                cout << "Cannot Open File: " << argv[1] << endl;
                cout << "Commandline Use: ./maxFlow <inputFile>" << endl;
                return 1;
            }
            cout << "File Opened: " << argv[1] << endl;
            graph = readInputFile(inputFile);
            cout << "Graph Read" << endl;
            flow = edmondsKarp(graph);
            cout << "Max Flow Calculated" << endl;
        }

    cout << "Max Flow: " << flow << endl;

    return 0;

}