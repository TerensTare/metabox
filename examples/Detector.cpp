#include <iostream>

#include "Detector.hpp"

inline void windows_fn()
{
    std::cout << "Windows";
}

inline void linux_fn()
{
    std::cout << "Linux";
}

inline void mac_fn()
{
    std::cout << "MacOS";
}

inline void unix_fn()
{
    std::cout << "Unix";
}

inline void other_fn()
{
    std::cout << "Unknown OS";
}

int main()
{
    if constexpr (tnt::is_windows_v)
        windows_fn();
    else if constexpr (tnt::is_linux_v)
        linux_fn();
    else if constexpr (tnt::is_macos_v)
        mac_fn();
    else if constexpr (tnt::is_unix_v)
        unix_fn();
    else
        other_fn();
}