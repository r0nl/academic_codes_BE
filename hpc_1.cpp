#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

// Define the number of threads to use for parallelization
const int NUM_THREADS = 32;

// Function to perform Breadth First Search (BFS) in parallel
void bfs(vector<vector<int>> &graph, int start) {
    // Mark all nodes as not visited
    vector<bool> visited(graph.size(), false);
    // Create a queue for BFS
    queue<int> q;
    // Mark the starting node as visited and enqueue it
    visited[start] = true;
    q.push(start);

    // While there are nodes in the queue
    while (!q.empty()) {
        // Dequeue a vertex from the queue and print it
        int curr = q.front();
        cout << curr << " ";
        q.pop();

        // Get the neighbors of the current vertex
        vector<int> neighbors = graph[curr];

        // For each neighbor of the current vertex
        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < neighbors.size(); i++) {
            // Get the current neighbor
            int neighbor = neighbors[i];
            // If the neighbor has not been visited
            if (!visited[neighbor]) {
                // Mark it as visited and enqueue it
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

// Function to perform Depth First Search (DFS) in parallel
void dfs(vector<vector<int>> &graph, int start) {
    // Mark all nodes as not visited
    vector<bool> visited(graph.size(), false);
    // Create a stack for DFS
    stack<int> s;
    // Mark the starting node as visited and push it onto the stack
    visited[start] = true;
    s.push(start);

    // While there are nodes on the stack
    while (!s.empty()) {
        // Pop a vertex from the stack and print it
        int curr = s.top();
        cout << curr << " ";
        s.pop();

        // Get the neighbors of the current vertex
        vector<int> neighbors = graph[curr];

        // For each neighbor of the current vertex
        #pragma omp parallel for num_threads(NUM_THREADS)
        for (int i = 0; i < neighbors.size(); i++) {
            // Get the current neighbor
            int neighbor = neighbors[i];
            // If the neighbor has not been visited
            if (!visited[neighbor]) {
                // Mark it as visited and push it onto the stack
                visited[neighbor] = true;
                s.push(neighbor);
            }
        }
    }
}

int main() {
    int n, m, start;
    cout << "Enter the number of nodes and edges and start vertex: ";
    cin >> n >> m >> start;

    // Create an empty graph as an adjacency list
    vector<vector<int>> graph(n);

    // Read in the edges and add them to the graph
    cout << "Enter the edges:" << endl;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u); // Add this line for an undirected graph
    }

    // Run BFS starting from the specified node
    cout << endl << endl << "BFS: ";
    bfs(graph, start);
    cout << endl;

    // Run DFS starting from the specified node
    cout << endl << "DFS: ";
    dfs(graph, start);
    cout << endl;

    cout << endl << endl << "Adjacency List:" << endl;
    for (int i=0; i<n; i++){
        for (int j=0; j<graph[i].size(); j++){
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
