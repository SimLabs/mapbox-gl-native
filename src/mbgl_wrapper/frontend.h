#pragma once

#include "backend.h"

#include "mbgl/renderer/renderer_frontend.hpp"

namespace mbgl {

    class FileSource;
    class Scheduler;

} // namespace mbgl

namespace mbgl_wrapper
{

struct backend;

struct frontend
    : mbgl::RendererFrontend
{
    struct params_t
    {
        backend_ptr backend;
        std::shared_ptr<mbgl::FileSource> source;
        std::shared_ptr<mbgl::Scheduler> scheduler;
        float pixel_ratio = 1.f;
    };

};

typedef std::unique_ptr<frontend> frontend_uptr;

frontend_uptr create_frontend(frontend::params_t const &params);

} // namespace mbgl_wrapper