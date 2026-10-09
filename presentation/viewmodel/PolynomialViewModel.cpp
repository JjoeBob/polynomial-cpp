//
// Created by Cowabunga on 02.10.2026.
//

#include "PolynomialViewModel.h"
#include <string>

PolynomialViewModel::PolynomialViewModel(
    IPolynomialParserUseCase& parser,
    IValidatePolynomialInputUseCase& validator,
    IRasterizePolynomialUseCase& rasterizer)
    : m_parser(parser),
      m_validator(validator),
      m_rasterizer(rasterizer)
{
}

bool PolynomialViewModel::onCharTyped(char c)
{
    ValidationResult result = m_validator.validatePolynomialInput(m_currentInput, c);
    if (result.isValid)
    {
        m_currentInput += c;
        m_lastValidChar.setValue(c);
        return true;
    }
    return false; // можно также передавать ошибку
}

void PolynomialViewModel::onEnterPressed()
{
    if (m_validator.isExpressionFinished(m_currentInput))
    {
        m_polynomial.setValue(m_parser.parse(m_currentInput));
        m_rasterizer.rasterize(*m_polynomial.getValue()); // TODO: что-то делать с двумерным массивом-графиком
    }
}

Observable<char>& PolynomialViewModel::observeLastValidChar()
{
    return m_lastValidChar;
}

Observable<std::optional<Polynomial>>& PolynomialViewModel::observePolynomial()
{
    return m_polynomial;
}
