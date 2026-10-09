//
// Created by Cowabunga on 02.10.2026.
//

#ifndef POLYNOMIAL_POLYNOMIALVIEWMODEL_H
#define POLYNOMIAL_POLYNOMIALVIEWMODEL_H
#include "../../domain/api/IPolynomialParserUseCase.h"
#include "../../domain/api/IValidatePolynomialInputUseCase.h"
#include "../../domain/api/IRasterizePolynomialUseCase.h"
#include <string>
#include "../../core/Observable.h"

class PolynomialViewModel
{
public:
    PolynomialViewModel(
        IPolynomialParserUseCase& parser,
        IValidatePolynomialInputUseCase& validator,
        IRasterizePolynomialUseCase& rasterizer
    );
    bool onCharTyped(char c);
    void onEnterPressed();
    Observable<char>& observeLastValidChar();
    Observable<std::optional<Polynomial>>& observePolynomial();

private:
    IPolynomialParserUseCase& m_parser;
    IValidatePolynomialInputUseCase& m_validator;
    IRasterizePolynomialUseCase& m_rasterizer;
    std::string m_currentInput;
    Observable<char> m_lastValidChar;
    Observable<std::optional<Polynomial>> m_polynomial;
};

#endif //POLYNOMIAL_POLYNOMIALVIEWMODEL_H
