//
// Created by Cowabunga on 02.10.2026.
//

#ifndef POLYNOMIAL_CONSOLEVIEW_H
#define POLYNOMIAL_CONSOLEVIEW_H
#include "../viewmodel/PolynomialViewModel.h"

class ConsoleView
{
public:
    explicit ConsoleView(PolynomialViewModel& viewModel);
    void run() const;

private:
    void renderChar(char c) const;
    void renderPolynomial(Polynomial polynomial) const;
    void renderBuffer(std::vector<std::vector<char>> buffer) const;
    void readPolynomial() const;
    PolynomialViewModel& m_viewModel;
};


#endif //POLYNOMIAL_CONSOLEVIEW_H
