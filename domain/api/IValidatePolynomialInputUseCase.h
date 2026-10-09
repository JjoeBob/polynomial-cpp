//
// Created by Cowabunga on 03.10.2026.
//

#ifndef POLYNOMIAL_IVALIDATEPOLYNOMIALINPUTUSECASE_H
#define POLYNOMIAL_IVALIDATEPOLYNOMIALINPUTUSECASE_H

#include "../models/ValidationResult.h"
#include <string>
#include "../models/Polynomial.h"

class IValidatePolynomialInputUseCase
{
public:
    virtual ~IValidatePolynomialInputUseCase() = default;

    virtual ValidationResult validatePolynomialInput(const std::string& input, char character) = 0;
    virtual bool isExpressionFinished(const std::string& input) = 0;
};


#endif //POLYNOMIAL_IVALIDATEPOLYNOMIALINPUTUSECASE_H
