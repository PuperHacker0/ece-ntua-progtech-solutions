#include <iostream>
#include <vector>
using namespace std;

//We don't need to optimize for less fuel or less stops for refueling, so the problem becomes much easier.
//Just find a path that will work

struct Edge {
    int dest, cost;
    Edge(int d, int c) { dest = d, cost = c; }
};

//Recursive to begin with, if it OVERFLOWS we'll use a STACK
bool dfs(const vector<vector<Edge>> &children, int start, int end, int tank, vector<bool> &visited, vector<int> &path, int &fills, int currTankLevel) {
    if (visited[start]) return false; //Default answer, already seen this node

    visited[start] = true;
    
    if (start == end) { //Found the target node
        return true;
    }

    for (const Edge &e : children[start]) {
        //Take care of gas here
        if (e.cost > tank) continue; //BLOCK path; we can't reach this node through this road, tank will not suffice

        path.push_back(e.dest);
        //Tank size suffices, will it need a refill?
        int tankLevelAtDestination = currTankLevel - e.cost; //Currtank level is not global so we dont need a flag for that too, just send the right info
        bool refillForDestination = false;
        
        if (tankLevelAtDestination < 0) {
            refillForDestination = true; //We'll need to fill up in this node to reach the destination
            fills++;
            tankLevelAtDestination = tank - e.cost;
        }
        //else Gas still sufficient and tankLevel already set   

        if (dfs(children, e.dest, end, tank, visited, path, fills, tankLevelAtDestination)) {
            return true;
        }

        path.pop_back(); //Remove the node we just put and didn't find a solution, try the next child
        if (refillForDestination) fills--; //Undo fill for this destination, move to next one (fills is global so we need it constantly updated)
    }

    return false; //No child of this node could find a solution, return false
}

void query(const vector<vector<Edge>> &children, int A, int B, int C, int N) {
    //Special case
    if (A == B) {
        printf("POSSIBLE: 0 fill(s), %d\n", A);
        return;
    }

    vector<bool> visited(N, false);
    vector<int> path = { A }; //Add the root to the path and the rest is done automatically in dfs
    //visited is automatically marked inside dfs so no need to update that too
    int fills = 1; //We start by filling up with a tank at the first node

    bool possible = dfs(children, A, B, C, visited, path, fills, C);

    if (!possible) {
        printf("IMPOSSIBLE\n");
        return;
    }
    //else, possible
    printf("POSSIBLE: %d fill(s),", fills);

    for (int i : path) printf(" %d", i);
    
    printf("\n");
}

int main() {
    int N, M;
    scanf("%d%d", &N, &M);

    vector<vector<Edge>> adjList(N);
    int U, V, L;
    for (int i = 0; i < M; ++i) {
        scanf("%d%d%d", &U, &V, &L);

        //Undirected graph
        adjList[U].push_back(Edge(V, L));
        adjList[V].push_back(Edge(U, L));
    }

    int Q, A, B, C;
    scanf("%d", &Q);

    for (int i = 0; i < Q; ++i) {
        scanf("%d%d%d", &A, &B, &C);
        query(adjList, A, B, C, N);
    }

    return 0;
}