#include "qt_mapbox_wrapper_internal.h"
#include <QMapboxGL>
#include "mbgl/util/logging.hpp"
#include "parse_json.h"

namespace qt_mapbox_wrapper 
{

struct map_impl
    : map_internal
{
    explicit map_impl(params_t const &params)
        : style_url_(params.style_url)
        , initial_coord_(params.lat, params.lon)
        , initial_zoom_(params.zoom)
    {
        settings_.setCacheDatabasePath(params.cache_path);
        settings_.setCacheDatabaseMaximumSize(uint64_t(params.cache_size_mb) * 1024 * 1024);
        settings_.setApiBaseUrl(params.server_url);
    }

    void init(uint32_t width, uint32_t height, double pixel_ratio) override
    {
        auto size = QSize(width, height);
        qmapboxgl_ = std::make_unique<QMapboxGL>(nullptr, settings_, size, pixel_ratio);
        qmapboxgl_->setCoordinateZoom(initial_coord_, initial_zoom_);
        qmapboxgl_->setStyleUrl(style_url_);

        qmapboxgl_->resize(size, size * pixel_ratio);
    }

    void add_layer(char const* json, char const* before_name) override
    {
        auto const params = parse_json(json);
        qmapboxgl_->addLayer(params, before_name ? QString(before_name) : QString());
    }

    void add_source(char const* name, char const* json) override
    {
        auto const params = parse_json(json);
        qmapboxgl_->addSource(name, params);
    }

    void update_source(char const* name, char const* json) override
    {
        auto const params = parse_json(json);

        if (mbgl::get_verbose_logging())
            mbgl::Log::Debug(mbgl::Event::General, "Updating source '%s': %s", name, json);

        qmapboxgl_->updateSource(name, params);
    }

    void add_image(char const* name, char const* path) override
    {
        QImage img(path);
        if (img.isNull())
        {
            mbgl::Log::Error(mbgl::Event::General, "Image for '%s' at '%s' is NULL", name, path);
            return;
        }

        qmapboxgl_->addImage(name, img);
    }

    bool ready() const override
    {
        if (!qmapboxgl_)
            return false;

        return qmapboxgl_->isFullyLoaded();
    }

    QMapboxGL* get_qmapboxgl() override
    {
        return qmapboxgl_.get();
    }

private:
    QMapboxGLSettings settings_;
    QString style_url_;
    QMapbox::Coordinate initial_coord_;
    double initial_zoom_;

    std::unique_ptr<QMapboxGL> qmapboxgl_;
};

map_internal_ptr create_map_internal(map_internal::params_t const &params)
{
    return std::make_shared<map_impl>(params);
}

} // namespace qt_mapbox_wrapper