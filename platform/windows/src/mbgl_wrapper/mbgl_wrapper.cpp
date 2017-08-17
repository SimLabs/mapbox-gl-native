#include "mbgl_wrapper/mbgl_wrapper.h"

#include <mbgl/util/default_thread_pool.hpp>
#include <mbgl/util/run_loop.hpp>
#include <mbgl/storage/default_file_source.hpp>
#include <mbgl/gl/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/work_task.hpp>
#include <mbgl/tile/tile_id.hpp>

#include <memory>

#define __HELPER_DL_EXPORT __attribute__((used, visibility ("default")))

namespace mbgl_wrapper
{

extern "C"
{
    __HELPER_DL_EXPORT void init(params_t const *params);    
    __HELPER_DL_EXPORT void shutdown();    
    __HELPER_DL_EXPORT void update(uint32_t zoom, uint32_t x0, uint32_t y0, uint32_t width, uint32_t height);
} // extern "C"


std::shared_ptr<mbgl::ThreadPool> threadPool = std::make_shared<mbgl::ThreadPool>(4);
std::shared_ptr<mbgl::util::RunLoop> loop;
std::shared_ptr<mbgl::DefaultFileSource> fileSource;
std::shared_ptr<mbgl::HeadlessFrontend> frontend;
std::shared_ptr<mbgl::Map> map;

params_t const *params;

void init(params_t const *params_)
{
    params = params_;
    double pixelRatio = 1;

    fileSource = std::make_shared<mbgl::DefaultFileSource>("C:\\temp\\mbgl-cache.db", ".");
    fileSource->setAPIBaseURL("http://localhost:8080/");

    loop = std::make_shared<mbgl::util::RunLoop>();

    frontend = std::make_shared<mbgl::HeadlessFrontend>(mbgl::Size(params->tile_width, params->tile_height), pixelRatio, *fileSource, *threadPool);
    
    map = std::make_shared<mbgl::Map>(*frontend, mbgl::MapObserver::nullObserver(), frontend->getSize(), pixelRatio, *fileSource, *threadPool, mbgl::MapMode::Still);

    std::string style_path = "http://localhost:8080/styles/klokantech-basic/style.json";
    if (style_path.find("://") == std::string::npos) {
        style_path = std::string("file://") + style_path;
    }

    map->getStyle().loadURL(style_path);
    map->setBearing(0); // north
    map->setPitch(0);   // top-down
}

void shutdown()
{
    map.reset();
    frontend.reset();
    loop.reset();
    fileSource.reset();
    threadPool.reset();
}

void update(uint32_t zoom, uint32_t x0, uint32_t y0, uint32_t width, uint32_t height)
{
    // getting bounds
    mbgl::CanonicalTileID first(zoom, x0, y0);
    mbgl::CanonicalTileID last(zoom, x0 + width, y0 + height);
    mbgl::LatLngBounds map_bounds(first);
    map_bounds.extend(mbgl::LatLngBounds(last));

    // setting bounds
    map->jumpTo(map->cameraForLatLngBounds(map_bounds, mbgl::EdgeInsets()));
    // map->setSize({ width, height });

    mbgl::PremultipliedImage image = frontend->render(*map);
    if (params->buffer_ready_f) {
        buffer_t buffer;
        buffer.zoom = zoom;
        buffer.x0 = x0;
        buffer.y0 = y0;
        buffer.width = width;
        buffer.height = height;
        buffer.bytes_per_pixel = image.channels;
        buffer.buffer_size = image.bytes();
        buffer.ptr = image.data.get();

        loop->invoke(params->buffer_ready_f, params->client_handle, &buffer);
    }
}

} // namespace mbgl_wrapper