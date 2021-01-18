#include <iostream>

#include "Curious.hpp"

template <typename T>
struct entity : tnt::crtp<T>
{
    inline std::string type() const
    {
        this->base().type();
    }
};

struct static_object : entity<static_object>
{
    inline std::string type() const
    {
        return "static";
    }
};

struct dynamic_object : entity<dynamic_object>
{
    inline std::string type() const
    {
        return "dynamic";
    }
};

int main()
{
    static_object stat;
    dynamic_object dyn;

    std::cout << "static_object type: " << stat.type()
              << "\ndynamic_object type: " << dyn.type();
}