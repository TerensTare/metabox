#ifndef TNT_META_FUNCTIONAL_HPP
#define TNT_META_FUNCTIONAL_HPP

namespace tnt
{
    namespace detail
    {
        // thx cppreference
        // https://en.cppreference.com/w/cpp/types/decay
        template <typename T>
        struct remove_ref
        {
            using type = T;
        };

        template <typename T>
        struct remove_ref<T &>
        {
            using type = T;
        };

        template <typename T>
        struct remove_ref<T &&>
        {
            using type = T;
        };

        template <typename T>
        using remove_ref_t = typename remove_ref<T>::type;

        template <typename T>
        struct is_lvalue_ref
        {
            inline static constexpr bool value{false};
        };

        template <typename T>
        struct is_lvalue_ref<T &>
        {
            inline static constexpr bool value{true};
        };
    } // namespace detail

    // thx foonathan
    // https://foonathan.net/2020/09/move-forward/#content
    template <typename T>
    constexpr detail::remove_ref_t<T> &&move(T &&t) noexcept
    {
        return static_cast<detail::remove_ref_t<T> &&>(t);
    }

    template <typename T>
    constexpr T &&forward(detail::remove_ref_t<T> &t) noexcept
    {
        return static_cast<T &&>(t);
    }

    template <typename T>
    constexpr T &&forward(detail::remove_ref_t<T> &&t) noexcept
    {
        static_assert(!detail::is_lvalue_ref<T>::value);
        return static_cast<T &&>(t);
    }

    template <typename... Ts>
    struct overload final : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overload(Ts...) -> overload<Ts...>;

    // thx riptutorial
    // https://riptutorial.com/cplusplus/example/8508/recursive-lambdas
    template <typename Fn>
    class y_comb
    {
    public:
        explicit constexpr y_comb(Fn &&fn_) noexcept(std::is_nothrow_move_constructible_v<Fn>)
            : fn{std::move(fn_)} {}

        template <typename... Args>
        // clang-format off
            requires std::invocable<Fn, y_comb<Fn>&, Args...>
        constexpr decltype(auto) operator()(Args &&... args) const
            noexcept(std::is_nothrow_invocable_v<Fn, y_comb<Fn>&, Args...>)
        {
            // clang-format on
            return fn(*this, std::forward<Args>(args)...);
        }

    private:
        [[no_unique_address]] Fn fn;
    };

    template <typename Fn>
    y_comb(Fn) -> y_comb<Fn>;
} // namespace tnt

#endif //! TNT_META_FUNCTIONAL_HPP