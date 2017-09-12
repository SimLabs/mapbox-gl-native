#pragma once


#pragma warning(disable: 4068)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
#pragma warning(disable: 4305)
#pragma warning(disable: 4996)

#ifdef __cplusplus

#include <boost/optional.hpp>
#include <boost/utility/in_place_factory.hpp>
#include <memory>


namespace std
{
    template <class _Arg, class _Result>
    struct unary_function
    {
        typedef _Arg argument_type;
        typedef _Result result_type;
    };

    template <class _Arg1, class _Arg2, class _Result>
    struct binary_function
    {
        typedef _Arg1 first_argument_type;
        typedef _Arg2 second_argument_type;
        typedef _Result result_type;
    };

    template <typename T>
    using optional = boost::optional <T>;

    namespace experimental
    {
        template <typename T>
        using optional = boost::optional <T>;

        const auto nullopt = boost::none;
    }

}

using boost::in_place;

template<typename T>
struct my_optional_t
{
    explicit my_optional_t(T &&val)
        : val(std::move(val))
    {}

    explicit my_optional_t(T const &val)
        : val(val)
    {}

    
    template<typename U>
    operator boost::optional<U>()
    {
        return boost::optional<U>(std::move(val));
    }

private:
    T val;
};

template<typename T>
my_optional_t<std::remove_reference_t<T>> make_my_optional(T &&val)
{
    return my_optional_t<std::remove_reference_t<T>>(std::move(val));
}

template<typename T>
my_optional_t<std::remove_reference_t<T>> make_my_optional(T const &val)
{
    return my_optional_t<std::remove_reference_t<T>>(val);
}



#define FORWARD_CTOR(type, base) \
    template<typename... Args> \
    type(Args&&... args) \
        : base(std::forward<Args>(args)...) \
    {}; 
    

constexpr double M_PI = 3.1415926;
constexpr double M_PI_2 = M_PI / 2.0;

#define MBGL_USE_GLES2 0

#define NOT_MACRO !

#define TEMPORARILY_DISABLED 1

#endif
