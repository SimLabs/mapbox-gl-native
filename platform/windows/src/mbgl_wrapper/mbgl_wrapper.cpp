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

#include "mbgl_wrapper_functions.h"

// for saving
#include <mbgl/util/image.hpp>

namespace mbgl_wrapper
{

std::shared_ptr<mbgl::ThreadPool> threadPool = std::make_shared<mbgl::ThreadPool>(4);
std::shared_ptr<mbgl::util::RunLoop> loop;
std::shared_ptr<mbgl::DefaultFileSource> fileSource;
std::shared_ptr<mbgl::HeadlessFrontend> frontend;
std::shared_ptr<mbgl::Map> map;

params_t params;

void init(params_t const *params_)
{
    mbgl::Log::Debug(mbgl::Event::Setup, "Init");
    params = *params_;
    double pixelRatio = 1;

    fileSource = std::make_shared<mbgl::DefaultFileSource>("C:\\temp\\mbgl-cache.db", ".");
    fileSource->setAPIBaseURL("http://localhost:8080/");

    loop = std::make_shared<mbgl::util::RunLoop>();

    frontend = std::make_shared<mbgl::HeadlessFrontend>(mbgl::Size(params.tile_width, params.tile_height), pixelRatio, *fileSource, *threadPool);
    
    map = std::make_shared<mbgl::Map>(*frontend, mbgl::MapObserver::nullObserver(), frontend->getSize(), pixelRatio, *fileSource, *threadPool, mbgl::MapMode::Still);

    std::string style_path = "http://localhost:8080/styles/klokantech-basic/style.json";
    if (style_path.find("://") == std::string::npos) {
        style_path = std::string("file://") + style_path;
    }

    map->getStyle().loadURL(style_path);
    map->setBearing(0); // north
    map->setPitch(0);   // top-down
    mbgl::Log::Debug(mbgl::Event::Setup, "Init completed");
}

void shutdown()
{
    mbgl::Log::Debug(mbgl::Event::General, "Shutdown");
    map.reset();
    frontend.reset();
    loop.reset();
    fileSource.reset();
    threadPool.reset();
    mbgl::Log::Debug(mbgl::Event::General, "Shutdown completed");
}

const std::string save_path = "C:\\Users\\user\\Desktop\\kek.png";
const std::string save_path_image = "C:\\Users\\user\\Desktop\\kek_image.png";

void save_buffer(mbgl_wrapper::buffer_t const *buffer) {
    static bool saved;
    
    if (saved) {
        return;
    }

    try {
        std::cerr << "opened stream\n";
        std::ofstream out(save_path, std::ios::binary);
        std::size_t width = buffer->width * params.tile_width;
        std::size_t height = buffer->height * params.tile_height;
        std::cerr << "trying to save " << width << "x" << height << " image of size " << buffer->buffer_size << "\n";;
        out << mbgl::encodePNG(mbgl::PremultipliedImage({width, height}, buffer->ptr, buffer->buffer_size));
        std::cerr << "wrote\n";
        out.close();
        saved = true;
    } catch(std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::exit(1);
    }
    // std::size_t width = buffer->width;
    // Magick::Image image(width, buffer->height, "RGBA", Magick::CharPixel, buffer->ptr);
    // image.write("C:\\Users\\user\\Desktop\\kek.png");
}

void save_image(mbgl::PremultipliedImage &image) {
    static bool saved;
    
    if (saved) {
        return;
    }

    try {
        std::cerr << "opened stream\n";
        std::ofstream out(save_path_image, std::ios::binary);
        out << mbgl::encodePNG(image);
        std::cerr << "wrote\n";
        out.close();
        saved = true;
    } catch(std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::exit(1);
    }
    // std::size_t width = buffer->width;
    // Magick::Image image(width, buffer->height, "RGBA", Magick::CharPixel, buffer->ptr);
    // image.write("C:\\Users\\user\\Desktop\\kek.png");
}

void update(uint32_t zoom, uint32_t x0, uint32_t y0, uint32_t width, uint32_t height)
{
    uint32_t mbgl_y0 = (1 << zoom) - 1 - y0;
    // setting size
    mbgl::Size size = { width * params.tile_width, height * params.tile_height };
    mbgl::Log::Debug(mbgl::Event::Render, "Adjusting map to size %dx%d", size.width, size.height);
    map->setSize(size);
    frontend->setSize(size);

    // getting bounds
    mbgl::Log::Debug(mbgl::Event::Render, "Getting bounds");
    mbgl::CanonicalTileID first(zoom, x0, mbgl_y0);
    mbgl::CanonicalTileID last(zoom, x0 + width - 1, mbgl_y0 + height - 1); 
    mbgl::LatLngBounds map_bounds(first);
    map_bounds.extend(mbgl::LatLngBounds(last));
    map->setLatLngBounds(map_bounds);

    // setting position
    map->jumpTo(map->cameraForLatLngBounds(map_bounds, mbgl::EdgeInsets()));

    // rendering
    mbgl::Log::Debug(mbgl::Event::Render, "Rendering");
    mbgl::PremultipliedImage image = frontend->render(*map);
    mbgl::Log::Debug(mbgl::Event::Render, "Got image of size %dx%d", image.size.width, image.size.height);
    if (params.buffer_ready_f) {
        buffer_t *buffer = new buffer_t;
        buffer->zoom = zoom;
        buffer->x0 = x0;
        buffer->y0 = y0;
        buffer->width = width;
        buffer->height = height;
        buffer->bytes_per_pixel = image.channels;
        buffer->buffer_size = image.bytes();
        uint8_t *ptr = new uint8_t[buffer->buffer_size];
        std::copy(image.data.get(), image.data.get() + buffer->buffer_size, ptr);
        buffer->ptr = ptr;

        mbgl::Log::Debug(mbgl::Event::Render, "Finished rendering callback invocation");
        params.buffer_ready_f(params.client_handle, buffer);
        // log_stream << "Async callback" << std::endl;
        // loop->invoke(params.buffer_ready_f, params.client_handle, buffer);
    }
    mbgl::Log::Debug(mbgl::Event::Render, "Update completed");
}

} // namespace mbgl_wrapper