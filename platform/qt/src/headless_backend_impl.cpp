#include <mbgl/gl/headless_backend.hpp>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLWindow>
#include <QWindow>

namespace mbgl 
{

namespace
{

struct QTImpl 
    : HeadlessBackend::Impl
{
    QTImpl()
        : context_(std::make_unique<QOpenGLContext>())
    {
        assert(s_offscreen_surface);
        auto format = s_offscreen_surface->format();
        context_->setFormat(format);

        auto ok = context_->create();
        assert(ok);
    }
    
    void activateContext() override
    {
        auto ok = context_->makeCurrent(s_offscreen_surface);
        assert(ok);
        auto context = QOpenGLContext::currentContext();
        assert(context == context_.get());
    }

    void deactivateContext() override
    {
        //context_->doneCurrent();
        int aaa = 5;
    }

private:
    std::unique_ptr<QOpenGLContext> context_;

public:
    typedef QOffscreenSurface offscreen_surface_t;
    static offscreen_surface_t *s_offscreen_surface;
};

QTImpl::offscreen_surface_t *QTImpl::s_offscreen_surface = nullptr;

} // namespace

gl::ProcAddress HeadlessBackend::initializeExtension(const char* name)
{
    QOpenGLContext* thisContext = QOpenGLContext::currentContext();
    return thisContext->getProcAddress(name);
}

bool HeadlessBackend::hasDisplay()
{
    return true;
}

void HeadlessBackend::createContext()
{
    impl = std::make_unique<QTImpl>();
}


void SetOffscreenSurface(void *surface)
{
    QTImpl::s_offscreen_surface = static_cast<QTImpl::offscreen_surface_t*>(surface);
}

} // namespace mbgl