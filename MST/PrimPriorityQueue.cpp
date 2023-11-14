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

typedef pair<int, int> intPair;
class graph{
    vector<vertex> vertices;
    int numberOfVertices;
    int numberOfEdges;
    vector<vector<int>> weightsofEdges;

    //For application of Prim's algorithm
    vector<bool> msTree; vector<int> keys;
    priority_queue<intPair, vector<intPair>, greater<intPair>> keyQueue;
    //for the queue, the FIRST value is the KEYVALUE, SECOND is the ID;
    //Setting the keyValue first so the p.queue will be sorted according to it

public:
    graph(int V, int E){
        numberOfVertices = V;  numberOfEdges = E;
        for(int i=0; i<numberOfVertices; i++){ vertices.push_back({i, numberOfVertices}); }
        weightsofEdges.resize(numberOfVertices, vector<int>(numberOfVertices, 0));
    }//Constructor

    void setAdjacent(int u, int v, int weight){
        //Undirected..
        vertices[u].setAdjacent(v);    vertices[v].setAdjacent(u);
        weightsofEdges[u][v] = weight; weightsofEdges[v][u] = weight;
    }//function

    //Function for Prim's algorithm
    void primMST(int root){
        msTree.resize(numberOfVertices, false);
        vector<int> parents(numberOfVertices); parents[root] = -1;

        keyQueue.push(make_pair(0, root)); keys.push_back(0);
        for(int i=0; i<numberOfVertices; i++)
            { keyQueue.push(make_pair(INT_MAX, i)); keys.push_back(INT_MAX); }

        int record = numberOfVertices; //For Loop Count
        int primWeight=0; //For Answers

        //This is not efficint to run the loop V times
        //Will fix it later to be O(ElogV)
        while(record){
            intPair minkeyNode = keyQueue.top();
            keyQueue.pop();
            while(msTree[minkeyNode.second]){ minkeyNode = keyQueue.top(); keyQueue.pop(); }

            msTree[minkeyNode.second] = true;
            primWeight += keys[minkeyNode.second];

            for(int i=0; i<numberOfVertices; i++){
                if(vertices[minkeyNode.second].checkAdjacent(i) && weightsofEdges[minkeyNode.second][i]<keys[i]){
                    // Updating current key values to lowest possible values
                    keys[i] = weightsofEdges[minkeyNode.second][i];
                    parents[i] = minkeyNode.second;
                    keyQueue.push(make_pair(keys[i], i));
                }
            }
            record--;
        } //Loop Ends

        // Output for Prim's File
        cout<<"Prim-Jarnik Algorithm:"<<endl<<"Total Weight = "<<primWeight<<endl;
        cout<<"Root Node = "<<root<<endl;
        for(int i=0; i<numberOfVertices; i++){
                if(i!=root) cout<<parents[i]<<"  "<<i<<endl;
        }
    }//MST Function
};

int main(void){
    ifstream instream("In.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    int V, E; instream>>V>>E;

    graph myGraph(V, E);
    int u, v, weight;
    for(int i = 0; i<E; i++){
        instream>>u>>v>>weight;
        myGraph.setAdjacent(u, v, weight);
    }

    int root = 0;
    myGraph.primMST(root);
    instream.close();
    return 0;
}

//g++ -o prim PrimPriorityQueue.cpp





