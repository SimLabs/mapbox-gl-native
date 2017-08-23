#pragma once

#include "mbgl_wrapper.h"
#include "mbgl-wrapper_export.h"

namespace mbgl_wrapper
{

extern "C" {
    void MBGL_WRAPPER_EXPORT init(params_t const *params);    
    void MBGL_WRAPPER_EXPORT shutdown();    
    void MBGL_WRAPPER_EXPORT update(uint32_t zoom, uint32_t x0, uint32_t y0, uint32_t width, uint32_t height);
}

}