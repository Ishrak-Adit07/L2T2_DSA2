#include <bits/stdc++.h>
#include <fstream>
using namespace std;

//Helper Functions
float min(float a, float b) { return a < b ? a : b; }

//Helper Classes
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
    vector<teamVertex> teams;
    int numberOfTeams, numberOfOpponents;

    int numberOfVertices;
    //vector<vertex> vertices;
    vector<Edge> edges;
    int pairStart, pairEnd;
    int teamStart, teamEnd;

public:
    graph(int N){
        numberOfTeams = N;
        numberOfOpponents = numberOfTeams - 1;
        int n = 0;
        for(int i=1; i<=numberOfOpponents-1; i++) n += i; //How many pairs left from the opponents
        pairStart = 1; pairEnd = n;
        teamStart = n+1; teamEnd = n+numberOfOpponents;
        numberOfVertices = n+ numberOfOpponents +2; //Pairs+ Opponent Teams +(source+sink)
        //for(int i=0; i<numberOfVertices; i++) vertices.push_back({i, numberOfVertices});
    }

    void setTeam(teamVertex team){
        teams.push_back(team);
    }

    void display(){
        for(int i=0; i<numberOfTeams; i++){
                cout<<teams[i].getName()<<"  "<<teams[i].getWins()<<"  "<<teams[i].getLosses()<<"  "<<teams[i].getLefts()<<"  ";
                teams[i].showGamesLeft();
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

    pair<int, bool> FordFulkerson(int source, int sink, int teamToCheck){
        vector<vector<int>> resMatrix; //Residual Matrix
        resMatrix.resize(numberOfVertices, vector<int>(numberOfVertices));

        //Initial Capacity = Edge weight/0
        for(int i=0; i<numberOfVertices; i++)
            for(int j=0; j<numberOfVertices; j++)
                resMatrix[i][j] = 0;
        for(auto edge: edges) resMatrix[edge.getFrom()][edge.getTo()] = edge.getWeight();

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
                int u = parent[v];
                resMatrix[u][v] -= bnCapacity;
                resMatrix[v][u] += bnCapacity;
            }

            maxFlow += bnCapacity;
        }//while

        // Check Saturation for Initial Edges
        bool eliminationCheck = false;
        for(int i=pairStart; i<=pairEnd; i++){ if(resMatrix[0][i]>0) eliminationCheck = true; }

        return {maxFlow, eliminationCheck};
    }

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
        /*for(auto edge: edges) {
            cout<<edge.getFrom()<<"  "<<edge.getTo()<<"  ";
            if(edge.getWeight() == INT_MAX) cout<<"INF"<<endl;
            else cout<<edge.getWeight()<<endl;
        }*/

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
};

int main(void){
    ifstream instream("In.txt");
    if(!instream){
        cout<<"Input File Not Available"<<endl;
        return -1;
    }
    ofstream outstream("Out.txt");

    int numberOfTeams; instream>>numberOfTeams;
    graph myGraph(numberOfTeams);

    string name; int win, loss, left;
    int n;//To help make vector gamesLeft

    vector<int> gamesLeft(numberOfTeams);
    for(int i=0; i<numberOfTeams; i++){
        instream>>name>>win>>loss>>left;
        for(int j=0; j<numberOfTeams; j++) { instream>>n; gamesLeft[j] = n; }

        teamVertex team(numberOfTeams, i, name, win, loss, left, gamesLeft);
        myGraph.setTeam(team);
    }

    myGraph.checkForEliminations(outstream);
    instream.close(); outstream.close();
}

























//Test Cases*******************
/*
5
New_York 75 59 28 0 3 8 7 3
Baltimore 71 63 28 3 0 2 7 4
Boston 69 66 27 8 2 0 0 0
Toronto 63 72 27 7 7 0 0 0
Detroit 49 86 27 3 4 0 0 0
*/
/*
4
Atlanta       83 71  8  0 1 6 1
Philadelphia  80 79  3  1 0 0 2
New_York      78 78  6  6 0 0 0
Montreal      77 82  3  1 2 0 0
*/

