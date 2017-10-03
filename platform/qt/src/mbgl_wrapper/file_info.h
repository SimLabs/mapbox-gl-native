#pragma once

#include <QFileInfo>
#include <QDateTime>

namespace mbgl_wrapper {

namespace file_info
{

typedef QDateTime timestamp_t;

inline timestamp_t last_modified(std::string const &filename)
{
    return QFileInfo(QString::fromStdString(filename)).lastModified();
}

} // namespace file_info

} // namespace mbgl_wrapper