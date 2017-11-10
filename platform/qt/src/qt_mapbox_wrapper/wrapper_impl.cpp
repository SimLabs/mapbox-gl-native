#include "qt_mapbox_wrapper/qt_mapbox_wrapper.h"

#include "safe_run.h"

#include <QMapboxGL>

namespace mbgl
{
    void set_log_pfn(qt_mapbox_wrapper::log_pfn pfn);
    void set_verbose_logging(bool value);
}


namespace qt_mapbox_wrapper
{

 
QT_MAPBOX_WRAPPER_API void set_log_pfn(log_pfn pfn) 
{
    SAFE_RUN_BEGIN()

    mbgl::set_log_pfn(pfn);
    QMapbox::SetLogPfn(pfn);

    SAFE_RUN_END()
}

QT_MAPBOX_WRAPPER_API void set_verbose_logging(bool value)
{
    SAFE_RUN_BEGIN()

    mbgl::set_verbose_logging(value);
    QMapbox::SetVerboseLogging(value);

    SAFE_RUN_END()
}



} // namespace qt_mapbox_wrapper

