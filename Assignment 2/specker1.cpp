#include <iostream>
#include <stdexcept>
using namespace std;

class Move {
public:
    // Take sc coins from heap sh and put tc coins to heap th.
    Move(int sh, int sc, int th, int tc) {
        this->sh = sh, this->sc = sc;
        this->th = th, this->tc = tc;
    }
    
    int getSource() const { return sh; }
    int getSourceCoins() const { return sc; }
    int getTarget() const { return th; }
    int getTargetCoins() const { return tc; }
    
    friend ostream & operator << (ostream &out, const Move &move) {
        out << "takes " << move.getSourceCoins() << " coins from heap " << move.getSource() << " and puts ";

        if (move.getTargetCoins() == 0) { out << "nothing"; }
        else out << move.getTargetCoins() << " coins to heap " << move.getTarget();

        return out;
    }

private:
    int sh, sc, th, tc;
};

class State {
public:
    // State with h heaps, where the i-th heap starts with c[i] coins.
    // A total of n players are in the game, numbered from 0 to n-1,
    // and player 0 is the first to play.
    State(int h, const int c[], int n) {
        H = h, N = n;
        nextTurn = 0;

        C = new int[H];

        for (int i = 0; i < H; ++i) {
            C[i] = c[i];
            sum += c[i];
        }
    }
    ~State() {
        delete[] C;
    }

    void next(const Move &move) { // may throw logic_error
        //Check first
        int s = move.getSource(), t = move.getTarget();
        int sc = move.getSourceCoins(), tc = move.getTargetCoins();

        if (s < 0 || s >= H || t < 0 || t >= H || sc <= 0 || tc < 0 || tc >= sc || sc > C[s])
            throw logic_error("invalid heap");

        //Execute the move

        C[s] -= sc, C[t] += tc;
        sum = sum - sc + tc;
        (++nextTurn) %= N;
    }
    bool winning() const {
        return sum == 0;
    }

    int getHeaps() const { return H; }
    int getCoins(int h) const {
        if (h < 0 || h >= H) throw logic_error("invalid heap");

        return C[h];
    } // may throw logic_error
    
    int getPlayers() const { return N; }
    int getPlaying() const { return nextTurn; }
    
    friend ostream & operator << (ostream &out, const State &state) {
        for (int i = 0; i < state.getHeaps() - 1; ++i) {
            out << state.C[i] << ", ";
        }
        out << state.C[state.getHeaps() - 1] << " with " << state.getPlaying() << "/" << state.getPlayers() << " playing next";

        return out;
    }

private:
    int* C;
    int H, N;
    int nextTurn, sum = 0;
};

/*int main() {
    return 0;
}*/