#pragma once


 
 #include <QOpenGLFunctions_4_3_Compatibility>
 
 namespace mbgl
 {
 
     typedef QOpenGLFunctions_4_3_Compatibility GlFunctions;
 
     GlFunctions *getGlFunctions();
 }
 

#define MY_GL_FUNCTION(name) mbgl::getGlFunctions()->name