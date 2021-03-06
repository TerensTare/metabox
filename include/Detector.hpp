#ifndef TNT_DETECTOR_HPP
#define TNT_DETECTOR_HPP

// TODO:
// support more os and compiler checks.

#if (defined(_M_AMD64) || defined(_M_X64) || defined(__amd64)) && !defined(__x86_64__)
#define __x86_64__ 1
#define TNT_DEFINE_X86
#endif

namespace tnt
{
    namespace detail
    {
        template <typename... Ts>
        using void_t = void;

        template <typename T, typename U>
        inline constexpr bool is_same_v{false};

        template <typename T>
        inline constexpr bool is_same_v<T, T>{true};

        template <typename T, typename... Ts>
        inline constexpr bool is_any_of_v{(... or is_same_v<T, Ts>)};

        struct tag_type_t
        {
            tag_type_t() = delete;

            tag_type_t(tag_type_t const &) = delete;
            tag_type_t &operator=(tag_type_t const &) = delete;

            tag_type_t(tag_type_t &&) = delete;
            tag_type_t &operator=(tag_type_t &&) = delete;
        };
    } // namespace detail

    // thx Jonathan Boccara
    // https://www.fluentcpp.com/2020/05/29/how-to-make-derived-classes-implement-more-than-assignment/
    template <template <typename...> typename Expression,
              typename Attempt, typename... Ts>
    struct is_detected
    {
        inline static constexpr bool value{false};
    };

    template <template <typename...> class Expression, typename... Ts>
    struct is_detected<Expression, detail::void_t<Expression<Ts...>>, Ts...>
    {
        inline static constexpr bool value{true};
    };

    template <template <typename...> class Expression, typename... Ts>
    inline constexpr bool is_detected_v{is_detected<Expression, void, Ts...>::value};

    // os

    struct windows_t final : detail::tag_type_t
    {
    };

    struct linux_t final : detail::tag_type_t
    {
    };

    struct macos_t final : detail::tag_type_t
    {
    };

    struct ios_t final : detail::tag_type_t
    {
    };

    struct android_t final : detail::tag_type_t
    {
    };

    struct unix_t final : detail::tag_type_t
    {
    };

    template <typename T>
    concept os = detail::is_any_of_v<T, windows_t, linux_t, macos_t, ios_t, android_t, unix_t>;

    /// @brief Check if the os where the app is running matches O.
    /// @tparam O The os tag.
    template <os O>
    struct is_os;

    template <>
    struct is_os<windows_t>
    {
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_os<linux_t>
    {
#if defined(__linux__) && !defined(__android__)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_os<macos_t>
    {
#ifdef TARGET_OS_MAC
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_os<ios_t>
    {
#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_os<android_t>
    {
#ifdef __android__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_os<unix_t>
    {
#if !defined(__linux__) && defined(__unix__)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <os O>
    inline constexpr bool is_os_v{is_os<O>::value};

    inline constexpr bool is_windows_v{is_os<windows_t>::value};
    inline constexpr bool is_linux_v{is_os<linux_t>::value};
    inline constexpr bool is_macos_v{is_os<macos_t>::value};
    inline constexpr bool is_ios_v{is_os<ios_t>::value};
    inline constexpr bool is_android_v{is_os<android_t>::value};
    inline constexpr bool is_unix_v{is_os<unix_t>::value};

    // compiler

    struct gcc_t final : detail::tag_type_t
    {
    };

    struct clang_t final : detail::tag_type_t
    {
    };

    struct msvc_t final : detail::tag_type_t
    {
    };

    template <typename T>
    concept compiler = detail::is_any_of_v<T, gcc_t, clang_t, msvc_t>;

    /// @brief Check if the compiler being used matches C.
    /// @tparam C The compiler tag.
    template <compiler C>
    struct is_compiler;

    template <>
    struct is_compiler<gcc_t>
    {
#ifdef __GNUC__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_compiler<clang_t>
    {
#ifdef __clang__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_compiler<msvc_t>
    {
#ifdef _MSC_VER
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <compiler C>
    inline constexpr bool is_compiler_v{is_compiler<C>::value};

    inline constexpr bool is_gcc_v{is_compiler<gcc_t>::value};
    inline constexpr bool is_clang_v{is_compiler<clang_t>::value};
    inline constexpr bool is_msvc_v{is_compiler<msvc_t>::value};

    // debug/release build

    struct debug_t final : detail::tag_type_t
    {
    };

    struct release_t final : detail::tag_type_t
    {
    };

    template <typename T>
    concept build_type = detail::is_any_of_v<T, debug_t, release_t>;

    template <build_type T>
    struct is_build;

    template <>
    struct is_build<debug_t>
    {
#if defined(_DEBUG) || defined(DEBUG)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_build<release_t>
    {
#if defined(NDEBUG) || !(defined(DEBUG) && defined(_DEBUG))
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <build_type T>
    inline static constexpr bool is_build_v{is_build<T>::value};

    inline static constexpr bool is_debug_v{is_build<debug_t>::value};
    inline static constexpr bool is_release_v{is_build<release_t>::value};

    // simd instructions

    struct x86sse_t final : detail::tag_type_t
    {
    };

    struct x86sse2_t final : detail::tag_type_t
    {
    };

    struct sse_t final : detail::tag_type_t
    {
    };

    struct sse2_t final : detail::tag_type_t
    {
    };

    struct sse3_t final : detail::tag_type_t
    {
    };

    struct ssse3_t final : detail::tag_type_t
    {
    };

    struct sse4_1_t final : detail::tag_type_t
    {
    };

    struct sse4_2_t final : detail::tag_type_t
    {
    };

    struct avx_t final : detail::tag_type_t
    {
    };

    struct avx2_t final : detail::tag_type_t
    {
    };

    template <typename T>
    concept simd_instruction =
        detail::is_any_of_v<T, x86sse_t, x86sse2_t,
                            sse_t, sse2_t, sse3_t,
                            ssse3_t, sse4_1_t, sse4_2_t,
                            avx_t, avx2_t>;

    /// @brief Check if the machine where the app is running supports the given SIMD instructions.
    /// @tparam S The desired SIMD instructions tag.
    template <simd_instruction S>
    struct is_simd;

    template <>
    struct is_simd<x86sse_t>
    {
#if defined(_M_IX86_FP) && (_M_IX86_FP == 1)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<x86sse2_t>
    {
#if defined(_M_IX86_FP) && (_M_IX86_FP == 2)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<sse_t>
    {
#ifdef __SSE__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<sse2_t>
    {
#if defined(__SSE2__) || defined(__x86_x64__)
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<sse3_t>
    {
#ifdef __SSE3__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<ssse3_t>
    {
#ifdef __SSSE3__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<sse4_1_t>
    {
#ifdef __SSE4_1__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<sse4_2_t>
    {
#ifdef __SSE4_2__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<avx_t>
    {
#ifdef __AVX__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <>
    struct is_simd<avx2_t>
    {
#ifdef __AVX2__
        inline static constexpr bool value{true};
#else
        inline static constexpr bool value{false};
#endif
    };

    template <simd_instruction S>
    inline constexpr bool is_simd_v{is_simd<S>::value};

    inline constexpr bool is_x86_sse_t{is_simd<x86sse_t>::value};
    inline constexpr bool is_x86_sse2_t{is_simd<x86sse2_t>::value};
    inline constexpr bool is_sse_t{is_simd<sse_t>::value};
    inline constexpr bool is_sse2_t{is_simd<sse2_t>::value};
    inline constexpr bool is_sse3_t{is_simd<sse3_t>::value};
    inline constexpr bool is_ssse3_t{is_simd<ssse3_t>::value};
    inline constexpr bool is_sse4_1_t{is_simd<sse4_1_t>::value};
    inline constexpr bool is_sse4_2_t{is_simd<sse4_2_t>::value};
    inline constexpr bool is_avx_t{is_simd<avx_t>::value};
    inline constexpr bool is_avx2_t{is_simd<avx2_t>::value};
} // namespace tnt

#ifdef TNT_DEFINE_X86
#undef __x86_x64__
#endif

#endif //!TNT_DETECTOR_HPP