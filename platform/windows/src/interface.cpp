#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "interface.h"

#include <mbgl/map/map.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/run_loop.hpp>

#include "verbose_map_observer.hpp"
#include <mbgl/gl/headless_frontend.hpp>
#include <mbgl/gl/headless_backend.hpp>
#include <mbgl/util/default_thread_pool.hpp>
#include <mbgl/storage/default_file_source.hpp>
#include <mbgl/style/style.hpp>

std::shared_ptr<mbgl::ThreadPool> threadPool;
std::shared_ptr<mbgl::util::RunLoop> loop;
std::shared_ptr<mbgl::DefaultFileSource> fileSource;
std::shared_ptr<mbgl::HeadlessFrontend> frontend;
std::shared_ptr<mbgl::Map> map;

void init(camera_params_t const *camera_params, map_params_t const *map_params) {
    std::cerr << "  camera_params\n";
    double lat = camera_params->latitude;
    double lon = camera_params->longitude;
    double zoom = camera_params->zoom;
    double bearing = camera_params->bearing;
    double pitch = camera_params->pitch;
    double pixelRatio = camera_params->pixel_ratio;
    uint32_t width = camera_params->width;
    uint32_t height = camera_params->height;

    std::cerr << "  map_params\n";
    std::string style_path = map_params->style_path;
    std::string api_base_url = map_params->api_base_url;
    std::string cache_file = map_params->cache_file;
    std::string asset_root = map_params->asset_root;
    std::string token = map_params->token;
    bool debug = map_params->debug;

    std::cerr << "  loop\n";
    loop = std::make_shared<mbgl::util::RunLoop>();

    std::cerr << "  threadPool\n";
    threadPool = std::make_shared<mbgl::ThreadPool>(4);

    std::cerr << "  fileSource\n";
    fileSource = std::make_shared<mbgl::DefaultFileSource>(cache_file, asset_root);

    // Set access token if present
    if (token.size()) {
        fileSource->setAccessToken(token);
    }

    fileSource->setAPIBaseURL(api_base_url);

    std::cerr << "  frontend\n";
    frontend = std::make_shared<mbgl::HeadlessFrontend>(mbgl::Size(width, height), pixelRatio, *fileSource, *threadPool);
    std::cerr << "  map\n";
    map = std::make_shared<mbgl::Map>(*frontend, mbgl::VerboseMapObserver::debugObserver(), frontend->getSize(), pixelRatio, *fileSource, *threadPool, mbgl::MapMode::Still);

    if (style_path.find("://") == std::string::npos) {
        style_path = std::string("file://") + style_path;
    }

    map->getStyle().loadURL(style_path);
    map->setLatLngZoom({ lat, lon }, zoom);
    map->setBearing(bearing);
    map->setPitch(pitch);

    if (debug) {
        map->setDebug(debug ? mbgl::MapDebugOptions::TileBorders | mbgl::MapDebugOptions::ParseStatus : mbgl::MapDebugOptions::NoDebug);
    }
}

void shutdown() {
    map.reset();
    frontend.reset();
    fileSource.reset();
    threadPool.reset();
    loop.reset();
}

void update(double zoom, double x0, double y0, uint32_t width, uint32_t height) {
	map->setLatLngZoom({ x0, y0 }, zoom);
    map->setSize({ width, height });
}

void save(const char *filename) {
    try {
        std::cerr << "trying to save\n";
        std::ofstream out(filename, std::ios::binary);
        std::cerr << "opened stream\n";
        mbgl::PremultipliedImage image = frontend->render(*map);
        std::cerr << "rendered\n";
        out << mbgl::encodePNG(image);
        std::cerr << "wrote\n";
        out.close();
    } catch(std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        std::exit(1);
    }
}

void lock_buffer(buffer_t *dst);
void unlock_buffer();