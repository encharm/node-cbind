#ifndef ALIGNED_STRING
#define ALIGNED_STRING

#include <cstdint>

// Sequence of char
template <char...Cs> struct char_sequence
{
    template <char C> using push_back = char_sequence<Cs..., C>;
};

// Remove all chars from char_sequence from '\0'
template <typename, char...> struct strip_sequence;

template <char...Cs>
struct strip_sequence<char_sequence<>, Cs...>
{
    using type = char_sequence<Cs...>;
};

template <char...Cs, char...Cs2>
struct strip_sequence<char_sequence<'\0', Cs...>, Cs2...>
{
    using type = char_sequence<Cs2...>;
};

template <char...Cs, char C, char...Cs2>
struct strip_sequence<char_sequence<C, Cs...>, Cs2...>
{
    using type = typename strip_sequence<char_sequence<Cs...>, Cs2..., C>::type;
};

// struct to create a aligned char array
template <std::size_t Alignment, typename chars> struct aligned_string;

template <std::size_t Alignment, char...Cs>
struct aligned_string<Alignment, char_sequence<Cs...>>
{
    alignas(Alignment) static constexpr char str[sizeof...(Cs)] = {Cs...};
};

template <std::size_t Alignment, char...Cs>
alignas(Alignment) constexpr
char aligned_string<Alignment, char_sequence<Cs...>>::str[sizeof...(Cs)];

// helper to get the i_th character (`\0` for out of bound)
template <std::size_t I, std::size_t N>
constexpr char at(const char (&a)[N]) { return I < N ? a[I] : '\0'; }

// helper to check if the c-string will not be truncated
template <std::size_t max_size, std::size_t N>
constexpr bool check_size(const char (&)[N])
{
    static_assert(N <= max_size, "string too long");
    return N <= max_size;
}

// Helper macros to build char_sequence from c-string
#define PUSH_BACK_8(S, I) \
    ::push_back<at<(I) + 0>(S)>::push_back<at<(I) + 1>(S)> \
    ::push_back<at<(I) + 2>(S)>::push_back<at<(I) + 3>(S)> \
    ::push_back<at<(I) + 4>(S)>::push_back<at<(I) + 5>(S)> \
    ::push_back<at<(I) + 6>(S)>::push_back<at<(I) + 7>(S)>

#define PUSH_BACK_32(S, I) \
        PUSH_BACK_8(S, (I) + 0) PUSH_BACK_8(S, (I) + 8) \
        PUSH_BACK_8(S, (I) + 16) PUSH_BACK_8(S, (I) + 24)

#define PUSH_BACK_128(S, I) \
    PUSH_BACK_32(S, (I) + 0) PUSH_BACK_32(S, (I) + 32) \
    PUSH_BACK_32(S, (I) + 64) PUSH_BACK_32(S, (I) + 96)

// Macro to create char_sequence from c-string (limited to 128 chars)
#define MAKE_CHAR_SEQUENCE(S) \
    strip_sequence<char_sequence<> \
    PUSH_BACK_128(S, 0) \
    >::type::template push_back<check_size<128>(S) ? '\0' : '\0'>

// Macro to return an aligned c-string
#define MAKE_ALIGNED_STRING(ALIGNMENT, S) \
    aligned_string<ALIGNMENT, MAKE_CHAR_SEQUENCE(S)>::str

#define TYPE_LITERAL(S) MAKE_ALIGNED_STRING(sizeof(void*), S)


#define CBIND_CONCAT(a, b) a ## b
#define CBIND_QUOTE(str) #str
#define CBIND_EXPAND_AND_QUOTE(str) CBIND_QUOTE(str)

#endif // ALIGNED_STRING