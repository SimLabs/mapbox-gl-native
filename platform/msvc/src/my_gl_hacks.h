#pragma once


 
#include <QOpenGLFunctions_3_0>
 
 namespace mbgl
 {
 
    typedef QOpenGLFunctions_3_0 GlFunctions;
 
     GlFunctions *getGlFunctions();
 }
 

#define MY_GL_FUNCTION(name) mbgl::getGlFunctions()->name
