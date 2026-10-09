//
// Created by Cowabunga on 02.10.2026.
//

#ifndef POLYNOMIAL_POLYNOMIAL_H
#define POLYNOMIAL_POLYNOMIAL_H
#include <map>


class Polynomial
{
public:
    Polynomial(const std::map<int, double>&);
    double getCoefficient(int degree) const;
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    double evaluate(double x) const;

private:
    std::map<int, double> m_terms;
};


#endif //POLYNOMIAL_POLYNOMIAL_H
