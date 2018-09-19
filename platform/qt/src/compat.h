#pragma once


#pragma warning(disable: 4068)
#pragma warning(disable: 4244)
#pragma warning(disable: 4267)
#pragma warning(disable: 4305)
#pragma warning(disable: 4996)

#ifdef __cplusplus

#include <experimental/optional>
#include <memory>

template<typename T>
struct my_optional_t
{
    explicit my_optional_t(T &&val)
        : val_(std::move(val))
    {}

    explicit my_optional_t(T const &val)
        : val_(val)
    {}


    template<typename U>
    operator std::experimental::optional<U>()
    {
        return std::experimental::optional<U>(std::move(val_));
    }

private:
    T val_;
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

#define MBGL_USE_GLES2 0

#define NOT_MACRO !

#define TEMPORARILY_DISABLED 1

#define UNICODE_CHAR(c) L##c

#endif


