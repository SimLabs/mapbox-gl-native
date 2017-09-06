#pragma once



namespace std
{
    template <typename T>
    class optional;

    
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

    namespace experimental
    {
        template <typename T>
        using optional = std::optional <T>;
    }


}

#define M_PI 3.1415926

#define MBGL_USE_GLES2 0

#define NOT_MACRO !