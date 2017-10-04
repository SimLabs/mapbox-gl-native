#include "mbgl_wrapper/mbgl_wrapper.h"
#include "backend.h"

#include <QSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Compatibility>
#include <mutex>


#include "mbgl/gl/context.hpp"
#include "mbgl/gl/types.hpp"
#include "mbgl/gl/texture.hpp"
#include "mbgl/gl/renderbuffer.hpp"
#include "mbgl/gl/framebuffer.hpp"

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

            fbs_.emplace_back(mbgl_context, params_.max_size);
            fbs_.emplace_back(mbgl_context, params_.max_size);
        }
    }

    ~backend_impl()
    {
        {
            mbgl::BackendScope guard{ *this };
            fbs_.clear();
            context.reset();
        }
    }

    void updateAssumedState() override
    {

    }

    void bind() override
    {
        auto &context = getContext();
        
        uint32_t fb_index;
        
        {
            std::lock_guard<std::mutex> lock(textures_mutex_);

            assert(!textures_.rendered);

            uint32_t const other = textures_.locked.get_value_or(textures_.latest);
            fb_index = (other + 1) % fbs_.size();

            textures_.rendered = fb_index;
        }

        context.bindFramebuffer = fbs_.at(fb_index).framebuffer.framebuffer;
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


    uint32_t lock_texture() override
    {
        std::lock_guard<std::mutex> lock(textures_mutex_);

        assert(!textures_.locked);
        textures_.locked = textures_.latest;
        return fbs_.at(textures_.latest).texture.texture;
    }

    void unlock_texture() override
    {
        std::lock_guard<std::mutex> lock(textures_mutex_);
        
        assert(textures_.locked);
        textures_.locked.reset();
    }

    void finish_render() override
    {
        std::lock_guard<std::mutex> lock(textures_mutex_);

        if (!textures_.rendered)
            return;

        textures_.latest = *textures_.rendered;
        textures_.rendered.reset();
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

    std::vector<fb_t> fbs_;
    mbgl::Size size_;

    struct
    {
        uint32_t latest = 0;
        std::optional<uint32_t> rendered;
        std::optional<uint32_t> locked;
    } textures_;

    std::mutex textures_mutex_;
};

backend_uptr create_backend(backend::params_t const &params)
{
    return std::make_unique<backend_impl>(params);
}

} // namespace mbgl_wrapper
