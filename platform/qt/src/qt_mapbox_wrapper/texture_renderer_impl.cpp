#include <QOpenGLContext>
#include <QMapboxGL>
#ifdef WIN32
#   include <QtPlatformHeaders/QWGLNativeContext>
#else
#   include <QtPlatformHeaders/QGLXNativeContext>
#endif
#include "mbgl/util/logging.hpp"
#include <QOffscreenSurface>

#include <cassert>

#include "qt_mapbox_wrapper_internal.h"

#include "safe_run.h"

namespace qt_mapbox_wrapper 
{

struct texture_renderer_impl
    : texture_renderer
{
    explicit texture_renderer_impl(params_t const &params, map_internal_ptr map_internal)
        : params_(params)
        , map_internal_(map_internal)
    {}


    void init(init_params_t const &params) override
    {
        SAFE_RUN_BEGIN()
        
        surface_.create();

        bool ok;

// WARNING!!! It's may be not working on linux
#ifdef WIN32
        QWGLNativeContext native_ctx((HGLRC)params.hglrc, (HWND)params.hwnd);
#else
        QGLXNativeContext native_ctx((GLXContext)params.hglrc, 0, (Window)params.hwnd);
#endif
        main_context_.setNativeHandle(QVariant::fromValue(native_ctx));
        ok = main_context_.create();
        assert(ok && "main_context.create failed");

        test_context_.setShareContext(&main_context_);
        ok = test_context_.create();
        assert(ok && "test_context.create failed");

        assert(main_context_.shareGroup() == test_context_.shareGroup());
        
        map_internal_->init(params.width, params.height, 1.);

        SAFE_RUN_END()
    }

    void render(uint32_t fbo) override
    {
        SAFE_RUN_BEGIN()
        
        auto qmapboxgl = map_internal_->get_qmapboxgl();

        bool ok = test_context_.makeCurrent(&surface_);
        assert(ok && "test_context.makeCurrent failed");

        qmapboxgl->setFramebufferObject(fbo);
        qmapboxgl->render();

        test_context_.doneCurrent();

        SAFE_RUN_END()
    }


    void set_pos(double lat, double lon, double zoom, double bearing) override
    {
        SAFE_RUN_BEGIN()

        auto *qmapboxgl = map_internal_->get_qmapboxgl();
        
        QMapbox::Coordinate coord(lat, lon);
        qmapboxgl->setCoordinateZoom(coord, zoom);
        qmapboxgl->setBearing(bearing);

        SAFE_RUN_END()
    }

private:
    params_t params_;
    map_internal_ptr map_internal_;
    QOpenGLContext main_context_, test_context_;
    QOffscreenSurface surface_;
};

QT_MAPBOX_WRAPPER_API texture_renderer *create_texture_renderer(texture_renderer::params_t const &params)
{
    SAFE_RUN_BEGIN()

    return new texture_renderer_impl(params, dynamic_cast<map_proxy *>(params.m)->get_internal());

    SAFE_RUN_END_RET(nullptr)
}

QT_MAPBOX_WRAPPER_API void release_texture_renderer(texture_renderer *p)
{
    SAFE_RUN_BEGIN()

    delete p;    

    SAFE_RUN_END()
}


} // namespace qt_mapbox_wrapper

