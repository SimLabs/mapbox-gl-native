#include <mbgl/util/event.hpp>
#include <mbgl/util/enum.hpp>

namespace mbgl {

MBGL_ENUM_BEGIN(EventSeverity) {
    { EventSeverity::Debug, "DEBUG" },
    { EventSeverity::Info, "INFO" },
    { EventSeverity::Warning, "WARNING" },
    { EventSeverity::Error, "ERROR" },
    { EventSeverity(-1), "UNKNOWN" },
} MBGL_ENUM_END(EventSeverity);

MBGL_ENUM_BEGIN(Event) {
    { Event::General, "General" },
    { Event::Setup, "Setup" },
    { Event::Shader, "Shader" },
    { Event::ParseStyle, "ParseStyle" },
    { Event::ParseTile, "ParseTile" },
    { Event::Render, "Render" },
    { Event::Style, "Style" },
    { Event::Database, "Database" },
    { Event::HttpRequest, "HttpRequest" },
    { Event::Sprite, "Sprite" },
    { Event::Image, "Image" },
    { Event::OpenGL, "OpenGL" },
    { Event::JNI, "JNI" },
    { Event::Android, "Android" },
    { Event::Crash, "Crash" },
    { Event::Glyph, "Glyph" },
    { Event(-1), "Unknown" },
} MBGL_ENUM_END(Event);


} // namespace mbgl
