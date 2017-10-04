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

//        assert(!lock_);
//        lock_ = lock_t(mutex_);
    }

    void deactivate() override
    {
//        assert(lock_);
//        lock_.reset();

        context_impl_.doneCurrent();
    }

    void set_size(mbgl::Size const& size) override
    {
        size_ = size;
    }

    uint32_t texture_id() const override
    {
        return fb_->texture.texture;
    }


//    external_lock *create_external_lock() override
//    {
//        return new external_lock_impl(this);
//    }
//
//
//    void delete_external_lock(external_lock* lock) override
//    {
//        delete lock;
//    }
//
//    bool locked() const override
//    {
//        return bool(lock_);
//    }

    
private:
    uint32_t get_texture() const
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

//private:
//    typedef std::mutex mutex_t;
//    typedef std::unique_lock<mutex_t> lock_t;
//
//    struct external_lock_impl
//        : external_lock
//    {
//        explicit external_lock_impl(backend_impl *owner)
//            : owner_(owner)
//            , lock_(owner->mutex_)
//        {}
//
//        ~external_lock_impl()
//        {
//            
//        }
//
//        external_lock_impl(external_lock_impl const &) = delete;
//        external_lock_impl &operator=(external_lock_impl const &) = delete;
//
//        uint32_t get_texture() const override
//        {
//            return owner_->get_texture();
//        }
//
//    private:
//        backend_impl *owner_;
//        lock_t lock_;
//    };

    
private:
    params_t params_;
    QOpenGLContext context_impl_;
    std::unique_ptr<fb_t> fb_;
    mbgl::Size size_;

//    mutex_t mutex_;
//    std::optional<lock_t> lock_;
};

backend_uptr create_backend(backend::params_t const &params)
{
    return std::make_unique<backend_impl>(params);
}

} // namespace mbgl_wrapper
