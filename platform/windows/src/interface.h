#ifndef MBGL_INTERFACE_H
#define MBGL_INTERFACE_H

#include "mapbox-interface_export.h"
#include <stdint.h>

struct camera_params_t
{
    uint32_t width, height;
    double latitude, longitude, zoom, bearing, pitch, pixel_ratio;
};

struct map_params_t {
	char *style_path;
	char *api_base_url;
	char *cache_file;
	char *asset_root;
	char *token;
	bool debug;
};

struct buffer_t
{
    uint8_t *ptr;
    uint32_t width, height;
};

void MAPBOX_INTERFACE_EXPORT init(camera_params_t const *camera_params, map_params_t const *map_params);
void MAPBOX_INTERFACE_EXPORT shutdown();

void MAPBOX_INTERFACE_EXPORT save(const char *filename);
void MAPBOX_INTERFACE_EXPORT update(double zoom, double x0, double y0, uint32_t width, uint32_t height);
void MAPBOX_INTERFACE_EXPORT lock_buffer(buffer_t *dst);
void MAPBOX_INTERFACE_EXPORT unlock_buffer();

#endif //MBGL_INTERFACE_H
