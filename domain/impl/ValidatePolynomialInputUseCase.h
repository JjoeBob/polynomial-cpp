//
// Created by Cowabunga on 03.10.2026.
//

#ifndef POLYNOMIAL_VALIDATEPOLYNOMIALINPUTUSECASE_H
#define POLYNOMIAL_VALIDATEPOLYNOMIALINPUTUSECASE_H
#include "../api/IValidatePolynomialInputUseCase.h"


class ValidatePolynomialInputUseCase : public IValidatePolynomialInputUseCase
{
public:
    ValidationResult validatePolynomialInput(const std::string& input, char character) override;
    bool isExpressionFinished(const std::string& input) override;
};


#endif //POLYNOMIAL_VALIDATEPOLYNOMIALINPUTUSECASE_H
