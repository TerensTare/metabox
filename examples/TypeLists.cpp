#include <type_traits>
#include "TypeLists.hpp"

int main()
{
    constexpr auto tl = tnt::make_list<int, char, float, bool>;

    static_assert(tnt::tl::size(tl) == 4);
    static_assert(tnt::tl::contains<char>(tl));
    static_assert(tnt::tl::front(tl) == tnt::make_tag<int>);
    static_assert(tnt::tl::back(tl) == tnt::make_tag<bool>);
    static_assert(tnt::tl::get<1>(tl) == tnt::make_tag<char>);
    static_assert(tnt::tl::find<float>(tl) == 2);

    static_assert(tnt::tl::all_of<std::is_fundamental>(tl));
    static_assert(tnt::tl::any_of<std::is_integral>(tl));
    static_assert(tnt::tl::none_of<std::is_reference>(tl));
}