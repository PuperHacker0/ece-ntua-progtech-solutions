#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class ChessBoardArray { //By default a square matrix with the same row and column base indexing
protected:
    class Row {
    public:
        Row(ChessBoardArray &a, int i) : //No check for i, assumed to be in range with base 0
            cb(a), row(i) {}
        
        int & operator[](int i) const {
            return cb.select(row, i); //also assumed to be in-bounds, this selects cb[i][j], so an element
            //So in total, a[2][3] calls the [] on the chessboard, which returns a Row, then we call the [] on the Row which
            //returns an int!
        }
    private:
        ChessBoardArray &cb;
        int row;
    };

    class ConstRow {
    public:
        ConstRow(const ChessBoardArray &a, int i) : //const chessboard!
            cb(a), row(i) {}
        
        int & operator[](int i) const { //const function!
            return cb.select(row, i);
        }

    private:
        const ChessBoardArray &cb; //const chessboard!
        int row;
    };

public:
    ChessBoardArray(unsigned size = 0, unsigned base = 0) :
        data(new int[(size * size + 1) / 2]), base(base), size(size) { //+1 to account for odd-edged tables
            for (int i = 0; i < (size * size + 1) / 2; ++i)
                data[i] = 0;
        } //By default sets everything to all 0s
        //Create a 1D linear array in memory, where we will store the data using a 2D->1D map of indices
        
    ChessBoardArray(const ChessBoardArray &a) :
        data(new int[(a.size * a.size + 1) / 2]), base(a.base), size(a.size) {
        
        for (unsigned i = 0; i < (size * size + 1) / 2; ++i) //Copy the 1D data array
            data[i] = a.data[i];
    }
    ~ChessBoardArray() { delete [] data; } //Deletes the array and the pointer too.

    ChessBoardArray & operator = (const ChessBoardArray &a) {
        delete [] data; //Delete the previous data so as to prevent a memleak before creating the new array
        
        size = a.size, base = a.base;
        data = new int[(size * size + 1) / 2];
        
        for (unsigned i = 0; i < (size * size + 1) / 2; ++i)
            data[i] = a.data[i];

        return *this; //Return a reference to this object, because we might want to do some_function(chessboardA = chessboardB) instantly
    }

    int & select(int i, int j) {
        return data[loc(i, j)]; //if the location is not correct, this will automatically throw an out of range error
    }

    int & select(int i, int j) const {
        return data[loc(i, j)];
    }

    Row operator [] (int i) { //chessboard[] returns a Row
        return Row(*this, i);
    }

    ConstRow operator [] (int i) const { //Same function but for const tables, so the function is required to be const too
        return ConstRow(*this, i);
    }

    friend ostream & operator << (ostream &out, const ChessBoardArray &a) {
        //Use a spacing of 4 characters for each print
        for (int i = a.base; i < a.base + a.size; ++i) {
            for (int j = a.base; j < a.base + a.size; ++j) {
                int pure_row = i - a.base, pure_column = j - a.base;

                out << setw(4) << (((pure_row % 2) ^ (pure_column % 2)) ? 0 : a.data[a.loc(i, j)]);
            }
            
            out << endl;
        }

        return out;
    }

protected:
    int *data;
    int base, size;
    
    unsigned loc(int i, int j) const { // may throw out_of_range
        //Access elements through this function so that we don't always have to check whether an index is within bounds
        int base0idxi = i - base, base0idxj = j - base;

        //First check if out of bounds
        if (base0idxi < 0 || base0idxi >= size || base0idxj < 0 || base0idxj >= size)
            throw out_of_range("invalid index");
        
        //Then check that we're not writing a black square
        if ((i % 2) ^ (j % 2))
            throw out_of_range("invalid index " + to_string(i) + "x" + to_string(j));
        /*Why this works: the white squares are located in even rows and even columns or odd rows and odd columns
        xoring means that if we have an even-odd or odd-even combination, it is inaccessible. Otherwise it is accessible.*/
        
        if (size % 2 == 0) //On even-sized tables
            return (base0idxi * size + base0idxj) / 2; //Convert to 1D index in memory
        //ONLY WHITE SQUARES WILL BE EVALUATED BY THIS FORMULA, THEY ARE ALL DIVISIBLE BY 2
        else //odd-sized table
            return ((base0idxi % 2 == 0 ? base0idxi * size : base0idxi * size + 1) + base0idxj) / 2;
    }
};

/*int main() {
    ChessBoardArray a(5, 2); // size 4x4, rows and columns numbered from 1
    a[3][5] = 42;
    a[4][6] = 17;
    
    try { a[2][1] = 7; }
    catch(out_of_range &e) { cout << "a[2][1] is black" << endl; }
    
    try { cout << a[1][2] << endl; }
    catch(out_of_range &e) { cout << "and so is a[1][2]" << endl; }
    
    cout << a;

    return 0;
}*/