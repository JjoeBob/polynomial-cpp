//
// Created by Cowabunga on 02.10.2026.
//

#ifndef POLYNOMIAL_POLYNOMIALPARSERUSECASE_H
#define POLYNOMIAL_POLYNOMIALPARSERUSECASE_H

#include "../api/IPolynomialParserUseCase.h"
#include <vector>
#include <string>
#include <utility>

class PolynomialParserUseCase : public IPolynomialParserUseCase
{
    friend void testSplitIntoTerms();

public:
    Polynomial parse(const std::string& input) override;

private:
    std::vector<std::string> splitIntoTerms(const std::string& expression);
    std::pair<int, double> parseTerm(const std::string& rawTerm);
};

#endif //POLYNOMIAL_POLYNOMIALPARSERUSECASE_H
