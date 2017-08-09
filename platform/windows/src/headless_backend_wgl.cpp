#include <mbgl/gl/headless_backend.hpp>
#include <mbgl/util/logging.hpp>

#include <windows.h>

#include <cassert>

namespace mbgl {

struct WGLImpl : public HeadlessBackend::Impl {
    WGLImpl(HDC deviceContext_, HGLRC glContext_) 
    : deviceContext(deviceContext_)
    , glContext(glContext_) {
    }

    ~WGLImpl() {
        wglDeleteContext(glContext);
    }

    void activateContext() final {
        if (!wglMakeCurrent(deviceContext, glContext)) {
            throw std::runtime_error("Switching OpenGL context failed.\n");
        }
    }

    HDC deviceContext = nullptr;
    HGLRC glContext = nullptr;
};

gl::ProcAddress HeadlessBackend::initializeExtension(const char* name) {
    return (void(*)()) wglGetProcAddress(name);
}

bool HeadlessBackend::hasDisplay() {
    return true;
};

void HeadlessBackend::createContext() {
    assert(!hasContext());

    HDC hdc = CreateCompatibleDC(NULL);
    
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
        PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
        32,                       //Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                       //Number of bits for the depthbuffer
        8,                        //Number of bits for the stencilbuffer
        0,                        //Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int iPixelFormat = ChoosePixelFormat(hdc, &pfd);

    if (!SetPixelFormat(hdc, iPixelFormat, &pfd)) {
        throw std::runtime_error("Error setting pixel format.");
    }

    HGLRC glContext = wglCreateContext(hdc);
    if (glContext == nullptr) {
        throw std::runtime_error("Error creating GL context object.");
    }

    impl.reset(new WGLImpl(hdc, glContext));
}

} // namespace mbgl
