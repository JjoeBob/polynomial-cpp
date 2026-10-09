//
// Created by Cowabunga on 03.10.2026.
//

#ifndef POLYNOMIAL_RASTERIZEPOLYNOMIALUSECASE_H
#define POLYNOMIAL_RASTERIZEPOLYNOMIALUSECASE_H
#include "../api/IRasterizePolynomialUseCase.h"
#include "../api/IRasterizePolynomialUseCase.h"
#include "../models/Polynomial.h"


class RasterizePolynomialUseCase : public IRasterizePolynomialUseCase
{
public:
    using ScreenBuffer = std::vector<std::vector<char>>;
    ScreenBuffer rasterize(
        const Polynomial& polynomial,
        int width,
        int height,
        double xMin,
        double xMax) override;
};


#endif //POLYNOMIAL_RASTERIZEPOLYNOMIALUSECASE_H
