#include "ConsoleView.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cstdio>

char getch()
{
    char buf = 0;
    termios oldSettings{};
    // 1. Сохраняем исходные настройки терминала 💾
    tcgetattr(STDIN_FILENO, &oldSettings);

    termios newSettings = oldSettings;
    // 2. Отключаем буферизацию строк (~ICANON) и эхо-печать (~ECHO) ⚙️
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    // 3. Считываем ровно один символ сразу после нажатия 📥
    buf = static_cast<char>(getchar());

    // 4. Обязательно возвращаем терминал в исходное состояние 🔄
    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    return buf;
}

ConsoleView::ConsoleView(PolynomialViewModel& viewModel) : m_viewModel(viewModel)
{
}

void ConsoleView::run() const
{
    m_viewModel.observeLastValidChar().observe([this](char c)
    {
        // в идеале нужно создать sealed interface UiState и передавать его реализацию с полем input, а не input напрямую
        this->renderChar(c);
    });

    m_viewModel.observePolynomial().observe([this](std::optional<Polynomial> polynomial)
    {
        if (polynomial.has_value())
        {
            renderPolynomial(*polynomial);
        }
    });

    m_viewModel.observeBuffer().observe([this](std::optional<std::vector<std::vector<char>>> buffer)
    {
        if (buffer.has_value())
        {
            renderBuffer(*buffer);
        }
    });

    readPolynomial();
}

void ConsoleView::renderChar(char c) const
{
    // и здесь соответственно должен быть when, который будет обрабатывать все состояния UiState
    std::cout << c;
}

void ConsoleView::renderPolynomial(Polynomial polynomial) const
{
    std::cout << std::endl << polynomial << std::endl;
}

void ConsoleView::renderBuffer(std::vector<std::vector<char>> buffer) const
{
    for (auto y : buffer)
    {
        for (auto x : y)
        {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }
}

void ConsoleView::readPolynomial() const
{
    while (true)
    {
        char c = getch();

        if (c == '\n' || c == '\r')
        {
            m_viewModel.onEnterPressed();
            break;
        }

        m_viewModel.onCharTyped(c);
    }
}
