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

    ValidationError handleStart(char c, FsmContext& context)
    {
        if (c == '0')
        {
            context.state = State::ZeroConst;
            return ValidationError::None;
        }
        else if (c >= '0' && c <= '9')
        {
            context.state = State::Coefficient;
        }
        else if (std::isdigit(c))
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

    ValidationError handleCoefficient(char c, FsmContext& context)
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
        else if (c == '+' || c == '-')
        {
            return ValidationError::PowerNotDecreasing;
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

    ValidationError handleVariable(char c, FsmContext& context)
    {
        if (c == '^')
        {
            if (context.lastPower <= 2)
            {
                return ValidationError::PowerNotDecreasing;
            }
            context.state = State::PowerOperator;
        }
        else if (c == '+' || c == '-')
        {
            if (1 < context.lastPower)
            {
                context.lastPower = 1;
                context.signAllowed = false;
                context.state = State::Start;
            }
            else
            {
                return ValidationError::PowerNotDecreasing;
            }
        }
        else
        {
            return ValidationError::InvalidCharacter;
        }
        return ValidationError::None;
    }

    ValidationError handlePowerOperator(char c, FsmContext& context)
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

    ValidationError handlePowerValue(char c, FsmContext& context)
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
            if (context.currentPower < 2 || context.currentPower >= context.lastPower)
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

    ValidationError handleZeroConst(char c, FsmContext& context)
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

    ValidationError processCharacter(char c, FsmContext& context)
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
    std::string newInput = input + character;
    FsmContext context;

    for (char c : newInput)
    {
        ValidationError error = processCharacter(c, context);
        if (error != ValidationError::None)
        {
            return {false, error};
        }
    }
    return {true, ValidationError::None};
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
