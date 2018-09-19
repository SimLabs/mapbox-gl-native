#pragma once

#if defined _WIN32 || defined __CYGWIN__
#   if defined(qt_mapbox_wrapper_EXPORTS)
#      define QT_MAPBOX_WRAPPER_API __declspec(dllexport)
#   else
#      define QT_MAPBOX_WRAPPER_API __declspec(dllimport)
#   endif
#elif __GNUC__ >= 4
#   if defined(qt_mapbox_wrapper_EXPORTS)
#      define QT_MAPBOX_WRAPPER_API __attribute__((used, visibility ("default")))
#   else
#      define QT_MAPBOX_WRAPPER_API
#   endif
#else
#   define QT_MAPBOX_WRAPPER_API
#endif


namespace qt_mapbox_wrapper
{

struct map;
struct widget;
struct widget_client;
struct texture_renderer;
struct gl_context_desc;

enum log_severity_t
{
    ls_debug,
    ls_info,
    ls_warning,
    ls_error,
};

typedef void(*log_pfn)(log_severity_t severity, char const *message);

} // namespace qt_mapbox_wrapper
