#ifndef TNT_CRTP_HELPER_HPP
#define TNT_CRTP_HELPER_HPP

namespace tnt
{
    // thx Jonathan Boccara
    // https://www.fluentcpp.com/2017/05/19/crtp-helper/
    template <typename T>
    struct crtp
    {
        constexpr crtp() noexcept
            : super{static_cast<T &>(*this)} {}

        constexpr T &base() noexcept { return static_cast<T &>(*this); }
        constexpr T const &base() const noexcept { return static_cast<T const &>(*this); }

    protected:
        T &super;
    };
} // namespace tnt

#endif //!TNT_CRTP_HELPER_HPP