#include <iostream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

int min(int a, int b){
    return a<b?a:b;
}

class Graph {
public:
    int V;
    vector<vector<int>> graph;

    Graph(int V) : V(V), graph(V, vector<int>(V, 0)) {}

    void add_edge(int u, int v, int weight) {
        graph[u][v] = weight;
        graph[v][u] = weight;
    }
};

Graph Create_Random_Graph(int V) {
    Graph G(V);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> weight_dist(50, 200);

    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            int weight = weight_dist(gen);
            G.add_edge(i, j, weight);
        }
    }
    return G;
}

int tsp_dp(int mask, int pos, Graph& G, vector<vector<int>> &dp){
    if (mask == (1 << G.V) - 1) {
            return G.graph[pos][0];
        }

        if (dp[mask][pos] != -1) {
            return dp[mask][pos];
        }

        int ans = INT8_MAX;
        for (int city = 0; city < G.V; ++city) {
            if (((mask >> city) & 1) == 0) {
                int new_mask = mask | (1 << city);
                ans = min(ans, G.graph[pos][city] + tsp_dp(new_mask, city, G, dp));
            }
        }

        dp[mask][pos] = ans;
        return ans;
}

int Exact_TSP(Graph& G) {
    vector<vector<int>> dp(1 << G.V, vector<int>(G.V, -1));

    return tsp_dp(1, 0, G, dp);
}

vector<int> Metric_Approximation_TSP(Graph& G) {
    // Implementation of the 2*OPT approximation algorithm
    // You can use your preferred method to find Minimum Spanning Tree (MST)

    vector<int> tour;
    vector<int> visited(G.V, false);

    // Find an MST (Minimum Spanning Tree) - Prim's algorithm
    vector<int> parent(G.V, -1);
    vector<int> key(G.V, INT8_MAX);

    key[0] = 0;
    for (int count = 0; count < G.V - 1; ++count) {
        int u = -1;
        for (int v = 0; v < G.V; ++v) {
            if (!visited[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }

        visited[u] = true;

        for (int v = 0; v < G.V; ++v) {
            if (G.graph[u][v] && !visited[v] && G.graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = G.graph[u][v];
            }
        }
    }

    // Double every edge of the MST to obtain an Eulerian graph
    for (int i = 1; i < G.V; ++i) {
        tour.push_back(parent[i]);
        tour.push_back(i);
    }

    return tour;
}

int Calculate_Tour_Length(Graph& G, vector<int>& tour) {
    int length = 0;
    int n = tour.size();
    for (int i = 0; i < n; ++i) {
        int u = tour[i];
        int v = tour[(i + 1) % n];
        length += G.graph[u][v];
    }
    return length;
}

int main() {
    int X;
    cin >> X;

    vector<int> optimal_lengths;
    vector<int> approximate_lengths;

    for (int x = 1; x <= X; ++x) {
        int V = 20;
        Graph G = Create_Random_Graph(V);
        int optimal_tour_length = Exact_TSP(G);
        vector<int> approximate_tour = Metric_Approximation_TSP(G);
        optimal_lengths.push_back(optimal_tour_length);

        int approximate_length = Calculate_Tour_Length(G, approximate_tour);
        approximate_lengths.push_back(approximate_length);

        double ratio = static_cast<double>(approximate_length) / optimal_tour_length;
        cout << "Test Case " << x << ": " << ratio << endl;
    }

    return 0;
}

//g++ -o app app2.cpp