#include <mbgl/gl/headless_display.hpp>

#include <windows.h>

#include <iostream>
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <string>

namespace mbgl {

class HeadlessDisplay::Impl {
public:
    Impl();
    ~Impl();

    HDC hdc = nullptr;

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

    int iPixelFormat;
};

HeadlessDisplay::Impl::Impl() {
    std::cerr << "CONSTRUCTING DISPLAY\n";

    hdc = CreateCompatibleDC(NULL);
    assert(hdc);
  
    iPixelFormat = ChoosePixelFormat(hdc, &pfd);

    if (!iPixelFormat) {
        throw std::runtime_error("Error choosing pixel format: " + std::to_string(GetLastError()));
    }

    std::cerr << "  Trying to set pixel format\n";
    auto result = SetPixelFormat(hdc, iPixelFormat, &pfd);
    if (result == FALSE) {
        auto errcode = GetLastError();
        std::cerr << "Could not set pixel format: " << errcode << "\n";
        // throw std::runtime_error("Error setting pixel format: " + std::to_string(errcode));
    }
    std::cerr << "CONSTRUCTED DISPLAY\n";
}

HeadlessDisplay::Impl::~Impl() {
    std::cerr << "DESTROYING DISPLAY\n";
    DeleteDC(hdc);
    hdc = nullptr;
    std::cerr << "DESTROYED DISPLAY\n";
}

template <>
HDC HeadlessDisplay::attribute() const {
    return impl->hdc;
}

template <>
PIXELFORMATDESCRIPTOR HeadlessDisplay::attribute() const {
    return impl->pfd;
}

HeadlessDisplay::HeadlessDisplay()
    : impl(std::make_unique<Impl>()) {
}

HeadlessDisplay::~HeadlessDisplay() = default;

} // namespace mbgl
