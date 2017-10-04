#pragma once

#include "mbgl_wrapper/mbgl_wrapper.h"
#include "mbgl/renderer/renderer_backend.hpp"

class QSurface;
class QOpenGLContext;

namespace mbgl_wrapper
{

struct backend
    : mbgl::RendererBackend
{
    struct params_t
    {
        QSurface *surface = nullptr;
        QOpenGLContext *main_context = nullptr;
        mbgl::Size max_size;
    };

    virtual void set_size(mbgl::Size const &size) = 0;

    virtual uint32_t lock_texture() = 0;
    virtual void unlock_texture() = 0;
    virtual void finish_render() = 0;

};

typedef std::unique_ptr<backend> backend_uptr;
typedef std::shared_ptr<backend> backend_ptr;

backend_uptr create_backend(backend::params_t const &params);

} // namespace mbgl_wrapper