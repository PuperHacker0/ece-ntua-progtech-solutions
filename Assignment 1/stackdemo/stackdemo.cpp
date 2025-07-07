#include <iostream>
using namespace std;

//ΔΕΝ ΚΑΝΕΙ DEEP COPY CONTENTS ΑΠΟ POINTERS ΠΧ STRINGS
template <typename T>
class stack {
public:
    stack(int size) {
        capacity = size;
        top = -1; //No element added yet, so index 0 is not the top
        elements = new T[capacity];
    }

    stack(const stack<T>& s) {
        capacity = s.capacity;
        top = s.top;

        elements = new T[capacity];
        for (int i = 0; i <= top; i++) {
            elements[i] = s.elements[i];
        }
    }

    ~stack() {
        delete[] elements;
    }

    const stack<T>& operator=(const stack<T>& s) {
        if (this != &s) {
            delete[] elements; //Free previously allocated memory before copying new table

            capacity = s.capacity;
            top = s.top;
            elements = new T[capacity];

            for (int i = 0; i <= top; i++) {
                elements[i] = s.elements[i];
            }
        }

        return *this; //Since we're asked to return something
    }

    bool empty() const {
        return top == -1; //No elements left
    }

    void push(const T& x) {
        if (top + 1 == capacity) { //First resize the stack to fit the new element
            T* new_elem = new T[capacity + 10]; //Increase by 10 elements each time

            for (int i = 0; i <= top; i++) {
                new_elem[i] = elements[i];
            }

            delete[] elements;
            elements = new_elem;
            capacity +=10;

            //new_elem will be destroyed after this block ends so no memory leak
        }

        elements[++top] = x;
    }

    T pop() {
        //Κάνουμε assume ότι είναι οκ το size, δεν ζητούνται έλεγχοι
        return elements[top--];
    }

    int size() const {
        return top + 1;
    }

    friend std::ostream& operator<<(std::ostream& out, const stack<T>& s) {
        out << "[";
        for (int i = 0; i < s.top; i++) {
            out << s.elements[i] << ", ";
        }

        if (s.top >= 0) out << s.elements[s.top]; //Ξεχωριστά για να μην βάλει κενό στο τελευταίο
        out << "]";
        
        return out;
    }

private:
        int capacity = 0;
        int top = -1;
        T* elements;
};

/*int main() {
    stack<int> s(10);
    s.push(42);
    cout << s;

    return 0;
}*/