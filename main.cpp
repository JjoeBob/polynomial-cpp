#include <iostream>

#include "domain/models/Polynomial.h"
#include <cassert>
#include <memory>
#include "domain/impl/PolynomialParserUseCase.h"
#include "domain/impl/ValidatePolynomialInputUseCase.h"
#include "domain/impl/RasterizePolynomialUseCase.h"
#include "presentation/view/ConsoleView.h"

using namespace std;

void testSplitIntoTerms()
{
    PolynomialParserUseCase parser;

    auto terms = parser.splitIntoTerms("+23x^3+34x+4");

    assert(terms.size() == 3);
    assert(terms[0] == "+23x^3");
    assert(terms[1] == "+34x");
    assert(terms[2] == "+4");
}

int main()
{
    auto parser = PolynomialParserUseCase();
    auto validator = ValidatePolynomialInputUseCase();
    auto rasterizer = RasterizePolynomialUseCase();
    auto viewModel = PolynomialViewModel(parser, validator, rasterizer);
    ConsoleView view(viewModel);

    view.run();

    return 0;
}
