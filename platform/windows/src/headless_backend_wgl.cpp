#include <mbgl/gl/headless_backend.hpp>
#include <mbgl/gl/headless_display.hpp>
#include <mbgl/util/logging.hpp>
#include <mbgl/util/event.hpp>

#include <GL/glew.h>
#include <windows.h>

#include <cassert>

namespace mbgl {

struct WGLImpl : public HeadlessBackend::Impl {
    WGLImpl(HDC deviceContext_, HGLRC glContext_) 
    : deviceContext(deviceContext_)
    , glContext(glContext_) {
        mbgl::Log::Debug(mbgl::Event::OpenGL, "Initialized WGLImpl");
    }

    ~WGLImpl() {
        wglDeleteContext(glContext);
        mbgl::Log::Debug(mbgl::Event::OpenGL, "Destructed WGLImpl");
    }

    void activateContext() final {
        mbgl::Log::Debug(mbgl::Event::OpenGL, "Activating context");
        if (!wglMakeCurrent(deviceContext, glContext)) {
            throw std::runtime_error("Switching OpenGL context failed.\n");
        }
        glewInit();
        mbgl::Log::Debug(mbgl::Event::OpenGL, "Activated context");
    }

    void deactivateContext() final {
        mbgl::Log::Debug(mbgl::Event::OpenGL, "Deactivating context");
        if (!wglMakeCurrent(nullptr, nullptr)) {
            throw std::runtime_error("Removing OpenGL context failed.\n");
        }
        mbgl::Log::Debug(mbgl::Event::OpenGL, "Deactivated context");
    }

    HDC deviceContext = nullptr;
    HGLRC glContext = nullptr;
};

gl::ProcAddress HeadlessBackend::initializeExtension(const char* name) {
    mbgl::Log::Debug(mbgl::Event::OpenGL, "Initializing extension %s", name);
    return (void(*)()) wglGetProcAddress(name);
}

bool HeadlessBackend::hasDisplay() {
    if (!display) {
        display.reset(HeadlessDisplay::create());
    }
    return bool(display);
};

void HeadlessBackend::createContext() {
    mbgl::Log::Debug(mbgl::Event::OpenGL, "Creating context");
    assert(!hasContext());
    mbgl::Log::Debug(mbgl::Event::OpenGL, "Creating context - context does not exist");
    assert(hasDisplay());
    mbgl::Log::Debug(mbgl::Event::OpenGL, "Creating context - display is initialized");

    HDC hdc = display->attribute<HDC>();

    HGLRC glContext = wglCreateContext(hdc);
    if (glContext == nullptr) {
        throw std::runtime_error("Error creating GL context object: " + std::to_string(GetLastError()));
    }
    mbgl::Log::Debug(mbgl::Event::OpenGL, "Creating context - Initializing WGLImpl");

    impl = std::make_unique<WGLImpl>(hdc, glContext);
    mbgl::Log::Debug(mbgl::Event::OpenGL, "Created context");
}

} // namespace mbgl
