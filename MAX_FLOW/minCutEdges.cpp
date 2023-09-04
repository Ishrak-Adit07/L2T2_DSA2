#include <bits/stdc++.h>
#include <fstream>
using namespace std;

//Helper Functions
float min(float a, float b) { return a < b ? a : b; }

//Helper Classes
class vertex{
    int id;
    string name;
    //int numberOfVertices;
    //vector<bool> adjacents;
public:
	/*
    vertex(int vid, int n){
        id = vid;
        numberOfVertices = n;
        adjacents.resize(n, false);
    }
    */
    vertex(string name, int id){
		name = name;
		id = id;
	}
    int getId(){ return id; }
    string getName() { return name; }
    //void setAdjacent(int i){ adjacents[i] = true; }
    //bool checkAdjacent(int v){ return adjacents[v]; }
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

class teamVertex{
    int id;
    string name;
    int wins, losses, left;
    int numberOfTeams;
public:
    vector<int> gamesLeft;
    teamVertex(int N, int tid, string tname, int w, int loss, int l, vector<int> lefts){
        numberOfTeams = N; id = tid; name = tname;
        wins = w; losses = loss; left = l;
        gamesLeft.resize(numberOfTeams);
        for(int i=0; i<numberOfTeams; i++) gamesLeft[i] = lefts[i];
    }
    int getId() { return id; }
    string getName() { return name; }
    int getWins() { return wins; }
    int getLosses() { return losses; }
    int getLefts() { return left; }
    void showGamesLeft() { for(auto i: gamesLeft) { cout<<i<<"  "; } cout<<endl; }
    vector<int> getGamesLeft() { return gamesLeft; }
};

class graph{
    int numberOfVertices, numberOfEdges;
    vector<int> vertices;

    vector<Edge> edges;
    int pairStart, pairEnd;
    int teamStart, teamEnd;

public:
    graph(int V, int E){
        numberOfVertices = V;
        numberOfEdges = E;
        vertices.resize(numberOfVertices);
        for(int i=0; i<numberOfVertices; i++){
			vertices[i] = i;
        }
    }

    void setEdge(int from, int to, int weight){
		edges.push_back({from, to, weight});
    }

    void dfs(int node, vector<bool> &visited, vector<vector<int>> resMatrix){
		visited[node] = true;
		for(int i=0; i<numberOfVertices; i++){
			if(!visited[i] && resMatrix[node][i]>0){
				dfs(i, visited, resMatrix);
			}
		}
    }

    bool bfsPathCheck(int source, int sink, vector<vector<int>> resMatrix, vector<int> &parent){
        queue<int> bfsqueue;
        vector<bool> visited(numberOfVertices, false);
        bfsqueue.push(source);
        visited[source] = true;
        parent[source] = -1;

        while(!bfsqueue.empty()){
            int node = bfsqueue.front(); bfsqueue.pop();

            for(int v=0; v<numberOfVertices; v++){
                if(!visited[v] && resMatrix[node][v] > 0){
                    bfsqueue.push(v);
                    parent[v] = node;
                    visited[v] = true;
                }
            }//for
        }//while

        return visited[sink];
    }//function

    int FordFulkerson(int source, int sink){
        vector<vector<int>> resMatrix; //Residual Matrix
        resMatrix.resize(numberOfVertices, vector<int>(numberOfVertices));

        vector<vector<int>> edgeMatrix;
        edgeMatrix.resize(numberOfVertices, vector<int>(numberOfVertices));

        //Initial Capacity = Edge weight/0
        for(int i=0; i<numberOfVertices; i++){
			for(int j=0; j<numberOfVertices; j++){
				resMatrix[i][j] = 0;
				edgeMatrix[i][j] = 0;
			}
        }
        for(auto edge: edges) resMatrix[edge.getFrom()][edge.getTo()] = edgeMatrix[edge.getFrom()][edge.getTo()] = edge.getWeight();

		/*
        for(int i=0; i<numberOfVertices; i++){
			for(int j=0; j<numberOfVertices; j++){
				cout<<resMatrix[i][j]<<"  ";
			}
			cout<<endl;
        }
		*/
        vector<int> parent(numberOfVertices);
        int maxFlow = 0;

        //Finding augmented paths and updating residual matrix
        while(bfsPathCheck(source, sink, resMatrix, parent)){
            int bnCapacity = INT_MAX;
            for(int v = sink; v != source; v = parent[v]){
                int u = parent[v];
                bnCapacity = min(bnCapacity, resMatrix[u][v]);
            }

            for(int v = sink; v != source; v = parent[v]){
                int u=parent[v];
                resMatrix[u][v] -= bnCapacity;
                resMatrix[v][u] += bnCapacity;
            }

            maxFlow += bnCapacity;
        }//while

		cout<<"MinCut: "<<maxFlow<<endl;
        // Check Saturation for Initial Edges
        vector<bool> visited(numberOfVertices, false);
        dfs(source, visited, resMatrix);

        vector<int> sourceCut, sinkCut;

		cout<<"S-cut: ";
		for(int i=0; i<numberOfVertices; i++){
			if(visited[i]){
				cout<<i<<"  ";
				sourceCut.push_back(i);
			}

        }
        cout<<endl<<"T-cut: ";
        for(int i=0; i<numberOfVertices; i++){
			if(!visited[i]){
				 cout<<i<<"  ";
				 sinkCut.push_back(i);
			}
        }cout<<endl;


        cout<<"Min Cut Edges: "<<endl;
		for(auto i: sourceCut){
			for(auto j: sinkCut){
				if(edgeMatrix[i][j]>0){
					cout<<i<<"  "<<j<<endl;
				}
			}
		}
		/*
        for(int i=0; i<numberOfVertices; i++){
			for(int j=0; j<numberOfVertices; j++){
				cout<<resMatrix[i][j]<<"  ";
			}
			cout<<endl;
        }*/

        return maxFlow;
    }

	/*
    void makeGraph(int teamToCheck){
        // Source to Pairs*********************************************************
        int setEdgefromSource = pairStart;

        for(int i=0; i<numberOfTeams; i++){
            for(int j=i+1; j<numberOfTeams; j++){
                if(i!=teamToCheck && j!=teamToCheck && setEdgefromSource<=pairEnd){
                    edges.push_back({0, setEdgefromSource, teams[i].getGamesLeft()[j] });
                    setEdgefromSource++;
                }//if
            }//for
        }//for

        // Pairs to Teams**********************************************************
        int setEdgefromPairs = pairStart;
        int setEdgetoTeams = teamStart;

        for(int i=numberOfOpponents-1; i>0; i--){
            for(int j=1; j<=i; j++){
                edges.push_back({setEdgefromPairs, setEdgetoTeams, INT_MAX});
                edges.push_back({setEdgefromPairs, setEdgetoTeams+j, INT_MAX});
                setEdgefromPairs++;
            }
            setEdgetoTeams++;
        }

        // Teams to Sink***********************************************************
        int maxPossibleWinforTeamtoCheck = teams[teamToCheck].getWins()+teams[teamToCheck].getLefts();
        int teamJ = 0;
        for(int i=0; i<=numberOfOpponents; i++){
            if(i != teamToCheck){
                edges.push_back({teamStart+teamJ, numberOfVertices-1, maxPossibleWinforTeamtoCheck-teams[i].getWins()});
                teamJ++;
            }
        }

        // Display********************************************
        for(auto edge: edges) {
            cout<<edge.getFrom()<<"  "<<edge.getTo()<<"  ";
            if(edge.getWeight() == INT_MAX) cout<<"INF"<<endl;
            else cout<<edge.getWeight()<<endl;
        }

    }//makeGraph function


    bool isEliminated(int teamToCheck){
        makeGraph(teamToCheck);
        pair<int, bool> ans = FordFulkerson(0, numberOfVertices-1, teamToCheck);

        return ans.second;
    }//Helper for Check Elimination Function

    void checkForEliminations(ofstream &outstream){
        for(int i=0; i<numberOfTeams; i++){
            if(isEliminated(i)){
                outstream<<teams[i].getName()<<" is eliminated."<<endl<<endl;
                //outstream<<"They can win at most "<<teams[i].getWins()<<" + "<<teams[i].getLefts()<<" = ";
                //outstream<<teams[i].getWins()+teams[i].getLefts()<<" games."<<endl<<endl;
            }//if
        }//for
    }//Check Elimination Function
    */
};

int main(void){
    ifstream instream("input.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("Out.txt");

    int V, E; instream>>V>>E;
    graph myGraph(V, E);

    int from, to;
    int weight;
    int source, sink;

    for(int i=0; i<E; i++){
		instream>>from>>to>>weight;
		myGraph.setEdge(from, to, weight);
    }
    instream>>source>>sink;

    myGraph.FordFulkerson(source, sink);
    instream.close(); outstream.close();
}


//g++ -o mincut minCutEdges.cpp