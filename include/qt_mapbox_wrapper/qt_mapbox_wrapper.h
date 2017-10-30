#pragma once

#include "qt_mapbox_wrapper_fwd.h"

class QWidget;

namespace qt_mapbox_wrapper
{

enum log_severity_t
{
    ls_debug,
    ls_info,
    ls_warning,
    ls_error,
};

typedef void(*log_pfn)(log_severity_t severity, char const *message);

struct wrapper
{
    struct params_t
    {
        char const *server_url = nullptr;
        char const *style_url = nullptr;

        char const *cache_path = nullptr;
        uint32_t cache_size_mb = 50;

        double lat = 0.;
        double lon = 0.;
        double zoom = 0.;

        wrapper_client *client = nullptr;
    };

    virtual ~wrapper() = default;
    virtual QWidget *get_widget() = 0;

    virtual void add_layer(char const *json, char const *before_name) = 0;
    
    virtual void add_source(char const *name, char const *json) = 0;
    virtual void update_source(char const *name, char const *json) = 0;

    virtual void add_image(char const *name, char const *path) = 0;

    virtual bool ready() const = 0;
};

struct wrapper_client
{
    virtual ~wrapper_client() = default;

    virtual void on_mouse_latlon_changed(double lat, double lon) = 0;
};

QT_MAPBOX_WRAPPER_API wrapper *create_wrapper(wrapper::params_t const &params);
QT_MAPBOX_WRAPPER_API void delete_wrapper(wrapper *);

QT_MAPBOX_WRAPPER_API void set_log_pfn(log_pfn pfn);

} // namespace qt_mapbox_wrapper