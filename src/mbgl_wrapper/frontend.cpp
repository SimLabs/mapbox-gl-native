#include "frontend.h"
#include "mbgl/renderer/renderer.hpp"
#include "mbgl/util/async_task.hpp"
#include "mbgl/renderer/update_parameters.hpp"

namespace mbgl_wrapper
{


struct frontend_impl
    : frontend
{
    explicit frontend_impl(params_t const &params)
        : params_(params)
        , renderer_(std::make_unique<mbgl::Renderer>(*params.backend, params.pixel_ratio, *params.source, *params.scheduler))
        , async_invalidate_(std::bind(&frontend_impl::render_internal, this))
    {}

    ~frontend_impl()
    {
        
    }

public:
    void reset() override
    {
        assert(renderer_);
        renderer_.reset();
    }

    void setObserver(mbgl::RendererObserver& observer) override
    {
        assert(renderer_);
        renderer_->setObserver(&observer);
    }

    void update(std::shared_ptr<mbgl::UpdateParameters> update_parameters) override
    {
        update_parameters_ = update_parameters;
        async_invalidate_.send();
    }

    mbgl::TransformState const* current_state() const override
    {
        if (!update_parameters_)
            return nullptr;

        return &update_parameters_->transformState;
    }

private:
    void render_internal()
    {
        assert(params_.backend);
        assert(renderer_);
        assert(update_parameters_);
        
        {
            mbgl::BackendScope guard{ *params_.backend };
            renderer_->render(*update_parameters_);
        }
    }


private:
    params_t params_;
    std::unique_ptr<mbgl::Renderer> renderer_;

    mbgl::util::AsyncTask async_invalidate_;
    std::shared_ptr<mbgl::UpdateParameters> update_parameters_;
};


frontend_uptr create_frontend(frontend::params_t const &params)
{
    return std::make_unique<frontend_impl>(params);
}

    
} // namespace mbgl_wrapper
