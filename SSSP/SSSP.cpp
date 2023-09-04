#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class vertex{
    int id, numberOfVertices;
    vector<bool> adjacents;
public:
    vertex(int vid, int n){
        id = vid; numberOfVertices = n;
        adjacents.resize(n, false);
    }//Constructor
    int getId(){ return id; }//function
    void setAdjacent(int i){ adjacents[i] = true; }//function
    bool checkAdjacent(int v){ return adjacents[v]; }//function
};

class Edge{
    int from, to, weight;
public:
    Edge(){} //Empty Constructor
    Edge(int From, int To, int Weight){ from = From; to = To; weight = Weight; } //Constructor
    void setEdge(int From, int To, int Weight){ from = From; to = To; weight = Weight; } //function
    int getFrom() { return from; } //function
    int getTo(){ return to; } //function
    int getWeight() { return weight; } //function
};

typedef pair<int, int> intPair;
class graph{
    vector<vertex> vertices;
    int numberOfVertices, numberOfEdges;
    vector<vector<int>> weightsofEdges; //Better for Dijkstra
    vector<Edge> edges; //Better for Bellman-Ford

public:
    graph(int V, int E){
        numberOfVertices = V;  numberOfEdges = E;
        for(int i=0; i<numberOfVertices; i++) { vertices.push_back({i, numberOfVertices}); }
        weightsofEdges.resize(numberOfVertices, vector<int>(numberOfVertices, 0));
    }//Constructor

    void setAdjacent(int u, int v, int weight){
        // Directed Graph
        vertices[u].setAdjacent(v);
        weightsofEdges[u][v] = weight;
        edges.push_back({u, v, weight});
    }

    //Registering Dijkstra Algorithm
    void DijkstraSP(ofstream &outstream, int source, int destination){
        vector<bool> spTree(numberOfVertices, false);
        vector<int>  parents(numberOfVertices); parents[source] = -1;
        vector<int>  distances(numberOfVertices, INT_MAX);
        priority_queue< intPair, vector<intPair>, greater<intPair> > distQueue;
        //for the queue, the FIRST value is the WEIGHT, SECOND is the ID;

        distQueue.push(make_pair(0, source)); distances[source] = 0;
        for(int i=0; i<numberOfVertices; i++) { distQueue.push(make_pair(INT_MAX, i)); }

        int record = numberOfVertices; //For Loop Count
        while(record){

            // Extracting vertex with minimum key, unless the vertex is already visited
            intPair minkeyNode = distQueue.top(); distQueue.pop();
            // Next line is required since there can be multiple entries with same vertex in the queue
            while(spTree[minkeyNode.second]){ minkeyNode = distQueue.top(); distQueue.pop(); }

            spTree[minkeyNode.second] = true;
            for(int i=0; i<numberOfVertices; i++){
                if( vertices[minkeyNode.second].checkAdjacent(i) &&
                    (distances[minkeyNode.second]+abs(weightsofEdges[minkeyNode.second][i]))<distances[i] ){
                    // Updating current distance values to least possible values
                    distances[i] = distances[minkeyNode.second] + abs(weightsofEdges[minkeyNode.second][i]);
                    parents[i] = minkeyNode.second;
                    distQueue.push(make_pair(distances[i], i));
                }
            }
            record--;

        } //Loop Ends

        // Output for Dijkstra:
        outstream<<"Dijkstra Algorithm:"<<endl<<distances[destination]<<endl;
        // Route for Dijkstra:
        stack<int> route; int tempNode = destination;
        do{ route.push(tempNode);  tempNode = parents[tempNode];
        }while(tempNode!=source); route.push(source);
        while(!route.empty()){
            outstream<<route.top(); if(route.top()!=destination) outstream<<" -> ";
            route.pop();
        }outstream<<endl<<endl;

    }//Dijkstra


    //Registering Bellman-Ford's algorithm
    void BellmanFordSP(ofstream &outstream, int source, int destination){
        vector<int> distances(numberOfVertices, INT_MAX);
        vector<int> parents(numberOfVertices);
        distances[source] = 0; parents[source] = -1;

        // This is step gives us the shortest path considering negative weights
        for(int i=0; i<numberOfVertices-1; i++){
            for(int j=0; j<numberOfEdges; j++){
                if(distances[edges[j].getFrom()] != INT_MAX && ( distances[edges[j].getFrom()]+edges[j].getWeight() ) < distances[edges[j].getTo()] ){
                    distances[edges[j].getTo()] = distances[edges[j].getFrom()]+edges[j].getWeight();
                    parents[edges[j].getTo()] = edges[j].getFrom();
                }// Updating current distance values to least possible values
            }//Edge Loop
        }//Vertex-1 loop

        // If there is shorter path found than the one in the upper part, graph contains a negative cycle -> No definite shortest path
        for(int j=0; j<numberOfEdges; j++){
            if(distances[edges[j].getFrom()] != INT_MAX && ( distances[edges[j].getFrom()]+edges[j].getWeight() ) < distances[edges[j].getTo()] ){
                outstream<<"Bellman-Ford Algorithm:"<<endl<<"Negative Weight Cycle Present"<<endl<<endl;
                return;
            }
        } //Negative Cycle Loop

        // Output for Bellman-Ford
        outstream<<"Bellman-Ford Algorithm:"<<endl<<distances[destination]<<endl;
        // Route for Bellman-Ford(if exists):
        stack<int> route; int tempNode = destination;
        do{ route.push(tempNode);  tempNode = parents[tempNode];
        }while(tempNode!=source); route.push(source);
        while(!route.empty()){
            outstream<<route.top(); if(route.top()!=destination) outstream<<" -> ";
            route.pop();
        }outstream<<endl<<endl;
    }//Bellman-Ford Function
};

int main(void){
    ifstream instream("In.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("SSSP.txt");
    int V, E; instream>>V>>E;
    graph myGraph(V, E);

    int u, v, weight;
    for(int i = 0; i<E; i++){
        instream>>u>>v>>weight;
        myGraph.setAdjacent(u, v, weight);
    }
    int source, destination;
    instream>>source>>destination;

    myGraph.BellmanFordSP(outstream, source, destination);
    myGraph.DijkstraSP(outstream, source, destination);
    instream.close(); outstream.close();
    return 0;
}

//g++ -o sssp SSSP.cpp


