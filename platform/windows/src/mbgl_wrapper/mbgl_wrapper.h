#pragma once

namespace mbgl_wrapper
{
    struct buffer_t;
    typedef void(*buffer_ready_pfn)(void *client_handle, buffer_t const *buffer);

    enum log_severity_t
    {
        ls_debug,
        ls_info,
        ls_warning,
        ls_error,
    };

    typedef void(*log_pfn)(log_severity_t severity, char const *message);

    enum buffer_format_t
        : uint32_t
    {
        bf_rgba,
        bf_png,
    };

#pragma pack(push, 1)
    struct buffer_t
    {
        uint32_t zoom;
        uint32_t x0, y0;
        uint32_t width, height;

        buffer_format_t buffer_format;
        uint32_t buffer_size;

        uint8_t const *ptr;
    };

    struct params_t
    {
        void *client_handle = nullptr;
        uint32_t tile_width, tile_height;
        buffer_ready_pfn buffer_ready_f;

        double timeout_seconds;

        char const *server_url = nullptr;
        char const *style_url = nullptr;

        log_pfn log_f = nullptr;

        buffer_format_t desired_buffer_format = bf_rgba;
    };
#pragma pack(pop)

    struct functions_t
    {
        typedef void(*init_pfn)(params_t const *params);
        typedef void(*shutdown_pfn)();
        typedef void(*update_pfn)(uint32_t zoom, uint32_t x0, uint32_t y0, uint32_t width, uint32_t height, void *request_handle);

        init_pfn          init = nullptr;
        shutdown_pfn      shutdown = nullptr;
        update_pfn        update = nullptr;
    };



} // namespace mbgl_wrapper