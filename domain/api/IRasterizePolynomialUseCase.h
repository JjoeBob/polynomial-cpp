//
// Created by Cowabunga on 03.10.2026.
//

#ifndef POLYNOMIAL_IRASTERIZEPOLYNOMIALUSECASE_H
#define POLYNOMIAL_IRASTERIZEPOLYNOMIALUSECASE_H

#include <vector>
#include "../models/Polynomial.h"

class IRasterizePolynomialUseCase
{
public:
    using ScreenBuffer = std::vector<std::vector<char>>;

    virtual ~IRasterizePolynomialUseCase() = default;

    virtual ScreenBuffer rasterize(const Polynomial& polynomial,
                                   int width = 80,
                                   int height = 25,
                                   double xMin = -10.0,
                                   double xMax = 10.0) = 0;
};


#endif //POLYNOMIAL_RASTERIZEPOLYNOMIALUSECASE_H
