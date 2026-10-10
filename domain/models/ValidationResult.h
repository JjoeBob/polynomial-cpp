//
// Created by Cowabunga on 03.10.2026.

#ifndef POLYNOMIAL_VALIDATERESULT_H
#define POLYNOMIAL_VALIDATERESULT_H

/**
 * @brief Код ошибки валидации
 */
enum class ValidationError
{
    None,
    ConsecutiveOperators,
    InvalidCharacter,
    PowerNotDecreasing,
    ExpectedDigitAfterPower,
    MisplacedOperator
};

/**
 * @brief Результат проверки символа валидатором
 */
struct ValidationResult
{
    bool isValid;
    ValidationError error;
};


#endif //POLYNOMIAL_VALIDATERESULT_H
