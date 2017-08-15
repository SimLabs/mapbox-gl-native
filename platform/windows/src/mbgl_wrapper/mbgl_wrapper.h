#pragma once

namespace mbgl_wrapper
{
    struct buffer_t
    {
	    uint32_t zoom;
        uint32_t x0, y0;
	    uint32_t width, height;

        uint32_t bytes_per_pixel;
        uint32_t buffer_size;

        uint8_t const *ptr;
    };

    typedef void (*buffer_ready_pfn)(void *client_handle, buffer_t const *buffer);

    struct params_t
    {
	    void *client_handle = nullptr;
        uint32_t tile_width, tile_height;
        buffer_ready_pfn buffer_ready_f;
    };

    struct functions_t
    {
        typedef void (*init_pfn)(params_t const *params);    
        typedef void (*shutdown_pfn)();    
        typedef void (*update_pfn)(uint32_t zoom, uint32_t x0, uint32_t y0, uint32_t width, uint32_t height, void *request_handle);

        init_pfn          init          = nullptr;
        shutdown_pfn      shutdown      = nullptr;
        update_pfn        update        = nullptr;
    };



} // namespace mbgl_wrapper