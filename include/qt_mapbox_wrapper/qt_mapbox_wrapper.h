#pragma once

#include "qt_mapbox_wrapper_fwd.h"

class QWidget;

namespace qt_mapbox_wrapper
{

struct wrapper
{
    struct params_t
    {

    };

    virtual ~wrapper() = default;
    virtual QWidget *get_widget() = 0;

    virtual void add_layer(char const *json, char const *before_name) = 0;
    
    virtual void add_source(char const *name, char const *json) = 0;
    virtual void update_source(char const *name, char const *json) = 0;

    virtual void add_image(char const *name, char const *path) = 0;

    virtual bool ready() const = 0;
};

QT_MAPBOX_WRAPPER_API wrapper *create_wrapper(wrapper::params_t const &params);
QT_MAPBOX_WRAPPER_API void delete_wrapper(wrapper *);

} // namespace qt_mapbox_wrapper