#include "mbgl_wrapper/mbgl_wrapper.h"

#include <QOpenGLContext>
#include <QSurface>

#include <fstream>

#include <boost/algorithm/string/replace.hpp>

#include "frontend.h"
#include "backend.h"

#include "mbgl/util/run_loop.hpp"
#include "mbgl/util/shared_thread_pool.hpp"
#include "mbgl/storage/default_file_source.hpp"
#include "mbgl/map/map_observer.hpp"
#include "mbgl/map/mode.hpp"
#include "mbgl/map/map.hpp"
#include "mbgl/util/logging.hpp"
#include "mbgl/style/style.hpp"

#include "src/mbgl/tile/tile_id.hpp"

#include "file_info.h"


namespace mbgl_wrapper
{

struct wrapper_impl
    : wrapper
{
    explicit wrapper_impl(params_ext_t const &params)
        : params_(params)
    {
        loop_ = new mbgl::util::RunLoop(mbgl::util::RunLoop::Type::New);

        thread_pool_ = mbgl::sharedThreadPool();


        source_ = std::make_shared<mbgl::DefaultFileSource>(params_.cache_path, ".");

        if (!params_.server_url.empty())
            source_->setAPIBaseURL(params_.server_url);
        
        if (!params_.access_token.empty())
            source_->setAccessToken(params_.access_token);

        mbgl::Size const max_size(params_.tile_size * params_.max_tiles, params_.tile_size * params_.max_tiles);
        float pixel_ratio = 1.f;

        backend_ = create_backend({
            static_cast<QSurface *>(params_.surface),
            params_.main_context,
            max_size
        });

        frontend_ = create_frontend({                                               
            backend_,
            source_,                      
            thread_pool_,
            pixel_ratio
        });

        map_ = std::make_shared<mbgl::Map>(
            *frontend_,
            mbgl::MapObserver::nullObserver(),
            mbgl::Size(256, 256),
            pixel_ratio,
            *source_,
            *thread_pool_,
            mbgl::MapMode::Still
        );

        load_style(true);

        map_->setBearing(0); // north
        map_->setPitch(0);   // top-down
        map_->setPrefetchZoomDelta(0); // no prefetch        
    }

    ~wrapper_impl()
    {
        //loop_.reset();
    }

    void load_style(bool force)
    {
        auto new_style_timestamp = file_info::last_modified(params_.style_path);

        bool expired = false;
        if (!style_timestamp_ || new_style_timestamp > *style_timestamp_)
        {
            style_timestamp_ = new_style_timestamp;
            expired = true;
        }

        if (!force && !expired)
            return;

        std::string style_json;
        {
            std::ifstream style_stream(params_.style_path);

            style_json.assign(std::istreambuf_iterator<char>(style_stream), std::istreambuf_iterator<char>());

            boost::replace_all(style_json, "$(ServerURL)", params_.server_url);
        }

        map_->getStyle().loadJSON(style_json);
    }

    void render(request_t const& original_request, callback_pfn callback) override
    {
        auto request = original_request;

        // reload style if needed
        if (request.reload_style != request_t::rs_dont)
            load_style(request.reload_style == request_t::rs_force);
        
        request.width  = std::min(request.width , params_.max_tiles);
        request.height = std::min(request.height, params_.max_tiles);

        uint32_t mbgl_y0 = (1 << request.zoom) - 1 - request.y0;

        mbgl::Size size(request.width * params_.tile_size, request.height * params_.tile_size);
        map_->setSize(size);


        // getting bounds
        mbgl::CanonicalTileID first(request.zoom, request.x0, mbgl_y0);
        mbgl::CanonicalTileID last(request.zoom, request.x0 + request.width - 1, mbgl_y0 - request.height + 1); // minus, because converted coordinate is inverted
        mbgl::LatLngBounds map_bounds(first);
        map_bounds.extend(mbgl::LatLngBounds(last));
        map_->setLatLngBounds(map_bounds);

        // setting position
        map_->jumpTo(map_->cameraForLatLngBounds(map_bounds, mbgl::EdgeInsets()));

        backend_->set_size(size);

        bool rendered = false;
        map_->renderStill([&](std::exception_ptr error) 
        {
            if (error)
                mbgl::Log::Info(mbgl::Event::Render, "Rendering failed");

            rendered = true;
        });

        while (!rendered) 
        {
            auto *current_runloop = mbgl::util::RunLoop::Get();
            //Assert(current_runloop == loop_.get());
            (void)current_runloop;

            current_runloop->runOnce();
        }

        reply_t reply;
        reply.texture_id = backend_->get_texture();
        
        callback(params_.context, reply);
    }

private:
    struct params_int_t
        : params_base_t
    {
        explicit params_int_t(params_ext_t const &ext)
            : params_base_t(ext)
            , server_url  (cstr_to_str(ext.server_url  ))
            , style_path  (cstr_to_str(ext.style_path  ))
            , cache_path  (cstr_to_str(ext.cache_path  ))
            , access_token(cstr_to_str(ext.access_token))
        {}

        static std::string cstr_to_str(char const *cstr)
        {
            if (!cstr)
                return std::string();

            return std::string(cstr);
        }

        std::string server_url  ;
        std::string style_path  ;
        std::string cache_path  ;
        std::string access_token;
    };
    
private:
    params_int_t params_;

    std::shared_ptr<mbgl::ThreadPool> thread_pool_;
    mbgl::util::RunLoop * loop_;
    std::shared_ptr<mbgl::DefaultFileSource> source_;

    backend_ptr backend_;
    frontend_uptr frontend_;

    std::shared_ptr<mbgl::Map> map_;

    std::optional<file_info::timestamp_t> style_timestamp_;
};

MBGL_WRAPPER_API wrapper *create_wrapper(wrapper::params_ext_t const &params)
{
    return new wrapper_impl(params);
}

MBGL_WRAPPER_API void delete_wrapper(wrapper* wrapper)
{
    delete wrapper;
}

} // namespace mbgl_wrapper