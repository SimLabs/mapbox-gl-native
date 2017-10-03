#include <mbgl/util/logging.hpp>
#include <mbgl/util/enum.hpp>

#include <iostream>

#include <windows.h>
#include <fstream>

namespace mbgl {


    void Log::platformRecord(EventSeverity severity, const std::string &message)
    {
        static std::ofstream s("c:\\temp\\mbgl_log.txt");

        s << message << std::endl;
        s.flush();
        
        OutputDebugStringA("Mbgl log: ");
        OutputDebugStringA(message.c_str());
        OutputDebugStringA("\n");
    }

} // namespace mbgl
