#include <mbgl/gl/headless_display.hpp>

#include <windows.h>

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <string>

namespace mbgl {

class HeadlessDisplay::Impl {
public:
    Impl();
    ~Impl();

    HWND hwnd = nullptr;
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

// dummy function
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

HeadlessDisplay::Impl::Impl() {
    // Window description  
    WNDCLASSEX ex;
    ex.cbSize = sizeof(WNDCLASSEX);
    ex.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    ex.lpfnWndProc = WinProc;
    ex.cbClsExtra = 0;
    ex.cbWndExtra = 0;
    ex.hInstance = GetModuleHandle(nullptr);
    ex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    ex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    ex.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    ex.lpszMenuName = nullptr;
    ex.lpszClassName = "wndclass";
    ex.hIconSm = nullptr;
     
    RegisterClassEx(&ex);
     
    // Window creation
    hwnd = CreateWindowEx(NULL,
        ex.lpszClassName,
        "Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        ex.hInstance,
        NULL);
    assert(hwnd);

    hdc = GetDC(hwnd);
    assert(hdc);
  
    iPixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (!iPixelFormat) {
        throw std::runtime_error("Error choosing pixel format: " + std::to_string(GetLastError()));
    }

    auto result = SetPixelFormat(hdc, iPixelFormat, &pfd);
    if (result == FALSE) {
        throw std::runtime_error("Error setting pixel format: " + std::to_string(GetLastError()));
    }
}

HeadlessDisplay::Impl::~Impl() {
    DeleteDC(hdc);
    hdc = nullptr;
    DestroyWindow(hwnd);
    hwnd = nullptr;
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
