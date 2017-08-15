#include <mbgl/gl/headless_backend.hpp>
#include <mbgl/gl/headless_display.hpp>
#include <mbgl/util/logging.hpp>

#include <iostream>
#include <GL/glew.h>
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

    void deactivateContext() final {
        if (!wglMakeCurrent(nullptr, nullptr)) {
            throw std::runtime_error("Removing OpenGL context failed.\n");
        }
    }

    HDC deviceContext = nullptr;
    HGLRC glContext = nullptr;
};

gl::ProcAddress HeadlessBackend::initializeExtension(const char* name) {
    return (void(*)()) wglGetProcAddress(name);
}

bool HeadlessBackend::hasDisplay() {
    if (!display) {
        display.reset(HeadlessDisplay::create());
    }
    return bool(display);
};

void HeadlessBackend::createContext() {
    assert(!hasContext());

    HDC hdc = display->attribute<HDC>();

    HGLRC glContext = wglCreateContext(hdc);
    if (glContext == nullptr) {
        throw std::runtime_error("Error creating GL context object: " + std::to_string(GetLastError()));
    }

    std::cerr << "  glewInit\n";
    glewInit();

    impl = std::make_unique<WGLImpl>(hdc, glContext);
}

} // namespace mbgl
