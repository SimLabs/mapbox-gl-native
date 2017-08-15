#include "mbgl_wrapper/mbgl_wrapper.h"

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

params_t params;

void init(params_t const * params_)
{
    params = *params_;
    double pixelRatio = 1

    std::cerr << "Creating FileSource with cache file and asset file addresses\n";
    fileSource = std::make_shared<mbgl::DefaultFileSource>("C:\\temp\\mbgl-cache.db", ".");
    fileSource->setAPIBaseURL("http://localhost:8080/");

    std::cerr << "Creating RunLoop\n";
    loop = std::make_shared<mbgl::util::RunLoop>();

    std::cerr << "Creating frontend\n";
    frontend = std::make_shared<mbgl::HeadlessFrontend>(mbgl::Size(params->tile_width, params->tile_height), pixelRatio, *fileSource, *threadPool);
    
    std::cerr << "Creating map\n";
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
    // ignoring width and height
    map->setLatLngZoom({ x0, y0 }, zoom);
    map->setSize({ width, height });
    mbgl::PremultipliedImage image = frontend.render(map);
    if (params.buffer_ready_f) {
        buffer_t buffer;
        buffer.zoom = zoom;
        buffer.x0 = x0;
        buffer.y0 = y0;
        buffer.width = width;
        buffer.height = height;
        buffer.byte_per_pixel = image.channels;
        buffer.buffer_size = image.bytes();
        buffer.ptr = *image.data;

        std::thread runner([buffer]() {
            params.buffer_ready_f(params.client_handle, &buffer);
        });
        runner.detach();
    }
}

} // namespace mbgl_wrapper