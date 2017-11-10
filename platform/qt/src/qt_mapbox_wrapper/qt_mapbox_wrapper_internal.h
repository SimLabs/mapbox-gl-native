#pragma once

#include "qt_mapbox_wrapper/qt_mapbox_wrapper.h"

class QMapboxGL;

namespace qt_mapbox_wrapper 
{

struct map_internal
    : map
{
    virtual void init(uint32_t width, uint32_t height, double pixel_ratio) = 0;
    virtual QMapboxGL *get_qmapboxgl() = 0;
};

typedef std::shared_ptr<map_internal> map_internal_ptr;

map_internal_ptr create_map_internal(map_internal::params_t const &params);

struct map_proxy
    : map
{
    virtual map_internal_ptr get_internal() = 0;
};


} // namespace qt_mapbox_wrapper