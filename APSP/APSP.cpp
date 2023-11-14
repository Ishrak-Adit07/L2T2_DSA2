#include <bits/stdc++.h>
#include <fstream>
using namespace std;

//Helper Functions
float min(float a, float b) { return a < b ? a : b; }

class vertex{
    int id;
    int numberOfVertices;
    vector<bool> adjacents;
    //vector<float> edgeWeights;

public:
    vertex(int vid, int n){
        id = vid;
        numberOfVertices = n;
        adjacents.resize(numberOfVertices, false);
        //edgeWeights.resize(numberOfVertices, INT_MAX);
    }

    int getId(){ return id; }
    void setAdjacent(int i){ adjacents[i] = true; }
    bool checkAdjacent(int v){ return adjacents[v]; }

};

class Edge{
    int from, to;
    float weight;
public:
    Edge(){} //Empty Constructor
    Edge(int From, int To, float Weight){ from = From; to = To; weight = Weight; } //Constructor
    void setEdge(int From, int To, float Weight){ from = From; to = To; weight = Weight; } //function
    int getFrom() { return from; } //function
    int getTo(){ return to; } //function
    float getWeight() { return weight; } //function
};

class graph{
    vector<vertex> vertices;
    int numberOfVertices;
    int numberOfEdges;

    vector<Edge> edges;
public:
    graph(int V, int E){
        numberOfVertices = V;
        numberOfEdges = E;
        for(int i=0; i<numberOfVertices; i++){ vertices.push_back({i, numberOfVertices}); }
    }

    void setAdjacent(int u, int v, float weight){
        //Directed Graphs
        vertices[u].setAdjacent(v);
        edges.push_back({u, v, weight});
    }

    void matMultiply(ofstream &outstream){
        vector<vector<float>> Matrix, tempMatrix;
        Matrix.resize(numberOfVertices, vector<float>(numberOfVertices));
        tempMatrix.resize(numberOfVertices, vector<float>(numberOfVertices));

        //Setting Initial Values
        for(int i=0; i<numberOfVertices; i++){
            for(int j=0; j<numberOfVertices; j++){
                if(i==j) Matrix[i][j] = 0;
                else Matrix[i][j] = INT_MAX;
            }
        }
        for(auto edge: edges) Matrix[edge.getFrom()][edge.getTo()] = edge.getWeight();

        //Doing the multiplication log2(V) times
        int log2_V = ceil(log2(numberOfVertices));
        //Matrix Multiplication
        for(int edgeAllowed=0; edgeAllowed<log2_V; edgeAllowed++){

            /*//INTERMEDIATE Matrix Display
            for(int p=0; p<Matrix.size(); p++){
                for(int q=0; q<Matrix[p].size(); q++){
                    if(Matrix[p][q] == INT_MAX) outstream<<"INF"<<" ";
                    else outstream<<Matrix[p][q]<<" ";
                }outstream<<endl;
            }outstream<<endl<<endl;*/

            for(int i=0; i<numberOfVertices; i++){
                for(int j=0; j<numberOfVertices; j++){

                    tempMatrix[i][j] = INT_MAX;
                    for(int k=0; k<numberOfVertices; k++)
                        tempMatrix[i][j] = min( tempMatrix[i][j], (Matrix[i][k]+Matrix[k][j]) );

                }//Column-j
            }//Row-i

            //Setting original matrix to new matrix
            for(int i=0; i<numberOfVertices; i++)
                for(int j=0; j<numberOfVertices; j++)
                    Matrix[i][j] = tempMatrix[i][j];

        }//Edge allowed

        //Matrix Display
        outstream<<"Shortest distance Matrix(Matrix Multiplication): "<<endl;
        for(int i=0; i<Matrix.size(); i++){
            for(int j=0; j<Matrix[i].size(); j++){
                if(Matrix[i][j] == INT_MAX) outstream<<"INF"<<" ";
                else outstream<<Matrix[i][j]<<" ";
            }outstream<<endl;
        }outstream<<endl<<endl;
    }//Matrix Multiplication function

    //Floyd-Warsall Function
    void floydWarshall(ofstream &outstream){
        vector<vector<float>> Matrix;
        Matrix.resize(numberOfVertices, vector<float>(numberOfVertices));

        // Setting Initial Values
        for(int i=0; i<numberOfVertices; i++){
            for(int j=0; j<numberOfVertices; j++){
                if(i==j) Matrix[i][j] = 0; //Self Distance
                else Matrix[i][j] = INT_MAX;
            }
        }
        for(auto edge: edges) Matrix[edge.getFrom()][edge.getTo()] = edge.getWeight();

        //Floyd-Warshall
        for(int k=0; k<numberOfVertices; k++){

            for(int i=0; i<numberOfVertices; i++){
                for(int j=0; j<numberOfVertices; j++){
                    Matrix[i][j] = min(Matrix[i][j], (Matrix[i][k] + Matrix[k][j]));
                }//Column-j
            }//Row-i

        }

        //Matrix Display
        outstream<<"Shortest distance Matrix(Floyd-Warshall Algorithm): "<<endl;
        for(int i=0; i<Matrix.size(); i++){
            for(int j=0; j<Matrix[i].size(); j++){
                if(Matrix[i][j] == INT_MAX) outstream<<"INF"<<" ";
                else outstream<<Matrix[i][j]<<" ";
            }outstream<<endl;
        }outstream<<endl<<endl;
    }//Floyed-Warshall function
};

int main(void){
    ifstream instream("In.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("Out.txt");

    int V, E;
    instream>>V>>E;

    graph myGraph(V, E);
    int u, v; float weight;
    for(int i = 0; i<E; i++){
        instream>>u>>v>>weight;
        myGraph.setAdjacent(u-1, v-1, weight);
    }

    myGraph.matMultiply(outstream);
    myGraph.floydWarshall(outstream);

    instream.close(); outstream.close();
    return 0;
}


//g++ -o apsp APSP.cpp



