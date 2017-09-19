#pragma once


//#define FOR_GL_FUNCTIONS \
//    PROCESS_GL_FUNCTION(void, glActiveTexture) \
//    PROCESS_GL_FUNCTION(void, glAttachShader) \
//    PROCESS_GL_FUNCTION(void, glBindAttribLocation) \
//    PROCESS_GL_FUNCTION(void, glBindBuffer) \
//    PROCESS_GL_FUNCTION(void, glBindFramebuffer) \
//    PROCESS_GL_FUNCTION(void, glBindRenderbuffer) \
//    PROCESS_GL_FUNCTION(void, glBindTexture) \
//    PROCESS_GL_FUNCTION(void, glBlendColor) \
//    PROCESS_GL_FUNCTION(void, glBlendEquation) \
//    PROCESS_GL_FUNCTION(void, glBlendFunc) \
//    PROCESS_GL_FUNCTION(void, glBufferData) \
//    PROCESS_GL_FUNCTION(void, glBufferSubData) \
//    PROCESS_GL_FUNCTION(GLenum, glCheckFramebufferStatus) \
//    PROCESS_GL_FUNCTION(void, glClear) \
//    PROCESS_GL_FUNCTION(void, glClearColor) \
//    PROCESS_GL_FUNCTION(void, glClearDepth) \
//    PROCESS_GL_FUNCTION(void, glClearStencil) \
//    PROCESS_GL_FUNCTION(void, glColorMask) \
//    PROCESS_GL_FUNCTION(void, glCompileShader) \
//    PROCESS_GL_FUNCTION(GLuint, glCreateProgram) \
//    PROCESS_GL_FUNCTION(GLuint, glCreateShader) \
//    PROCESS_GL_FUNCTION(void, glDeleteBuffers) \
//    PROCESS_GL_FUNCTION(void, glDeleteFramebuffers) \
//    PROCESS_GL_FUNCTION(void, glDeleteProgram) \
//    PROCESS_GL_FUNCTION(void, glDeleteRenderbuffers) \
//    PROCESS_GL_FUNCTION(void, glDeleteShader) \
//    PROCESS_GL_FUNCTION(void, glDeleteTextures) \
//    PROCESS_GL_FUNCTION(void, glDepthFunc) \
//    PROCESS_GL_FUNCTION(void, glDepthMask) \
//    PROCESS_GL_FUNCTION(void, glDepthRange) \
//    PROCESS_GL_FUNCTION(void, glDisable) \
//    PROCESS_GL_FUNCTION(void, glDisableVertexAttribArray) \
//    PROCESS_GL_FUNCTION(void, glDrawElements) \
//    PROCESS_GL_FUNCTION(void, glDrawPixels) \
//    PROCESS_GL_FUNCTION(void, glEnable) \
//    PROCESS_GL_FUNCTION(void, glEnableVertexAttribArray) \
//    PROCESS_GL_FUNCTION(void, glFramebufferRenderbuffer) \
//    PROCESS_GL_FUNCTION(void, glFramebufferTexture2D) \
//    PROCESS_GL_FUNCTION(void, glGenBuffers) \
//    PROCESS_GL_FUNCTION(void, glGenFramebuffers) \
//    PROCESS_GL_FUNCTION(void, glGenRenderbuffers) \
//    PROCESS_GL_FUNCTION(void, glGenTextures) \
//    PROCESS_GL_FUNCTION(void, glGetActiveAttrib) \
//    PROCESS_GL_FUNCTION(void, glGetActiveUniform) \
//    PROCESS_GL_FUNCTION(void, glGetBooleanv) \
//    PROCESS_GL_FUNCTION(void, glGetDoublev) \
//    PROCESS_GL_FUNCTION(GLenum, glGetError) \
//    PROCESS_GL_FUNCTION(void, glGetFloatv) \
//    PROCESS_GL_FUNCTION(void, glGetIntegerv) \
//    PROCESS_GL_FUNCTION(void, glGetProgramInfoLog) \
//    PROCESS_GL_FUNCTION(void, glGetProgramiv) \
//    PROCESS_GL_FUNCTION(void, glGetShaderInfoLog) \
//    PROCESS_GL_FUNCTION(void, glGetShaderiv) \
//    PROCESS_GL_FUNCTION(const GLubyte *, glGetString) \
//    PROCESS_GL_FUNCTION(GLint, glGetUniformLocation) \
//    PROCESS_GL_FUNCTION(GLboolean, glIsEnabled) \
//    PROCESS_GL_FUNCTION(void, glLineWidth) \
//    PROCESS_GL_FUNCTION(void, glLinkProgram) \
//    PROCESS_GL_FUNCTION(void, glPixelStorei) \
//    PROCESS_GL_FUNCTION(void, glPixelTransferf) \
//    PROCESS_GL_FUNCTION(void, glPixelZoom) \
//    PROCESS_GL_FUNCTION(void, glPointSize) \
//    PROCESS_GL_FUNCTION(void, glRasterPos4d) \
//    PROCESS_GL_FUNCTION(void, glReadPixels) \
//    PROCESS_GL_FUNCTION(void, glRenderbufferStorage) \
//    PROCESS_GL_FUNCTION(void, glShaderSource) \
//    PROCESS_GL_FUNCTION(void, glStencilFunc) \
//    PROCESS_GL_FUNCTION(void, glStencilMask) \
//    PROCESS_GL_FUNCTION(void, glStencilOp) \
//    PROCESS_GL_FUNCTION(void, glTexImage2D) \
//    PROCESS_GL_FUNCTION(void, glTexParameteri) \
//    PROCESS_GL_FUNCTION(void, glUniform1f) \
//    PROCESS_GL_FUNCTION(void, glUniform1i) \
//    PROCESS_GL_FUNCTION(void, glUniform2fv) \
//    PROCESS_GL_FUNCTION(void, glUniform3fv) \
//    PROCESS_GL_FUNCTION(void, glUniform4fv) \
//    PROCESS_GL_FUNCTION(void, glUniformMatrix2fv) \
//    PROCESS_GL_FUNCTION(void, glUniformMatrix3fv) \
//    PROCESS_GL_FUNCTION(void, glUniformMatrix4fv) \
//    PROCESS_GL_FUNCTION(void, glUseProgram) \
//    PROCESS_GL_FUNCTION(void, glVertexAttribPointer) \
//    PROCESS_GL_FUNCTION(void, glViewport) 
//
//template<typename Ret>
//struct my_gl_function_base_t
//{
//    template<typename... Args>
//    Ret operator()(Args&&...) const
//    {
//        return Ret(0);
//    }
//};
//
//template<>
//struct my_gl_function_base_t<void>
//{
//    template<typename... Args>
//    void operator()(Args&&...) const
//    {
//        
//    }
//};
//
//
//#define PROCESS_GL_FUNCTION(ret, name) \
//    struct my_gl_function_##name##_t \
//        : my_gl_function_base_t<ret> \
//    {};
//    
//FOR_GL_FUNCTIONS
//
//#undef PROCESS_GL_FUNCTION
//
//
//
//#define MY_GL_FUNCTION(name) my_gl_function_##name##_t()

#include <QOpenGLFunctions_4_3_Compatibility>

namespace mbgl
{

    typedef QOpenGLFunctions_4_3_Compatibility GlFunctions;

    GlFunctions *getGlFunctions();
}


#define MY_GL_FUNCTION(name) getGlFunctions()->##name