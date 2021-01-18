#include <iostream>
#include <variant>

#include "Functional.hpp"

inline void overload_example()
{
    std::variant<int, char, float> var = 12;
    tnt::overload visitor{
        [](int) { std::cout << "int"; },
        [](float) { std::cout << "float"; },
        [](char) { std::cout << "char"; }};

    std::visit(visitor, var);

    var = 1.f;
    std::visit(visitor, var);

    var = 'a';
    std::visit(visitor, var);
}

inline void ycomb_example()
{
    // return type must be specified
    tnt::y_comb gcd{[](auto &&gcd, unsigned a, unsigned b) -> unsigned {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }};

    std::cout << "gcd(10, 15) = " << gcd(10, 15) << '\n';

    tnt::y_comb factorial{[](auto &&fact, unsigned u) -> unsigned {
        if (u < 2)
            return 1;
        return u * fact(u - 1);
    }};

    std::cout << "5! = " << factorial(5);
}

int main()
{
    overload_example();
    ycomb_example();
}