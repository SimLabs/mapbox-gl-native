#include <mbgl/util/logging.hpp>
#include <mbgl/util/enum.hpp>

#include <iostream>

#include <windows.h>
#include <fstream>

#include "qt_mapbox_wrapper/qt_mapbox_wrapper.h"
#include <sstream>

namespace mbgl {

    qt_mapbox_wrapper::log_pfn g_log_pfn = nullptr;

    void set_log_pfn(qt_mapbox_wrapper::log_pfn pfn)
    {
        g_log_pfn = pfn;
    }
    
    void Log::platformRecord(EventSeverity severity, const std::string &message)
    {
        if (g_log_pfn)
        {
            g_log_pfn(qt_mapbox_wrapper::log_severity_t(severity), message.c_str());
        }
        else
        {
            std::cerr << message << std::endl;

            OutputDebugStringA("Mbgl log: ");
            OutputDebugStringA(message.c_str());
            OutputDebugStringA("\n");        
        }
    }

} // namespace mbgl
