#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class Edge{
    int from, to, weight;

public:
    Edge(){} //Empty Constructor
    Edge(int From, int To, int Weight){
        from = From;
        to = To;
        weight = Weight;
    } //Constructor
    void setEdge(int From, int To, int Weight){
        from = From;
        to = To;
        weight = Weight;
    } //function
    int getFrom() { return from; } //function
    int getTo(){ return to; } //function
    int getWeight() { return weight; } //function
};

bool comparefn(Edge e1, Edge e2){
    return e1.getWeight()<e2.getWeight();
}

class graph{
    int numberOfVertices;
    int numberOfEdges;

    //For Application of Kruskal's Algorithm
    vector<Edge> edges;
    vector<int> parents, ranking;

public:
    graph(int V, int E){
        numberOfVertices = V;
        numberOfEdges = E;
        ranking.resize(numberOfVertices, 0);
        for(int i=0; i<numberOfVertices; i++) parents.push_back(i);
    }//Constructor
    
    void setAdjacent(int u, int v, int weight){
        edges.push_back({u, v, weight});
    }//function

    //Helper Functions(DSU) for Kruskal's Algorithm
    int findParent(int node){
        if(parents[node] == node) return node; // No parent, represents it's own set
        return parents[node] = findParent(parents[node]); //Find representative of that set
    } //function
    void uniteSets(int node1, int node2){
        int parent1 = findParent(node1); int parent2 = findParent(node2);
        if(ranking[parent1] > ranking[parent2]) parents[parent2] = parent1;
        else if(ranking[parent1] < ranking[parent2]) parents[parent1] = parent2;
        else{ parents[parent2] = findParent(parent1); ranking[parent1] += 1; }
    } //function

    //Function for Kruskal's algorithm
    void kruskalMST(ofstream &outstream){
        int kruskalWeight = 0;
        queue<int> from, to;

        sort(edges.begin(), edges.end(), comparefn);
        for(auto item: edges){
            if( findParent(item.getFrom()) != findParent(item.getTo())){
                // Current edge does not form a cycle in current MST, allowed.
                uniteSets(item.getFrom(), item.getTo()); //Joining
                //Recording Edge and Weight
                from.push(item.getFrom()); to.push(item.getTo());
                kruskalWeight += item.getWeight();
            }
            // else, the edge forms a cycle in current MST, hence, not allowed.
        }//for loop

        outstream<<"Kruskal's Algorithm:"<<endl<<"Total Weight = "<<kruskalWeight<<endl;
        while(!from.empty()){
            outstream<<from.front()<<"  "<<to.front()<<endl;
            from.pop(); to.pop();
        }
    } //MST Function
};

int main(void){
    ifstream instream("In.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("kruskal.txt");
    int V, E; instream>>V>>E;

    graph myGraph(V, E);
    int u, v, weight;
    for(int i = 0; i<E; i++){
        instream>>u>>v>>weight;
        myGraph.setAdjacent(u, v, weight);
    }

    myGraph.kruskalMST(outstream);
    instream.close(); outstream.close();
    return 0;
}


//g++ -o kk Kruskal.cpp


