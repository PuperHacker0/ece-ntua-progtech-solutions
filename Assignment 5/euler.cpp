#include <iostream>
using namespace std;

    //LOGIC
    //Read input
    //This suffices because the graph is given to be connected
    //Cound degrees of each vertex to know if there is a cycle or not
    //if yes print CYCLE
    //otherwise, try to find a path:
    //check if all degrees are even except for exactly two odd degrees! (if zero, we've already found a cycle)
    //This is because the euler path is start->middle->end.
    //So the middle vertices (almost all in the general case) need an entrance and an exit
    //So you can traverse the vertex multiple times but you have to get in and out otherwise you're stuck
    //so even degree.
    //But in the start, you are already assumed to have entered. So either you'll have even degree (cycle)
    //Or you can have an odd degree (path) so you traverse n times (enter and exit) and then leave one final time
    //otherwise you'll get stuck, because you've already entered once
    //then the same logic applies for the exit. traverse multiple times but you'll need to enter one final time
    //without leaving (you'll get stuck, an exit is implied when the program ends)
    //So only 0 (cycle) or 2 odd degree vertices are allowed because one is the start and the other is the end,
    //and we can't have an odd-degree end if we have multiple starts or multiple ends, meaning the traversal WILL
    //get stuck there, as it must, so that it can be considered an end. So if >1 odd-deg vertices, it has to be 2
    //And only 2, because we then can't leave the end node so max 2

int main() {
    //We don't need a graph representation, as counting degrees is enough to answer this question

    int N, M;
    cin >> N >> M;

    int *deg = new int[N];
    for (int i = 0; i < N; ++i)
        deg[i] = 0; //Initialize the memory before using it

    int u, v;
    while (M--) { //Read all M edges (M is last seen as 0, but the decrement still happens, unlike a statement which wouldn't)
        cin >> u >> v;
        deg[u]++, deg[v]++; //Increase degree of both vertices, 0-indexed
    }

    int odds = 0, odd1, odd2;

    for (int i = 0; i < N; ++i) {
        //the entire if block can be written with a 1-liner
        if (deg[i] % 2) { //odd
            ++odds;

            if (odds > 2) break; //No need to continue, impossible
            else if (odds == 2)
                odd2 = i;
            else //first odd
                odd1 = i;
        }
    }

    if (odds == 0)
        cout << "CYCLE\n";
    else if (odds == 2)
        cout << "PATH " << odd1 << " " << odd2 << endl;
        //odd1 < odd2 always because the ids are sorted in the traversal
    else
        cout << "IMPOSSIBLE\n";

    delete[] deg;
    deg = nullptr;

    return 0;
}