#include <bits/stdc++.h>
#include <fstream>

using namespace std;

int min(int a, int b){
    return a<b?a:b;
}

typedef pair<int, int> intPair;
typedef vector<intPair> Tour;

class vertex{
    int id;
    int numberOfVertices;
    vector<bool> adjacents;

public:
    vertex(int vid, int n){
        id = vid;
        numberOfVertices = n;
        adjacents.resize(n, false);
    }

    int getId(){ return id; }
    void setAdjacent(int i){ adjacents[i] = true; }
    bool checkAdjacent(int v){ return adjacents[v]; }
    vector<bool> getAdjacentList() { return adjacents; }
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

class graph{
    vector<vertex> vertices;
    vector<Edge> edges;
    vector<vector<int>> weightsOfEdges;
    int numberOfVertices;
    int numberOfEdges;

    //For application of Prim's algorithm
    vector<bool> msTree; vector<int> keys;
    priority_queue<intPair, vector<intPair>, greater<intPair>> keyQueue;
public:
    graph(int V, int E){
        numberOfVertices = V;
        numberOfEdges = E;
        for(int i=0; i<numberOfVertices; i++){
            vertices.push_back({i, numberOfVertices});
        }
        weightsOfEdges.resize(numberOfVertices, vector<int>(numberOfVertices, 0));
        //dp.resize(numberOfVertices+1, vector<int>(1<< (numberOfVertices+1) ));
    }

    void setAdjacent(int u, int v){
        vertices[u].setAdjacent(v);
    }

    bool checkAdjacent(int u, int v){
        return vertices[u].checkAdjacent(v);
    }

    void setEdge(int u, int v, int weight){
        if(weight != 0){
            vertices[u].setAdjacent(v);
            edges.push_back({u, v, weight});
        }
        weightsOfEdges[u][v] = weight;
    }



    void display(){
        cout<<numberOfVertices<<"  "<<numberOfEdges<<endl;
        cout<<"Id   ";
        for(int i=0; i<numberOfVertices; i++){
            cout<<i<<"  ";
        }
        cout<<endl;
        for(int i=0; i<numberOfVertices; i++){
            for(int j=0; j<numberOfVertices; j++){
                cout<<weightsOfEdges[i][j]<<"  ";
            }
            cout<<endl;
        }
    }

    void ensureMetricForm(){
        for(int k=0; k<numberOfVertices; k++){
            for(int i=0; i<numberOfVertices; i++){
                for(int j=0; j<numberOfVertices; j++){
                    if( weightsOfEdges[i][j] > ( weightsOfEdges[i][k]+weightsOfEdges[k][j] )){
                        weightsOfEdges[i][j] =  weightsOfEdges[i][k]+weightsOfEdges[k][j];
                    }
                }
            }
        }
    }

    //Function for Prim's algorithm
    vector<intPair> primMST(int root){
        vector<intPair> tour;

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
                if(vertices[minkeyNode.second].checkAdjacent(i) && weightsOfEdges[minkeyNode.second][i]<keys[i]){
                    // Updating current key values to lowest possible values
                    keys[i] = weightsOfEdges[minkeyNode.second][i];
                    parents[i] = minkeyNode.second;
                    keyQueue.push(make_pair(keys[i], i));
                }
            }
            record--;
        } //Loop Ends

        for(int i=0; i<numberOfVertices; i++){
            if(i!=root){
                tour.push_back(make_pair(i, parents[i]));
            }
        }
        return tour;
    }//MST Function

    int Exact_TSP_DP(int mask, int v, vector<vector<int>> &dp){
        if (mask == (1 << numberOfVertices) - 1) {
            return weightsOfEdges[v][0];
        }

        if (dp[mask][v] != -1) {
            return dp[mask][v];
        }

        int minPath = INT_MAX;
        for(int i=0; i<numberOfVertices; i++){
            if( ((mask>>i) & 1) == 0 ){
                int temp_mask = mask | (1<<i);
                minPath = min(minPath, weightsOfEdges[v][i]+Exact_TSP_DP(temp_mask, i, dp));
            }
        }
        dp[mask][v] = minPath;
        return minPath;
    }

    int Exact_TSP(){
        vector<vector<int>> dp(1 << numberOfVertices, vector<int>(numberOfVertices, -1));
        return Exact_TSP_DP(1, 0, dp);
    }

    int Metric_Approximation_TSP(){
        vector<intPair> tour = primMST(0);
        return Calculate_Tour_Length(tour);
    }

    int Calculate_Tour_Length(vector<intPair> T){
        int tourCost = 0;
        for(int i=0; i<T.size(); i++){
            tourCost += weightsOfEdges[T[i].first][T[i].second];
        }
        return tourCost;
    }
};

graph Create_Random_Grpah(int V){
    int E = V*(V-1)/2;
    graph randomGraph(V, E);
    int maxWeight = 50200;
    for(int i=0; i<V; i++){
        for(int j=0; j<V; j++){
            if(i != j && !randomGraph.checkAdjacent(i, j)){
                int randomWeight = (rand() % 151) + 50;
                randomGraph.setEdge(i, j, randomWeight);
                randomGraph.setEdge(j, i, randomWeight);
            }

            else if(i == j && !randomGraph.checkAdjacent(i, j)){
                randomGraph.setEdge(i, j, 0);
                randomGraph.setEdge(j, i, 0);
            }
        }
    }
    return randomGraph;
}

int main(void){
    int X; cin>>X; //Number of testcases
    int constNumberOfVertices = 20;
    int constNumberOfEdges = 190;

    for(int testCase=0; testCase<X; testCase++){
        graph G = Create_Random_Grpah(constNumberOfVertices);
        G.ensureMetricForm();
        //G.display();

        int exactTspLength = G.Exact_TSP();
        int approxTspLength = G.Metric_Approximation_TSP();

        cout<<testCase+1<<", "<<(float)approxTspLength/(float)exactTspLength<<endl;
    }
}

//g++ -o tsp 2005105.cpp