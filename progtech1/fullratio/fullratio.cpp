#include <iostream>

#ifndef CONTEST
#include "fullratio.hpp"
#endif
using namespace std;

rational::rational(int n, int d) {
    int gcd = rational::gcd(n, d);

    nom = n / gcd, den = d / gcd;
    
    if (nom > 0 && den < 0) {
        nom = -nom;
        den = -den;
    }
}

rational operator+(const rational &x, const rational &y) {
    int new_nom = x.nom * y.den + y.nom * x.den;
    int new_den = x.den * y.den;

    int gcd = rational::gcd(new_nom, new_den);
    new_nom /= gcd, new_den /= gcd;
    if (new_nom > 0 && new_den < 0) {
        new_nom = -new_nom;
        new_den = -new_den;
    }

    return rational(new_nom, new_den);
}

rational operator-(const rational &x, const rational &y) {
    return x + rational(-y.nom, y.den);
}

rational operator*(const rational &x, const rational &y) {
    int new_nom = x.nom * y.nom;
    int new_den = x.den * y.den;

    int gcd = rational::gcd(new_nom, new_den);
    new_nom /= gcd, new_den /= gcd;
    //cout << "After GCD: " << new_nom << " " << new_den << endl;
    if (new_nom > 0 && new_den < 0) {
        new_nom = -new_nom;
        new_den = -new_den;
    }

    return rational(new_nom, new_den);
}

rational operator/(const rational &x, const rational &y) {
    return x * rational(y.den, y.nom);
}

std::ostream &operator<<(std::ostream &out, const rational &x) {
    out << x.nom << "/" << x.den;

    return out;
}

int rational::gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

/*int main() {
    rational a(3, 8);
    rational b(-2, 11);
    cout << a * b;

    return 0;
}*/