#include <iostream>
#include <cmath>
using namespace std;

class Polynomial {
protected:
    class Term { //Class Term INSIDE a class Polynomial, since that's where it belongs and should be used!
        //So it can only be used locally INSIDE the Polynomial class, but not outside of it!
    public:
        int exponent;
        int coefficient;
        Term *next;

        Term(int exp, int coeff, Term *n = nullptr)
            : exponent(exp), coefficient(coeff), next(n) {}

        friend class Polynomial;
    };

    Term *head;

    void clear() {
        while (head) {
            Term *temp = head;
            head = head->next; //First get the next link in the chain
            delete temp; //Then delete current
        }
    }

    void copyFrom(const Polynomial &p) {
        Term *current = p.head;
        while (current) {
            addTerm(current->exponent, current->coefficient);
            current = current->next;
        }
    }

public:
    Polynomial() : head(nullptr) {}

    Polynomial(const Polynomial &p) : head(nullptr) {
        copyFrom(p);
    }

    ~Polynomial() {
        clear();
    }

    Polynomial &operator=(const Polynomial &p) {
        if (this != &p) { //If it's the same polynomial already in here, there's no need to copy it again
            clear();
            copyFrom(p);
        }
        return *this;
    }

    void addTerm(int expon, int coeff) {
        if (coeff == 0) return; //0 coeff means that x^y term does not exist

        Term **current = &head; //Pointer to the nodes (Term* pointers)

        //First step: traverse the list and skip the nodes with exponenent > this exponent, since we want the terms sorted.
        while (*current && (*current)->exponent > expon) { //*current is not the node pointers which need to be not null
            current = &((*current)->next);
        }

        //Step 2: if we find a term of the same exponent, subtract/add the coefficient
        if (*current && (*current)->exponent == expon) {
            (*current)->coefficient += coeff;
            
            //But adding a term with the same exponent can mean that the coefficient becomes 0!
            //Then we have to remove this node
            if ((*current)->coefficient == 0) {
                Term *temp = *current;
                *current = (*current)->next; //Connect the next node in the chain to this link and then delete this node
                delete temp;
            }
        }
        else { //Otherwise if there's no term of the same expoenent, create it now
            *current = new Term(expon, coeff, *current); //The Term constructor will by default connect the next pointer in the chain
        }
    }

    double evaluate(double x) {
        double result = 0.0;
        Term *current = head;
        
        while (current) {
            result += current->coefficient * pow(x, current->exponent);
            current = current->next;
        }
        
        return result;
    }

    friend Polynomial operator+(const Polynomial &p, const Polynomial &q) {
        Polynomial result;
        Term *a = p.head, *b = q.head;

        while (a || b) { //May not have the same number of nodes! Keep on going while either one is on, just like merging two sorted arrays!
            //Which is pretty much what this is!
            if (a && (!b || a->exponent > b->exponent)) { //If a has a node and b finished or b's next largest node is of lower exp than a's
                //Process ONLY a's largest node for the result
                result.addTerm(a->exponent, a->coefficient);
                a = a->next;
            } else if (b && (!a || b->exponent > a->exponent)) { //Conversely, if b has a node and a doesnt or a's node is smaller, process
                //JUST b for the result
                result.addTerm(b->exponent, b->coefficient);
                b = b->next;
            } else { //Both a and b still have nodes, AND their largest nodes are both of equal exponent!
                //In this case we add them
                //If the result is 0, addTerm() auto-rejects the node and we move on
                result.addTerm(a->exponent, a->coefficient + b->coefficient);
                a = a->next;
                b = b->next;
            }
        }

        return result;
    }

    friend Polynomial operator*(const Polynomial &p, const Polynomial &q) {
        Polynomial result;

        //Concise for loop for linked lists. Initialize list; while still going; advance node.
        for (Term *a = p.head; a; a = a->next) {
            for (Term *b = q.head; b; b = b->next) {
                result.addTerm(a->exponent + b->exponent, a->coefficient * b->coefficient);
            }

            //Add all combinations of nodes of a and b (epimeristiki).
            //Resultant nodes of same power will be handled by addTerm()
            //While the order will also remain sorted thanks to addTerm()
            //This is a bit inefficient, maybe it can be optimized using matrices or w/e but for now its ok
        }
        return result;
    }

    friend ostream &operator<<(ostream &out, const Polynomial &p) { //Interesting but too many special cases....
        Term *current = p.head;
        if (!current) { //Special case: an empty polynomial is just 0
            out << "0";
            return out;
        }

       bool first = true; //Need this flag just in case the first term is positive, so as not to output a +

        while (current) {
            if (current->coefficient > 0 && !first) //Coeff can't be 0
                out << "+ ";
            else if (current->coefficient < 0)
                out << "- ";

            int absCoeff = abs(current->coefficient);

            //If the coefficient is not 1, surely we print it.
            //Also if the exponent is 0 then surely we print the coefficient (just it) including the case where coeff = 1
            if (absCoeff != 1 || current->exponent == 0)
                out << absCoeff;

            //And if there is an expoenent, we also print it.
            if (current->exponent > 0) { //Equivalent to != 0 because there are no neegative exponents
                out << "x";
                if (current->exponent > 1) //If the exponent is 1, there's no need for ^1
                    out << "^" << current->exponent;
            }

            current = current->next;

            if (current) out << " "; //Dont print a space in the end

            if (first) first = false;
        }

        return out;
    }
};

/*int main() {
    Polynomial p;
    p.addTerm(1, 3); // 3x
    
     p.addTerm(2, 1); // x^2
     p.addTerm(0, -1); // -1
    
     Polynomial q(p); // x^2 + 3x - 1
     q.addTerm(1, -3); // -3x
    
     cout << "P(x) = " << p << endl;
     cout << "P(1) = " << p.evaluate(1) << endl;
     cout << "Q(x) = " << q << endl;
     cout << "Q(1) = " << q.evaluate(1) << endl;
     cout << "(P+Q)(x) = " << p+q << endl;
     cout << "(P*Q)(x) = " << p*q << endl;

     return 0;
}*/