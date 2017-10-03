#include "mbgl_wrapper/mbgl_wrapper.h"
#include "backend.h"

#include <QSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Compatibility>
#include "src/mbgl/gl/context.hpp"
#include "src/mbgl/gl/types.hpp"
#include "src/mbgl/gl/texture.hpp"
#include "src/mbgl/gl/renderbuffer.hpp"
#include "src/mbgl/gl/framebuffer.hpp"

namespace mbgl_wrapper
{

struct backend_impl
    : backend
{
    explicit backend_impl(params_t const &params)
        : params_(params)
    {
        auto format = params_.surface->format();
        context_impl_.setFormat(params.main_context->format());

        context_impl_.setShareContext(params.main_context);

        assert(context_impl_.shareContext() == params.main_context);
        
        auto ok = context_impl_.create();
        assert(ok);

        {
            mbgl::BackendScope guard{ *this };
            auto &mbgl_context = getContext();
            fb_ = std::make_unique<fb_t>(mbgl_context, params_.max_size);
        }
    }

    ~backend_impl()
    {
        {
            mbgl::BackendScope guard{ *this };
            fb_.reset();
            context.reset();
        }
    }

    void updateAssumedState() override
    {

    }

    void bind() override
    {
        auto &context = getContext();

        context.bindFramebuffer = fb_->framebuffer.framebuffer;
        context.scissorTest = false;
        context.viewport = { 0, 0, size_ };
    }

    mbgl::gl::ProcAddress initializeExtension(const char* name) override
    {
        QOpenGLContext* thisContext = QOpenGLContext::currentContext();
        return thisContext->getProcAddress(name);
    }

    void activate() override
    {
        auto ok = context_impl_.makeCurrent(params_.surface);
        assert(ok && "context makeCurrent failed");

        auto context = QOpenGLContext::currentContext();
        assert(context == &context_impl_);
    }

    void deactivate() override
    {
        context_impl_.doneCurrent();
    }

    void set_size(mbgl::Size const& size) override
    {
        size_ = size;
    }

    uint32_t get_texture() const override
    {
        return fb_->texture.texture;
    }

private:
    struct fb_t
    {
        fb_t(mbgl::gl::Context &context, mbgl::Size size)
            : texture(context.createTexture(size))
            , depth_stencil(context.createRenderbuffer<mbgl::gl::RenderbufferType::DepthStencil>(size))
            , framebuffer(context.createFramebuffer(texture, depth_stencil))
        {
        }
            
        
        mbgl::gl::Texture texture;
        mbgl::gl::Renderbuffer<mbgl::gl::RenderbufferType::DepthStencil> depth_stencil;
        mbgl::gl::Framebuffer framebuffer;
    };

private:
    params_t params_;
    QOpenGLContext context_impl_;
    std::unique_ptr<fb_t> fb_;
    mbgl::Size size_;
};

backend_uptr create_backend(backend::params_t const &params)
{
    return std::make_unique<backend_impl>(params);
}

} // namespace mbgl_wrapper
