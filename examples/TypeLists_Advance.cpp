#include <type_traits>
#include "TypeLists.hpp"

int main()
{
    constexpr auto tl = tnt::make_list<int, char, float, bool>;

    constexpr auto fundamental = tnt::value_fn<std::is_fundamental>;
    constexpr auto integral = tnt::value_fn<std::is_integral>;
    constexpr auto reference = tnt::value_fn<std::is_reference>;

    static_assert(tnt::tl::all_of(fundamental, tl));
    static_assert(tnt::tl::any_of(integral, tl));
    static_assert(tnt::tl::none_of(reference, tl));

    constexpr auto add_ref = tnt::type_fn<std::add_lvalue_reference>;

    static_assert(tnt::tl::transform(add_ref, tl) == tnt::make_list<int &, char &, float &, bool &>);
    static_assert(tnt::tl::reverse(tl) == tnt::make_list<int, char, float, bool>);
    static_assert(tnt::tl::generate<2>(tnt::make_tag<int>) == tnt::make_list<int, int>);

    static_assert(tnt::tl::filter(integral, tl) == tnt::make_tag<float>);
}