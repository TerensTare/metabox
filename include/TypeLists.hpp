#ifndef TNT_TYPE_LISTS_HPP
#define TNT_TYPE_LISTS_HPP

#include <utility>

// TODO:
// merge type_fn and value_fn using concepts (same for bind_*)

// thx Oleg Fatkhiev
// https://2019.cppconf-moscow.ru/en/talks/2zq0btoxldq6tmo3g7cvuo/
namespace tnt
{
    template <typename...>
    struct type_list
    {
    };

    // shorthand so that the following is possible
    // constexpr auto li = tnt::make_list<int, char>;
    template <typename... Ts>
    inline constexpr type_list<Ts...> make_list{};

    template <typename T>
    using type_tag = type_list<T>;

    // shorthand so that the following is possible
    // constexpr auto tag = tnt::make_tag<int>;
    template <typename T>
    inline constexpr type_tag<T> make_tag{};

    using empty_list = type_list<>;

    template <template <typename...> typename P>
    struct value_wrapper_fn
    {
        template <typename... Ts>
        constexpr auto operator()(type_tag<Ts>...) noexcept
        {
            return P<Ts...>::value;
        }

        template <typename... Ts>
        constexpr auto operator()(type_list<Ts...>) noexcept
        {
            return P<Ts...>::value;
        }
    };

    template <template <typename...> typename P>
    inline constexpr value_wrapper_fn<P> value_fn{};

    template <template <typename...> typename P, typename... Us>
    struct bind_value_wrapper_fn
    {
        template <typename... Ts>
        constexpr auto operator()(type_tag<Ts>...) noexcept
        {
            return P<Us..., Ts...>::value;
        }

        template <typename... Ts>
        constexpr auto operator()(type_list<Ts...>) noexcept
        {
            return P<Us..., Ts...>::value;
        }
    };

    template <template <typename...> typename P, typename... Ts>
    inline constexpr bind_value_wrapper_fn<P, Ts...> bind_value_fn{};

    template <template <typename...> typename P>
    struct type_wrapper_fn
    {
        template <typename... Ts>
        constexpr auto operator()(type_tag<Ts>...) noexcept
        {
            return make_tag<typename P<Ts...>::type>;
        }

        template <typename... Ts>
        constexpr auto operator()(type_list<Ts...>) noexcept
        {
            return make_tag<typename P<Ts...>::type>;
        }
    };

    template <template <typename...> typename P>
    inline constexpr type_wrapper_fn<P> type_fn{};

    template <template <typename...> typename P, typename... Us>
    struct bind_type_wrapper_fn
    {
        template <typename... Ts>
        constexpr auto operator()(type_tag<Ts>...) noexcept
        {
            return make_tag<typename P<Us..., Ts...>::type>;
        }

        template <typename... Ts>
        constexpr auto operator()(type_list<Ts...>) noexcept
        {
            return make_tag<typename P<Us..., Ts...>::type>;
        }
    };

    template <template <typename...> typename P, typename... Ts>
    inline constexpr bind_type_wrapper_fn<P, Ts...> bind_type_fn{};

    // operator==/operator|
    template <typename... Ts, typename... Us>
    constexpr bool operator==(tnt::type_list<Ts...>, tnt::type_list<Us...>) noexcept
    {
        if constexpr (sizeof...(Ts) != sizeof...(Us))
            return false;
        else
            return (... && std::is_same_v<Ts, Us>);
    }

    template <typename... Ts, typename... Us>
    constexpr type_list<Ts..., Us...> operator|(tnt::type_list<Ts...>, tnt::type_list<Us...>) noexcept { return {}; }

    namespace tl
    {
        // size/empty/contains
        template <typename... Ts>
        constexpr std::size_t size(type_list<Ts...>) noexcept { return sizeof...(Ts); }

        template <typename... Ts>
        [[nodiscard]] constexpr bool empty(type_list<Ts...>) noexcept { return (sizeof...(Ts) == 0); }

        template <typename T, typename... Ts>
        constexpr bool contains(type_list<Ts...>) noexcept { return (... || std::is_same_v<T, Ts>); }

        template <typename... Ts, typename... Us>
        constexpr bool contains(type_list<Ts...>, type_list<Us...>) noexcept
        {
            return (... && tl::v2::contains<Us>(make_list<Ts...>));
        }

        // front/back
        template <typename T, typename... Ts>
        constexpr type_tag<T> front(type_list<T, Ts...>) noexcept { return {}; }

        template <typename T, typename... Ts>
        constexpr auto back(type_list<T, Ts...>) noexcept { return tl::back(make_list<Ts...>); }

        template <typename T>
        constexpr type_tag<T> back(type_tag<T>) noexcept { return {}; }

        // push_front/push_back
        template <typename T, typename... Ts>
        constexpr type_list<T, Ts...> push_front(type_list<Ts...>) noexcept { return {}; }

        template <typename... Ts, typename... Us>
        constexpr type_list<Us..., Ts...> push_front(type_list<Ts...>, type_list<Us...>) noexcept { return {}; }

        template <typename T, typename... Ts>
        constexpr type_list<Ts..., T> push_back(type_list<Ts...>) noexcept { return {}; }

        template <typename... Ts, typename... Us>
        constexpr type_list<Ts..., Us...> push_back(type_list<Ts...>, type_list<Us...>) noexcept { return {}; }

        // pop_front/pop_back
        template <typename T, typename... Ts>
        constexpr type_list<Ts...> pop_front(type_list<T, Ts...>) noexcept { return {}; }

        template <typename... Ts, typename... Us>
        constexpr type_list<Us...> pop_front(type_list<Ts..., Us...>, type_list<Ts...>) noexcept { return {}; }

        template <typename... Ts, typename T>
        constexpr type_list<Ts...> pop_back(type_list<Ts..., T>) noexcept { return {}; }

        template <typename... Ts, typename... Us>
        constexpr type_list<Ts...> pop_back(type_list<Ts..., Us...>, type_list<Us...>) noexcept { return {}; }

        // get/find/find_if
        template <std::size_t I, typename T, typename... Ts>
        constexpr auto get(type_list<T, Ts...>) noexcept
        {
            static_assert(I < sizeof...(Ts) + 1);
            if constexpr (I > 0)
                return tl::get<I - 1>(make_list<Ts...>);
            else
                return make_tag<T>;
        }

        namespace detail
        {
            template <typename T, std::size_t S>
            constexpr T min_elem(T (&arr)[S]) noexcept
            {
                auto m{arr[0]};
                return [&arr, m ]<std::size_t... Si>(std::index_sequence<Si...>) mutable
                {
                    return ((m = std::min(m, arr[Si])), ...);
                }
                (std::make_index_sequence<S>{});
            }
        } // namespace detail

        template <typename T, typename... Ts>
        constexpr std::size_t find(type_list<Ts...>) noexcept
        {
            return []<std::size_t... S>(std::index_sequence<S...>) noexcept->std::size_t
            {
                constexpr std::size_t arr[]{[]<unsigned I>(std::integral_constant<unsigned, I>) noexcept -> unsigned {
                    if constexpr (tl::get<I>(make_list<Ts...>) == make_tag<T>)
                        return I;
                    else
                        return sizeof...(Ts);
                }(std::integral_constant<unsigned, S>{})...};
                return detail::min_elem(arr);
            }
            (std::make_index_sequence<sizeof...(Ts)>{});
        }

        template <template <typename...> typename P, typename... Ts>
        constexpr std::size_t find_if(type_list<Ts...>) noexcept
        {
            return []<std::size_t... S>(std::index_sequence<S...>) noexcept->std::size_t
            {
                return std::min({[]<std::size_t I, typename T>(std::integral_constant<std::size_t, I>, type_tag<T>) noexcept -> std::size_t {
                    if constexpr (P<T>::value)
                        return I;
                    else
                        return sizeof...(Ts);
                }(std::integral_constant<std::size_t, S>{}, make_tag<Ts>)...});
            }
            (std::make_index_sequence<sizeof...(Ts)>{});
        }

        template <typename A, typename... Ts>
        constexpr std::size_t find_if(A, type_list<Ts...>) noexcept
        {
            return []<std::size_t... S>(std::index_sequence<S...>) noexcept->std::size_t
            {
                return std::min({[]<std::size_t I, typename T>(std::integral_constant<std::size_t, I>, type_tag<T>) noexcept -> std::size_t {
                    if constexpr (A{}(make_tag<Ts>))
                        return I;
                    else
                        return sizeof...(Ts);
                }(std::integral_constant<std::size_t, S>{}, make_tag<Ts>)...});
            }
            (std::make_index_sequence<sizeof...(Ts)>{});
        }

        // all_of/any_of/none_of
        template <template <typename...> typename P, typename... Ts>
        constexpr bool all_of(type_list<Ts...>) noexcept
        {
            return (... && P<Ts>::value);
        }

        template <typename P, typename... Ts>
        constexpr bool all_of(P, type_list<Ts...>) noexcept
        {
            return (... && P{}(make_tag<Ts>));
        }

        template <template <typename...> typename P, typename... Ts>
        constexpr bool any_of(type_list<Ts...>) noexcept
        {
            return (... || P<Ts>::value);
        }

        template <typename P, typename... Ts>
        constexpr bool any_of(P, type_list<Ts...>) noexcept
        {
            return (... || P{}(make_tag<Ts>));
        }

        template <template <typename...> typename P, typename... Ts>
        constexpr bool none_of(type_list<Ts...>) noexcept
        {
            return not(... && P<Ts>::value);
        }

        template <typename P, typename... Ts>
        constexpr bool none_of(P, type_list<Ts...>) noexcept
        {
            return not(... && P{}(make_tag<Ts>));
        }

        // transform/reverse/generate
        template <template <typename...> typename P, typename... Ts>
        constexpr type_list<typename P<Ts>::type...> transform(type_list<Ts...>) noexcept { return {}; }

        template <typename P, typename... Ts>
        constexpr auto transform(P, type_list<Ts...>) noexcept
        {
            return (empty_list{} | ... | P{}(make_tag<Ts>));
        }

        template <typename... Ts>
        constexpr auto reverse(type_list<Ts...>) noexcept
        {
            return []<std::size_t... S>(std::index_sequence<S...>)
            {
                return make_list<tl::get<sizeof...(Ts) - S - 1>(make_list<Ts...>)...>;
            }
            (std::make_index_sequence<sizeof...(Ts)>{});
        }

        template <std::size_t I, typename T>
        constexpr auto generate() noexcept
        {
            return []<std::size_t... S>(std::index_sequence<S...>)
            {
                return ((S, make_tag<T>) | ...);
            }
            (std::make_index_sequence<I>{});
        }

        template <std::size_t I, typename T>
        constexpr auto generate(type_tag<T>) noexcept
        {
            return []<std::size_t... S>(std::index_sequence<S...>)
            {
                return ((S, make_tag<T>) | ...);
            }
            (std::make_index_sequence<I>{});
        }

        // filter
        template <template <typename...> typename P, typename... Ts>
        constexpr auto filter(type_list<Ts...>) noexcept
        {
            constexpr auto filter_one = []<template <typename...> typename F,
                                           typename T>(F, type_tag<T>) noexcept
            {
                if constexpr (F<T>::value)
                    return empty_list{};
                else
                    return make_tag<T>;
            };

            return (empty_list{} | ... | filter_one(P{}, make_list<Ts>));
        }

        template <typename P, typename... Ts>
        constexpr auto filter(P, type_list<Ts...>) noexcept
        {
            constexpr auto filter_one = []<typename F, typename T>(
                                            F, type_tag<T>) noexcept {
                if constexpr (F{}(make_tag<T>))
                    return empty_list{};
                else
                    return make_tag<T>;
            };

            return (empty_list{} | ... | filter_one(P{}, make_tag<Ts>));
        }
    } // namespace tl
} // namespace tnt

#endif //!TNT_TYPE_LISTS_HPP