#pragma once

#include <QVariantMap>

namespace qt_mapbox_wrapper 
{

struct parse_json_error
    : std::runtime_error
{
    explicit parse_json_error(std::string const &str)
        : std::runtime_error(str)
    {
                
    }
};

QVariantMap parse_json(char const *json_cstr);


} // namespace qt_mapbox_wrapper