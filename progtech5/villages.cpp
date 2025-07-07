#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class UnionFind { //0-indexed union-find
    vector<int> parent;
    vector<int> height_upper_bound;
    int size;
public:
    UnionFind(int s) {
        size = s;
        parent.resize(size);
        height_upper_bound.resize(size, 0);

        for (int i = 0; i < size; i++)
            parent[i] = i;
    }

    int find(int i) {
        return (parent[i] == i) ? i : (parent[i] = find(parent[i]));
    }

    void unite(int i, int j) { //Improved union using union by rank
        int i_root = find(i), j_root = find(j);
       
        if (i_root == j_root) return; //Already united in the same set

        if (height_upper_bound[i_root] < height_upper_bound[j_root])
            parent[i_root] = j_root;
        else if (height_upper_bound[i_root] > height_upper_bound[j_root])
            parent[j_root] = i_root;
        else {
            parent[j_root] = i_root;
            height_upper_bound[i_root]++;
        }
    }

    int separate_sets() {
        int ans = 0;

        for (int i = 0; i < size; ++i)
            ans += (parent[i] == i);

        return ans;
    }
};

int main() {
    int N, M, K;
    scanf(" %d %d %d", &N, &M, &K);

    UnionFind uf(N);

    int u, v;
    while (M--) {
        scanf(" %d %d", &u, &v);
        uf.unite(u - 1, v - 1);
    }

    printf("%d\n", max(1, uf.separate_sets() - K));

    return 0;
}