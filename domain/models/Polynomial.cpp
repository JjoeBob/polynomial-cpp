//
// Created by Cowabunga on 02.10.2026.
//

#include "Polynomial.h"

#include <iomanip>
#include <ostream>

Polynomial::Polynomial(const std::map<int, double>& terms) : m_terms(terms)
{
}

double Polynomial::getCoefficient(int degree) const
{
    std::map<int, double>::const_iterator it = m_terms.find(degree);

    if (it != m_terms.end())
    {
        return it->second;
    }
    return 0.0;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& poly)
{
    if (poly.m_terms.empty())
    {
        return os << "0 : 0" << std::endl;
    }
    int maxDegree = poly.m_terms.rbegin()->first;
    if (maxDegree < 0)
    {
        maxDegree = 0;
    }

    for (int degree = maxDegree; degree >= 0; --degree)
    {
        std::string label;
        if (degree > 1)
        {
            label = "X^" + std::to_string(degree);
        }
        else if (degree == 1)
        {
            label = "X";
        }
        else
        {
            label = "0";
        }

        os << std::left << std::setw(6) << label << " : " << poly.getCoefficient(degree) << "\n";
    }

    return os;
}

double Polynomial::evaluate(double x) const
{
    double result = 0.0;
    for (const auto& [degree, coeff] : m_terms)
    {
        result += coeff * std::pow(x, degree);
    }
    return result;
}
