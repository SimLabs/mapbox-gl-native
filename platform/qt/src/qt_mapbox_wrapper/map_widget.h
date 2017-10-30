#pragma once

#include "qt_mapbox_wrapper/qt_mapbox_wrapper.h"

class QWidget;
class QMapboxGL;

namespace qt_mapbox_wrapper
{

struct map_widget
{
    typedef wrapper::params_t params_t;
    
    virtual ~map_widget() = default;
    virtual QWidget *as_qwidget() = 0;
    virtual QMapboxGL *get_map() = 0;
};

typedef std::unique_ptr<map_widget> map_widget_uptr;

map_widget_uptr create_map_widget(map_widget::params_t const &params);

} // namespace qt_mapbox_wrapper