//
// Created by Cowabunga on 03.10.2026.
//

#ifndef POLYNOMIAL_VALIDATEPOLYNOMIALINPUTUSECASE_H
#define POLYNOMIAL_VALIDATEPOLYNOMIALINPUTUSECASE_H
#include "../api/IValidatePolynomialInputUseCase.h"


class ValidatePolynomialInputUseCase : public IValidatePolynomialInputUseCase
{
public:

    /**
     * @brief Проверяет допустипость добавления нового символа к текущей строке ввода многочлена
     *
     * @param input Текущая строка ввода многочлена
     * @param character Новый рассматриваемый символ
     * @return ValidationResult с флагом допустимости и кодом ошибки
     */
    ValidationResult validatePolynomialInput(const std::string& input, char character) override;

    /**
     * @brief Проверяет, является ли входная строка завершенным многочленом
     *
     * Метод проходит посимвольно с начала строки. Ввод считается завершенным,
     * если не было найдено ошибок и выражение не обрывается на операторе
     *
     * @param input  Строка ввода многочлена для валидации
     * @return true, если выражение является завершенным; false,
     * если содержит ошибки или обрывается на операторе (+, -, ^)
     *
     * @note Метод возвращает false, если ввести член вида x^1 или x^0
     */
    bool isExpressionFinished(const std::string& input) override;
private:

};


#endif //POLYNOMIAL_VALIDATEPOLYNOMIALINPUTUSECASE_H
