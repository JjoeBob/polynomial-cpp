//
// Created by Cowabunga on 03.10.2026.
//

#include "RasterizePolynomialUseCase.h"
#include <cmath>

std::vector<std::vector<char>> RasterizePolynomialUseCase::rasterize(
    const Polynomial& polynomial,
    int width,
    int height,
    double xMin,
    double xMax)
{
    std::vector buffer(51, std::vector<char>(51, ' '));
    int result;

    if (width <= 0 || height <= 0 || xMin >= xMax)
    {
        return {};
    }

    for (int y = 0; y <= 50; y++)
    {
        for (int x = 0; x <= 50; x++)
        {
            if (x == 25 && y == 25)
            {
                buffer[x][y] = '+';
            }
            else if (x == 25)
            {
                buffer[x][y] = '-';
            }
            else if (y == 25)
            {
                buffer[x][y] = '|';
            }
            else
            {
                buffer[x][y] = ' ';
            }
        }
    }

    for (int x = -25; x <= 25; x++)
    {
        result = polynomial.evaluate(x);
        if (result + 25 > 0 && result + 25 < 50)
            buffer[25 - result][x + 25] = '*';
    }

    return buffer;
}
