#pragma once

class QSurface;
class QOpenGLContext;

namespace mbgl_wrapper
{

struct request_t
{
    enum reload_style_enum
        : uint32_t
    {
        rs_dont,
        rs_if_expired,
        rs_force
    };

    uint32_t x0, y0, width, height;
    uint32_t zoom = 0;
    reload_style_enum reload_style = rs_dont;
};

struct reply_t
{
    bool error = false;
    uint32_t texture_id = 0;
    bool needs_more_work = false;
};


typedef void(*callback_pfn)(void *context, reply_t const &reply);

struct wrapper
{
    struct params_base_t
    {
        QSurface *surface = nullptr;
        QOpenGLContext *main_context = nullptr;

        uint32_t tile_size = 256;
        uint32_t max_tiles = 16;

        void *context = nullptr;

        callback_pfn callback = nullptr;
    };
    
    struct params_ext_t
        : params_base_t
    {
        char const *server_url   = nullptr;
        char const *style_path   = nullptr;
        char const *cache_path   = nullptr;
        char const *access_token = nullptr;
    };

    virtual ~wrapper() {}

    virtual void render(request_t const& request) = 0;
    virtual void update() = 0;
};

#if defined(mbgl_wrapper_EXPORTS)
#   define MBGL_WRAPPER_API __declspec(dllexport) 
#else
#   define MBGL_WRAPPER_API __declspec(dllimport)
#endif


MBGL_WRAPPER_API wrapper *create_wrapper(wrapper::params_ext_t const &params);
MBGL_WRAPPER_API void delete_wrapper(wrapper* wrapper);

} // namespace mbgl_wrapper