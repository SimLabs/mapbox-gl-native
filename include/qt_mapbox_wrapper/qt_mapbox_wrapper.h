#pragma once

#include "qt_mapbox_wrapper_fwd.h"

class QWidget;

namespace qt_mapbox_wrapper
{



struct map
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
    };

    virtual ~map() = default;

    virtual void add_layer(char const *json, char const *before_name) = 0;
    
    virtual void add_source(char const *name, char const *json) = 0;
    virtual void update_source(char const *name, char const *json) = 0;

    virtual void add_image(char const *name, char const *path) = 0;

    virtual bool ready() const = 0;
};

QT_MAPBOX_WRAPPER_API map *create_map(map::params_t const &params);
QT_MAPBOX_WRAPPER_API void release_map(map *);


struct widget
{
    struct params_t
    {
        map *m = nullptr;
        widget_client *client = nullptr;
    };
    
    virtual ~widget() = default;
    virtual QWidget *get_qwidget() = 0;
};

QT_MAPBOX_WRAPPER_API widget *create_widget(widget::params_t const &params);
QT_MAPBOX_WRAPPER_API void release_widget(widget *);

struct widget_client
{
    virtual ~widget_client() = default;

    virtual void on_mouse_latlon_changed(double lat, double lon) = 0;
};


struct texture_renderer
{
    struct params_t
    {
        map *m = nullptr;
    };

    struct init_params_t
    {
        uint32_t width = 256;
        uint32_t height = 256;
        void *hglrc = nullptr;
        void *hwnd = nullptr;
    };
    
    virtual ~texture_renderer() = default;

    virtual void init(init_params_t const &params) = 0;
    virtual void render(uint32_t fbo) = 0;

    virtual void set_pos(double lat, double lon, double zoom, double bearing) = 0;
};

QT_MAPBOX_WRAPPER_API texture_renderer *create_texture_renderer(texture_renderer::params_t const &params);
QT_MAPBOX_WRAPPER_API void release_texture_renderer(texture_renderer *);


QT_MAPBOX_WRAPPER_API void set_log_pfn(log_pfn pfn);
QT_MAPBOX_WRAPPER_API void set_verbose_logging(bool value);

} // namespace qt_mapbox_wrapper