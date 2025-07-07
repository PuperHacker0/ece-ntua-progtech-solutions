#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

/*class Move {
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

class Player {
public:
    Player(const string &n) {
        name = n;
    }
    ~Player() {}
    
    virtual const string& getType() const = 0;
    virtual Move play(const State &s) = 0;

    string getName() const {
        return name;
    }

    friend ostream & operator << (ostream &out, const Player &player) {
        out << player.getType() << " player " << player.getName();

        return out;
    }

private:
    string name;
protected:
    string type;
};

class GreedyPlayer : public Player {
public:
    GreedyPlayer(const string& n)  : Player(n) {
        type = "Greedy";
    }

    const string& getType() const { return type; }

    Move play(const State &s) {
        int h = s.getHeaps(), tmp_max = 0, tmp_max_idx = 0;
        
        for (int i = 0; i < h; ++i) {
            if (tmp_max < s.getCoins(i)) { //Strictly smaller to pick the smallest index in equalities 
                tmp_max = s.getCoins(i);
                tmp_max_idx = i;
            }
        }

        return Move(tmp_max_idx, tmp_max, 0, 0); //Correct way of putting "nothing" back?
    }
};

class SpartanPlayer : public Player {
    public:
        SpartanPlayer(const string& n) : Player(n) {
            type = "Spartan";
        }

        const string& getType() const { return type; }
    
        Move play(const State &s) {
            int h = s.getHeaps(), tmp_max = 0, tmp_max_idx = 0;
            
            for (int i = 0; i < h; ++i) {
                if (tmp_max < s.getCoins(i)) { //Strictly smaller to pick the smallest index in equalities 
                    tmp_max = s.getCoins(i);
                    tmp_max_idx = i;
                }
            }
    
            return Move(tmp_max_idx, 1, 0, 0); //Correct way of putting "nothing" back?
        }
};

class SneakyPlayer : public Player {
    public:
        SneakyPlayer(const string& n)  : Player(n) {
            type = "Sneaky";
        }

        const string& getType() const { return type; }
    
        Move play(const State &s) {
            int h = s.getHeaps(), tmp_min = 1e9, tmp_min_idx = 0;
            
            for (int i = 0; i < h; ++i) {
                if (tmp_min > s.getCoins(i) && s.getCoins(i) != 0) { //Strictly smaller to pick the smallest index in equalities
                    //With sneakies we don't want to pick the 0s as the smallest because then the player would not play*
                    tmp_min = s.getCoins(i);
                    tmp_min_idx = i;
                }
            }

            if (tmp_min == 1e9) tmp_min = 0; //Special case
    
            return Move(tmp_min_idx, tmp_min, 0, 0); //Correct way of putting "nothing" back?
        }
};

class RighteousPlayer : public Player {
    public:
        RighteousPlayer(const string& n)  : Player(n) {
            type = "Righteous";
        }

        const string& getType() const { return type; }
    
        Move play(const State &s) {
            int h = s.getHeaps(), c = 0, tmp_max_idx = 0, tmp_min_idx = 0, tmp_min = 1e9;
            
            for (int i = 0; i < h; ++i) {
                if (c < s.getCoins(i)) { //Strictly smaller to pick the smallest index in equalities 
                    c = s.getCoins(i);
                    tmp_max_idx = i;
                }
                if (tmp_min > s.getCoins(i)) { //*But with righteous players we want to leave the case of 0s in if smaller
                    tmp_min = s.getCoins(i);
                    tmp_min_idx = i;
                }
            }

            if (tmp_min == 1e9) tmp_min = 0; //Special case

            return Move(tmp_max_idx, (c + 1) / 2, tmp_min_idx, (c + 1) / 2 - 1); //Correct way of putting "nothing" back?
        }
};*/

class Game {
public:
    Game(int heaps, int players) {
        H = heaps, P = players;
        lastHidx = 0, lastPidx = 0; //How many assigned so far
        s = nullptr;
        
        this->players = new Player*[P];
        this->heaps = new int[H];
    }
    ~Game() {
        delete[] heaps;
        
        for (int i = 0; i < P; ++i) {
            delete players[i]; //Delete pointer elements
        }
        delete[] players; //Delete entire array of pointer elements
    }

    void addHeap(int coins) { // may throw logic_error
        if (lastHidx == H) throw logic_error("Exceeded heap number");

        heaps[lastHidx++] = coins;
    }

    void addPlayer(Player *player) { // may throw logic_error
        if (lastPidx == P) throw logic_error("Exceeded player number");

        players[lastPidx++] = player; //Just copying the pointer, the actual player instance is somewhere in the heap
    }

    void play(ostream &out) { // may throw logic_error
        //To play if state is empty check and then create it to start the game
        if (s == nullptr) { //State not created yet (this is the first move)
            s = new State(H, heaps, P);
        }

        int playing;
        while (!s->winning()) {
            //Get who's playing from state
            playing = s->getPlaying();

            if (playing == lastPidx) throw logic_error("Accessing nonexistent player");

            //Ask this player from our array to generate a move based on state
            Move v = players[playing]->play(*s);

            //Present the state
            cout << "State: " << *s << endl;

            //Print the move from state and players
            cout << *players[playing] << " " << v << endl;

            //Play the move on state
            s->next(v);
        }

        //Someone won
        cout << "State: " << *s << endl << *players[playing] << " wins" << endl;
        //Present the last state and the LAST player to play from the Player main class printer function and say they won

        delete s;
    }

    int getPlayers() const { return P; }
    const Player *getPlayer(int p) const; // may throw logic_error

private:
    int H, P, lastHidx, lastPidx;
    int* heaps;
    State* s;
    Player** players; //Array of pointers
};

/*int main() {
    Game specker(3, 4);
    specker.addHeap(10);
    specker.addHeap(20);
    specker.addHeap(17);
    specker.addPlayer(new SneakyPlayer("Tom"));
    specker.addPlayer(new SpartanPlayer("Mary"));
    specker.addPlayer(new GreedyPlayer("Alan"));
    specker.addPlayer(new RighteousPlayer("Robin"));
    specker.play(cout);

    return 0;
}*/