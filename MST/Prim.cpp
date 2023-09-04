#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class vertex{
    int id;
    int numberOfVertices;
    vector<bool> adjacents;

public:
    vertex(int vid, int n){
        id = vid;
        numberOfVertices = n;
        adjacents.resize(n, false);
    }//Constructor
    int getId(){ return id; }//function
    void setAdjacent(int i){ adjacents[i] = true; }//function
    bool checkAdjacent(int v){ return adjacents[v]; }//function
};

class graph{
    vector<vertex> vertices;
    int numberOfVertices;
    int numberOfEdges;
    vector<vector<int>> weightsofEdges;

    //For application of Prim's algorithm
    vector<bool> msTree;
    vector<int> keys;

public:
    graph(int V, int E){
        numberOfVertices = V;
        numberOfEdges = E;
        for(int i=0; i<numberOfVertices; i++){ vertices.push_back({i, numberOfVertices}); }
        weightsofEdges.resize(numberOfVertices, vector<int>(numberOfVertices, 0));
    }//Constructor

    void setAdjacent(int u, int v, int weight){
        //Undirected, so edge is true both ways..
        vertices[u].setAdjacent(v);    vertices[v].setAdjacent(u);
        weightsofEdges[u][v] = weight; weightsofEdges[v][u] = weight;
    }//function

    //Helper Function for Prim's Algorithm
    int minKeyIndex(vector<bool> &msTree, vector<int> &keys){
        int minKey = INT_MAX; int minkeyindex;
        for(int i=0; i<numberOfVertices; i++){
            if(!msTree[i] && keys[i]<minKey){
                minKey = keys[i];
                minkeyindex = i;
            }
        }
        return minkeyindex;
    }//function

    //Function for Prim's algorithm
    void primMST(ofstream &outstream ,int root){
        msTree.resize(numberOfVertices, false);
        keys.resize(numberOfVertices, INT_MAX); keys[root] = 0;
        vector<int> parents(numberOfVertices); parents[root] = -1;

        int record = numberOfVertices; //For Loop Count
        int primWeight=0; //For Answers

        while(record){
            int minkeyNode = minKeyIndex(msTree, keys);
            msTree[minkeyNode] = true;
            primWeight += keys[minkeyNode];

            for(int i=0; i<numberOfVertices; i++){
                if(vertices[minkeyNode].checkAdjacent(i) && weightsofEdges[minkeyNode][i]<keys[i]){
                    // Updating current key values to lowest possible values
                    keys[i] = weightsofEdges[minkeyNode][i];
                    parents[i] = minkeyNode;
                }
            }
            record--;
        } //Loop Ends

        // Output for Prim's File
        outstream<<"Prim-Jarnik Algorithm:"<<endl<<"Total Weight = "<<primWeight<<endl;
        outstream<<"Root Node = "<<root<<endl;
        for(int i=0; i<numberOfVertices; i++){
                if(i!=root) outstream<<parents[i]<<"  "<<i<<endl;
        }
    }//MST Function
};

int main(void){
    ifstream instream("In.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("prim.txt");
    int V, E; instream>>V>>E;

    graph myGraph(V, E);
    int u, v, weight;
    for(int i = 0; i<E; i++){
        instream>>u>>v>>weight;
        myGraph.setAdjacent(u, v, weight);
    }

    int root = 0;
    myGraph.primMST(outstream, root);
    instream.close(); outstream.close();
    return 0;
}


//g++ -o prim Prim.cpp


