//
// Created by Cowabunga on 02.10.2026.
//

#ifndef POLYNOMIAL_IPOLYNOMIALPARSERUSECASE_H
#define POLYNOMIAL_IPOLYNOMIALPARSERUSECASE_H
#include <string>
#include "../models/Polynomial.h"

class IPolynomialParserUseCase
{
public:
    virtual ~IPolynomialParserUseCase() = default;

    virtual Polynomial parse(const std::string& input) = 0;
};


#endif //POLYNOMIAL_IPOLYNOMIALPARSERUSECASE_H
