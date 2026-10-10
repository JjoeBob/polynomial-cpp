//
// Created by Cowabunga on 03.10.2026.
//

#include "ValidatePolynomialInputUseCase.h"

#include "../api/IValidatePolynomialInputUseCase.h"
#include <limits>

namespace
{
    enum class State
    {
        Start,
        Coefficient,
        Variable,
        PowerOperator,
        PowerValue,
        ZeroConst
    };

    struct FsmContext
    {
        State state = State::Start;
        int lastPower = std::numeric_limits<int>::max();
        int currentPower = 0;
        bool signAllowed = true;
    };


    /** @brief Обрабатывает состояние Start автомата
     *
     * В начале мы можем ввести:
     *
     * - '0' - тогда автомат перейдет в состояние
     * State::ZeroConst (и больше ничего нельзя будет ввести)
     *
     * - '+' или '-' - переключаем флаг singAllowed в false (запрет на ввод 2
     * знаков подряд). Состояние не меняем (остается State::Start)
     *
     * - 1 <= digit <= 9 - переход автомата в состояние State::Coefficient
     *
     * - Любой другой символ - возвращается ValidationError::InvalidCharacter
     *
     * @param c Проверяемый символ
     * @param context Контекст автомата с текущим состоянием и степенями
     * @return ValidationError Код ошибки или ValidationError::None, если символ допустим
     */
    ValidationError handleStart(const char c, FsmContext& context)
    {
        if (c == '0')
        {
            context.state = State::ZeroConst;
            return ValidationError::None;
        }
        if (c >= '1' && c <= '9')
        {
            context.state = State::Coefficient;
        }
        else if (c == 'x')
        {
            if (context.lastPower <= 1)
            {
                return ValidationError::PowerNotDecreasing;
            }
            context.state = State::Variable;
        }
        else if (c == '+' || c == '-')
        {
            if (context.signAllowed)
            {
                context.signAllowed = false;
            }
            else
            {
                return ValidationError::ConsecutiveOperators;
            }
        }
        else if (c == '^')
        {
            return ValidationError::MisplacedOperator;
        }
        else
        {
            return ValidationError::InvalidCharacter;
        }
        return ValidationError::None;
    }

    /**
     *  @brief Обрабатывает состояние Coefficient автомата
     *
     * Если введен(а):
     *
     * - Цифра - ошибки нет, состояние не меняется
     *
     * - 'x' - рассматривается прошлая степень. Если она <= 1, то
     * метод возвращает ValidationError::PowerNotDecreasing, тк степень не убывает
     * (запись "5x + 3x" недопустима, тк по заданию: "Пользователь вводит полином по УМЕНЬШЕНИЮ степеней").
     * Если же прошлая степень > 1, то символ допустим, состояние автомата меняется на State::Variable
     *
     * - '+' или '-' - то текущий член - свободный. Возвращается ValidationError::PowerNotDecreasing, тк
     * после свободного члена не может быть других членов
     *
     * - '^' - возвращается ошибка ValidationError::MisplacedOperator, тк можно возводить в степень
     * только x. Запись вида "123^2" недопустима
     *
     * - Любой другой символ - возвращается ошибка ValidationError::InvalidCharacter
     *
     * @param c Проверяемый символ
     * @param context Контекст автомата с текущим состоянием и степенями
     * @return ValidationError код ошибки или ValidationError::None, если символ допустим
     */
    ValidationError handleCoefficient(const char c, FsmContext& context)
    {
        if (std::isdigit(c))
        {
            return ValidationError::None;
        }
        if (c == 'x')
        {
            if (context.lastPower <= 1)
            {
                return ValidationError::PowerNotDecreasing;
            }
            context.state = State::Variable;
            return ValidationError::None;
        }
        if (c == '+' || c == '-')
        {
            return ValidationError::PowerNotDecreasing;
        }
        if (c == '^')
        {
            return ValidationError::MisplacedOperator;
        }
        return ValidationError::InvalidCharacter;
    }

    /**
     * @brief Обрабатывает состояние Variable автомата
     *
     * Если введен(а):
     *
     * - '^' - проверяется прошлая степень. Если она <= 2, то возвращается ошибка
     * ValidationError::PowerNotDecreasing. Пример: была введена строка "3x^2+2x". Тут есть 2 случая:
     *  1. Мы попытаемся ввести "3x^2+2x^1" или "3x^2+2x^0" - степень здесь лишняя, поэтому ее в целом
     * можем не допускать для ввода
     *  2. Мы попытаемся ввести "3x^2+2x^2" или "3x^2+2x^3" - очевидная ошибка, тк степень не убывает
     *
     * - '+' или '-' - мы не написал знак '^', значит степень = 1. Возвращается ValidationError::None
     *
     * - Любой другой символ - возвращается ошибка ValidationError::InvalidCharacter
     *
     * @param c Проверяемый символ
     * @param context Контекст автомата с текущим состоянием и степенями
     * @return ValidationError код ошибки или ValidationError::None, если символ допустим
     */
    ValidationError handleVariable(const char c, FsmContext& context)
    {
        if (c == '^')
        {
            if (context.lastPower <= 2)
            {
                return ValidationError::PowerNotDecreasing;
            }
            context.state = State::PowerOperator;
            return ValidationError::None;
        }
        else if (c == '+' || c == '-')
        {
            context.lastPower = 1;
            context.signAllowed = false;
            context.state = State::Start;
            return ValidationError::None;
        }
        return ValidationError::InvalidCharacter;
    }

    /**
     * @brief Обрабатывает состояние PowerOperator автомата
     *
     * Если введен(а):
     *
     * - '0' - нет смысла в записи "x^0", поэтому сразу возвращается
     * ValidationError::InvalidCharacter
     *
     * - Цифра кроме '0' - если введенная цифра УЖЕ больше, чем прошлая степень, то
     * возвращается ошибка ValidationError::PowerNotDecreasing. Иначе обновляем текущую степень,
     * переводим автомат в состояние State::PowerValue (возвращаем ValidationError::None)
     *
     * - '^' или '+' или '-' - возращается ошибка
     * ValidationError:MisplacedOperator (пример: "2x^+")
     *
     * @param c Проверяемый символ
     * @param context Контекст автомата с текущим состоянием и степенями
     * @return ValidationError код ошибки или ValidationError::None, если символ допустим
     */
    ValidationError handlePowerOperator(const char c, FsmContext& context)
    {
        if (std::isdigit(c))
        {
            if (c == '0')
            {
                return ValidationError::InvalidCharacter;
            }
            int digit = c - '0';

            if (digit >= context.lastPower)
            {
                return ValidationError::PowerNotDecreasing;
            }
            context.currentPower = digit;
            context.state = State::PowerValue;
            return ValidationError::None;
        }
        if (c == '^' || c == '+' || c == '-')
        {
            return ValidationError::MisplacedOperator;
        }
        return ValidationError::InvalidCharacter;
    }

    /**
     * @brief Обрабатывает состояние PowerValue автомата
     *
     * Если введен(а):
     *
     * - Цифра - вычисляем обновленную степень путем умножения текущей на 10
     * и прибавления текущей цифры. Если обновленная степень больше, чем последняя,
     * то возвращаем ошибку ValidationError::PowerNotDecreasing. Обновляем текущую степень
     * и остаемся в состоянии State::PowerValue
     *
     * - '+' или '-' - есть только проверка context.currentPower < 2 (возвращается
     * ValidationError::PowerNotDecreasing), чтобы запретить ввод выражения вида "x^1+"
     *
     * - '^' - возвращается ошибка ValidationError::MisplacedOperator
     *
     * - Любой другой символ - ValidationError::InvalidCharacter
     *
     * @param c Проверяемый символ
     * @param context Контекст автомата с текущим состоянием и степенями
     * @return ValidationError код ошибки или ValidationError::None, если символ допустим
     */
    ValidationError handlePowerValue(const char c, FsmContext& context)
    {
        if (std::isdigit(c))
        {
            int updatedPower = context.currentPower * 10 + (c - '0');
            if (updatedPower >= context.lastPower)
            {
                return ValidationError::PowerNotDecreasing;
            }
            context.currentPower = updatedPower;
        }
        else if (c == '+' || c == '-')
        {
            if (context.currentPower < 2)
            {
                return ValidationError::PowerNotDecreasing;
            }
            context.lastPower = context.currentPower;
            context.currentPower = 0;
            context.state = State::Start;
            context.signAllowed = false;
        }
        else if (c == '^')
        {
            return ValidationError::MisplacedOperator;
        }
        else
        {
            return ValidationError::InvalidCharacter;
        }
        return ValidationError::None;
    }


    /** @brief Обрабатывает состояние автомата State::ZeroConst
     *
     * @param c Проверяемый символ
     * @param context Контекст автомата с текущим состоянием и степенями
     * @return ValidationError Код ошибки
     *
     * @note Никогда не вернет ValidationError::None, следовательно, ввод после '0'
     * в принципе невозможен. Обработка делается только чтобы узнать конкретную
     * причину ошибки
     */
    ValidationError handleZeroConst(const char c, FsmContext& context)
    {
        if (c == '+' || c == '-')
        {
            return ValidationError::PowerNotDecreasing;
        }
        if (c == '^')
        {
            return ValidationError::MisplacedOperator;
        }
        return ValidationError::InvalidCharacter;
    }


    /** @brief Получает символ и переводит автомат в следующее состояние
     *
     * @param c Проверяемый символ
     * @param context Контекст автомата с текущим состоянием и степенями
     * @return ValidationError Код ошибки или ValidationError::None, если символ допустим
     */
    ValidationError processCharacter(const char c, FsmContext& context)
    {
        switch (context.state)
        {
        case State::Start: return handleStart(c, context);
        case State::Coefficient: return handleCoefficient(c, context);
        case State::Variable: return handleVariable(c, context);
        case State::PowerOperator: return handlePowerOperator(c, context);
        case State::PowerValue: return handlePowerValue(c, context);
        case State::ZeroConst: return handleZeroConst(c, context);
        }
        return ValidationError::InvalidCharacter;
    }
}


ValidationResult ValidatePolynomialInputUseCase::validatePolynomialInput(const std::string& input, char character)
{
    const std::string newInput = input + character;
    FsmContext context;

    for (const char c : newInput)
    {
        ValidationError error = processCharacter(c, context);
        if (error != ValidationError::None)
        {
            return {.isValid = false, .error = error};
        }
    }
    return {.isValid = true, .error = ValidationError::None};
}

bool ValidatePolynomialInputUseCase::isExpressionFinished(const std::string& input)
{
    if (input.empty())
    {
        return false;
    }

    FsmContext context;
    for (char c : input)
    {
        if (processCharacter(c, context) != ValidationError::None)
        {
            return false;
        }
    }
    return context.state == State::Coefficient ||
        context.state == State::Variable ||
        context.state == State::ZeroConst ||
        (context.state == State::PowerValue && context.currentPower >= 2);
}
