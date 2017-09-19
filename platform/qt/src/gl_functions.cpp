#include "my_gl_hacks.h"

#include <QOpenGLContext>

namespace mbgl
{

    GlFunctions *getGlFunctions()
    {
        auto context = QOpenGLContext::currentContext();
        assert(context);
        
        return context->versionFunctions<QOpenGLFunctions_4_3_Compatibility>();

    }
}
