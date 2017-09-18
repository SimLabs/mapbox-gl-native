#include "my_gl_hacks.h"

#include <QOpenGLContext>
QOpenGLFunctions_4_3_Compatibility *get_qt_opengl_functions()
{
    return QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Compatibility>();
}

