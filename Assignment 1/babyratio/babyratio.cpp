#ifndef CONTEST
#include "babyratio.hpp"
#endif
#include <iostream>
using namespace std;

rational::rational(int n, int d) {
    int gcd = rational::gcd(n, d);

    nom = n / gcd, den = d / gcd;
}

rational rational::add(rational r) {
    int new_nom = nom * r.den + r.nom * den;
    int new_den = den * r.den;

    int gcd = rational::gcd(new_nom, new_den);

    return rational(new_nom / gcd, new_den / gcd);
}

rational rational::sub(rational r) {
    return add(rational(-r.nom, r.den));
}

rational rational::mul(rational r) {
    int new_nom = nom * r.nom;
    int new_den = den * r.den;

    int gcd = rational::gcd(new_nom, new_den);

    return rational(new_nom / gcd, new_den / gcd);
}

rational rational::div(rational r) {
    return mul(rational(r.den, r.nom));
}

void rational::print() {
    if (nom > 0 && den < 0)
        nom = -nom, den = -den;
    cout << nom << "/" << den;
}

int rational::gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

/*
int main() {
    rational a(-31, 3);
    rational b(66, -5);
    a.mul(b).print();

    return 0;
}*/