#include "platform/msvc/src/my_gl_hacks.h"

#include <cassert>

#include <QOpenGLContext>

namespace mbgl
{

    GlFunctions *getGlFunctions()
    {
        auto context = QOpenGLContext::currentContext();
        assert(context);
        
        return context->versionFunctions<GlFunctions>();

    }
}
