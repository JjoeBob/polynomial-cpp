cmake_minimum_required(VERSION 4.3)
project(polynomial)

set(CMAKE_CXX_STANDARD 20)

add_executable(polynomial main.cpp
        domain/models/Polynomial.cpp
        domain/models/Polynomial.h
        domain/impl/PolynomialParserUseCase.cpp
        domain/impl/PolynomialParserUseCase.h
        domain/api/IPolynomialParserUseCase.h
        presentation/View/ConsoleView.cpp
        presentation/View/ConsoleView.h
        presentation/viewModel/PolynomialViewModel.cpp
        presentation/viewModel/PolynomialViewModel.h
        presentation/state/UiState.cpp
        presentation/state/UiState.h
        domain/api/IValidatePolynomialInputUseCase.h
        domain/models/ValidationResult.cpp
        domain/models/ValidationResult.h
        domain/impl/ValidatePolynomialInputUseCase.cpp
        domain/impl/ValidatePolynomialInputUseCase.h
        domain/api/IRasterizePolynomialUseCase.h
        domain/impl/RasterizePolynomialUseCase.cpp
        domain/impl/RasterizePolynomialUseCase.h
        core/Observable.h)
