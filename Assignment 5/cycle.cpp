#include <iostream>
#include <vector>
using namespace std;

class Graph {
public:
    Graph(int v) { V = v, children.resize(V); }
    ~Graph() {}
    
    void addEdge(int u, int v) { children[u].push_back(v); }

    bool cycle(vector<int> &path) const {
        vector<bool> visited(V, false), in_current_path(V, false);

        //Ensure all nodes will be checked even if directed edges don't allow a single pass for sure
        //Before returning the path, we need to make sure that a cycle ONLY is present, not a cycle with a tail (O not P)

        for (int i = 0; i < V; ++i)
            if (dfs(i, visited, path, in_current_path)) {
                int startNode = path[path.size() - 1], startidx;
                
                for (startidx = path.size() - 2; path[startidx] != startNode; --startidx);

                vector<int> pathCopy;
                for (int s = startidx; s < path.size() - 1; ++s)
                    pathCopy.push_back(path[s]);

                path.clear();
                path = pathCopy;

                return true;
            }

        return false; //if all traversed and not already returned, there's no cycle
    }

private:
    int V;
    vector<vector<int>> children;
    //Use adjacency list representation

    bool dfs(int root, vector<bool> &visited, vector<int> &path, vector<bool> &in_current_path) const {
        if (!visited[root]) { //if already visited, skip, false
            visited[root] = true;
            path.push_back(root);
            in_current_path[root] = true;
            
            for (int c : children[root]) {
                if (in_current_path[c]) {
                    path.push_back(c); //Add it to the path so we can know what the start/end node is
                    return true; //The loop closed, we found a cycle. Return success
                }
                
                if (dfs(c, visited, path, in_current_path)) return true; //path already in here
            }

            path.pop_back();
            in_current_path[root] = false;
        }

        //This node failed to find a cycle
        return false; //return the "default answer"
        //before we return false, pop this current node
        //otherwise return true with full cycle in path
    }
};

#ifndef CONTEST
int main() {
    int V, E;
    cin >> V >> E;
    
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v; cin >> u >> v;
        g.addEdge(u, v);
    }
    
    vector<int> path;
    bool c = g.cycle(path);
    
    if (c) {
        cout << "CYCLE: ";
        for (int i = 0; i < path.size(); ++i)
        cout << path[i] << (i == path.size()-1 ? "\n" : " ");
    }
    else {
        cout << "NO CYCLE" << endl;
    }
}
#endif