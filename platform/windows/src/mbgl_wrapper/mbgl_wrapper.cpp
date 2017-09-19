#include <mbgl/util/logging.hpp>
#include <mbgl/util/event.hpp>
#include <mbgl/util/default_thread_pool.hpp>
#include <mbgl/util/run_loop.hpp>
#include <mbgl/storage/default_file_source.hpp>
#include <mbgl/gl/headless_frontend.hpp>
#include <mbgl/map/map.hpp>
#include <mbgl/style/style.hpp>
#include <mbgl/util/work_task.hpp>
#include <mbgl/tile/tile_id.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "mbgl_wrapper_functions.h"
#include "logging_to_func.h"
#include "mbgl/util/shared_thread_pool.hpp"


namespace mbgl
{
    void SetOffscreenSurface(void *surface);

}

namespace mbgl_wrapper
{
std::shared_ptr<mbgl::ThreadPool> threadPool;
std::shared_ptr<mbgl::util::RunLoop> loop;
std::shared_ptr<mbgl::DefaultFileSource> fileSource;
std::shared_ptr<mbgl::HeadlessFrontend> frontend;
std::shared_ptr<mbgl::Map> map;

params_t params;

std::unique_ptr<uint8_t[]> buffer_data;
size_t buffer_capacity = 0;

void init(params_t const *params_)
{
    threadPool = mbgl::sharedThreadPool();

    params = *params_;
    mbgl::setLogFunc(params.log_f);
    mbgl::Log::Info(mbgl::Event::Setup, "Init");

    mbgl::SetOffscreenSurface(params.offscreen_surface_ptr);

    double pixelRatio = 1;

    fileSource = std::make_shared<mbgl::DefaultFileSource>("C:\\temp\\mbgl-cache.db", ".");

    std::string server_url(params.server_url);

    fileSource->setAPIBaseURL(server_url);  //"http://192.168.1.61:8080/"
    mbgl::Log::Info(mbgl::Event::Setup, "Server path: %s", server_url.c_str());

    loop = std::make_shared<mbgl::util::RunLoop>(mbgl::util::RunLoop::Type::New);

    frontend = std::make_shared<mbgl::HeadlessFrontend>(mbgl::Size(params.tile_width, params.tile_height), pixelRatio, *fileSource, *threadPool);
    
    map = std::make_shared<mbgl::Map>(*frontend, mbgl::MapObserver::nullObserver(), frontend->getSize(), pixelRatio, *fileSource, *threadPool, mbgl::MapMode::Still);

    std::string style_path = params.style_url;//"http://192.168.1.61:8080/styles/klokantech-basic/style.json";
    mbgl::Log::Info(mbgl::Event::Setup, "Style path: %s", style_path.c_str());
    if (style_path.find("://") == std::string::npos) {
        style_path = std::string("file://") + style_path;
    }

    map->getStyle().loadURL(style_path);
    map->setBearing(0); // north
    map->setPitch(0);   // top-down
    map->setPrefetchZoomDelta(0); // no prefetch
    mbgl::Log::Info(mbgl::Event::Setup, "Init completed");
}

void shutdown()
{
    mbgl::Log::Info(mbgl::Event::General, "Shutdown");
    map.reset();
    frontend.reset();
    loop.reset();
    fileSource.reset();
    threadPool.reset();
    mbgl::Log::Info(mbgl::Event::General, "Shutdown completed");
}

void update(uint32_t zoom, uint32_t x0, uint32_t y0, uint32_t width, uint32_t height)
{
    buffer_t buffer;
    buffer.zoom = zoom;
    buffer.x0 = x0;
    buffer.y0 = y0;
    buffer.width = width;
    buffer.height = height;

    try
    {
        uint32_t mbgl_y0 = (1 << zoom) - 1 - y0;
        // setting size
        mbgl::Size size = { width * params.tile_width, height * params.tile_height };
        map->setSize(size);
        frontend->setSize(size);

        // getting bounds
        mbgl::CanonicalTileID first(zoom, x0, mbgl_y0);
        mbgl::CanonicalTileID last(zoom, x0 + width - 1, mbgl_y0 - height + 1); // minus, because converted coordinate is inverted
        mbgl::LatLngBounds map_bounds(first);
        map_bounds.extend(mbgl::LatLngBounds(last));
        map->setLatLngBounds(map_bounds);

        // setting position
        map->jumpTo(map->cameraForLatLngBounds(map_bounds, mbgl::EdgeInsets()));

        // rendering
        mbgl::PremultipliedImage image = frontend->render(*map);


        buffer.buffer_format = params.desired_buffer_format;

        if (params.desired_buffer_format == bf_rgba)
        {
            if (image.channels != 4)
            {
                std::stringstream ss;
                ss << "Image has " << image.channels << " channels, 4 expected";
                throw std::runtime_error(ss.str());
            }
            
            buffer.buffer_size = image.bytes();
            buffer_data = std::move(image.data);
            buffer_capacity = image.bytes();
        }
        else if (params.desired_buffer_format == bf_png)
        {
            auto str = mbgl::encodePNG(image);

            buffer.buffer_size = str.size();

            if (buffer.buffer_size > buffer_capacity)
            {
                buffer_data = std::make_unique<uint8_t[]>(buffer.buffer_size);
                buffer_capacity = buffer.buffer_size;
            }

            if (buffer.buffer_size > 0)
                memcpy(buffer_data.get(), str.data(), buffer.buffer_size);
        }
        else
        {
            std::stringstream ss;
            ss << "Unsupported buffer format: " << params.desired_buffer_format;
            throw std::runtime_error(ss.str());
        }


        buffer.ptr = buffer_data.get();
    }
    catch (std::exception const &e)
    {
        mbgl::Log::Error(mbgl::Event::Render, "Update error: %s", e.what());

        buffer.buffer_format = bf_rgba;
        buffer.buffer_size = 0;
        buffer.ptr = nullptr;
    }


    if (params.buffer_ready_f) 
    {
        params.buffer_ready_f(params.client_handle, &buffer);
    }
}

} // namespace mbgl_wrapper