//
// Created by Cowabunga on 02.10.2026.
//

#include "../impl/PolynomialParserUseCase.h"
#include <vector>
#include <string>
#include <map>

Polynomial PolynomialParserUseCase::parse(const std::string& input)
{
    std::map<int, double> termsMap;
    std::vector<std::string> terms = splitIntoTerms(input);

    for (const std::string& term : terms)
    {
        std::pair<int, double> resultTerm = parseTerm(term);
        termsMap.insert(resultTerm); // если степени не могут повторяться. Если могут, то пишем termsMap[resultTerm.first] += resultTerm.second;
    }

    return Polynomial(termsMap);
}

std::vector<std::string> PolynomialParserUseCase::splitIntoTerms(const std::string& expression)
{
    std::string formatted = expression;
    std::vector<std::string> terms;

    if (formatted == "")
    {
        return {};
    }
    if (formatted[0] != '-' && formatted[0] != '+')
    {
        formatted = '+' + formatted;
    }

    std::string currentTerm;
    for (char c : formatted)
    {
        if ((c == '+' || c == '-') && !currentTerm.empty())
        {
            terms.push_back(currentTerm);
            currentTerm.clear();
        }
        currentTerm += c;
    }
    terms.push_back(currentTerm);

    return terms;
}

std::pair<int, double> PolynomialParserUseCase::parseTerm(const std::string& rawTerm)
{
    double coefficient;
    int power;

    size_t xPos = rawTerm.find('x');
    size_t caretPos = rawTerm.find('^');

    if (xPos == std::string::npos)
    {
        coefficient = stod(rawTerm);
        return std::pair(0, coefficient);
    }

    std::string coefficientStr = rawTerm.substr(0, xPos);
    if (coefficientStr == "+")
    {
        coefficient = 1.0;
    }
    else if (coefficientStr == "-")
    {
        coefficient = -1.0;
    }
    else
    {
        coefficient = stod(coefficientStr);
    }

    std::string powerStr;
    if (caretPos == std::string::npos)
    {
        power = 1;
    }
    else
    {
        powerStr = rawTerm.substr(caretPos + 1);
        power = stoi(powerStr);
    }

    return std::pair(power, coefficient);
}
