#include <mbgl/gl/headless_frontend.hpp>
#include <mbgl/renderer/renderer.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/util/run_loop.hpp>
#include <mbgl/util/logging.hpp>
#include <mbgl/util/event.hpp>

namespace mbgl {

HeadlessFrontend::HeadlessFrontend(float pixelRatio_, FileSource& fileSource, Scheduler& scheduler)
    : HeadlessFrontend({ 256, 256 }, pixelRatio_, fileSource, scheduler) {
}

HeadlessFrontend::HeadlessFrontend(Size size_, float pixelRatio_, FileSource& fileSource, Scheduler& scheduler)
    : size(size_),
    pixelRatio(pixelRatio_),
    backend({ static_cast<uint32_t>(size.width * pixelRatio),
              static_cast<uint32_t>(size.height * pixelRatio) }),
    asyncInvalidate([this] {
        if (renderer && updateParameters) {
            mbgl::BackendScope guard { backend };
            renderer->render(*updateParameters);
        }
    }),
    renderer(std::make_unique<Renderer>(backend, pixelRatio, fileSource, scheduler)) {
    mbgl::Log::Debug(mbgl::Event::Render, "Initialized headless frontend");
}

HeadlessFrontend::~HeadlessFrontend() = default;

void HeadlessFrontend::reset() {
    assert(renderer);
    renderer.reset();
}

void HeadlessFrontend::update(std::shared_ptr<UpdateParameters> updateParameters_) {
    updateParameters = updateParameters_;
    asyncInvalidate.send();
}

void HeadlessFrontend::setObserver(RendererObserver& observer_) {
    assert(renderer);
    renderer->setObserver(&observer_);
}

Size HeadlessFrontend::getSize() const {
    return size;
}

Renderer* HeadlessFrontend::getRenderer() {
    assert(renderer);
    return renderer.get();
}

RendererBackend* HeadlessFrontend::getBackend() {
    return &backend;
}

void HeadlessFrontend::setSize(Size size_) {
    if (size != size_) {
        size = size_;
        backend.setSize({ static_cast<uint32_t>(size_.width * pixelRatio),
                          static_cast<uint32_t>(size_.height * pixelRatio) });
    }
}

PremultipliedImage HeadlessFrontend::readStillImage() {
    return backend.readStillImage();
}

PremultipliedImage HeadlessFrontend::render(Map& map) {
    mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- begin");
    PremultipliedImage result;

    mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- asking map to render itself");
    map.renderStill([&](std::exception_ptr error) {
        mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- executing callback");
        if (error) {
            mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- error encountered");
            std::rethrow_exception(error);
        } else {
            mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- ok -- reading still image");
            result = backend.readStillImage();
            mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- ok -- read still image");
        }
    });

    mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- waiting for map to render");
    while (!result.valid()) {
        util::RunLoop::Get()->runOnce();
    }

    mbgl::Log::Debug(mbgl::Event::Render, "HeadlessFrontend::render -- done");
    return result;
}

} // namespace mbgl
