#include "qt_mapbox_wrapper/qt_mapbox_wrapper.h"

#include "map_widget.h"

#include <QWidget>
#include <qevent.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMapboxGL>
#include "mbgl/util/event.hpp"
#include "mbgl/util/logging.hpp"

namespace mbgl
{
    void set_log_pfn(qt_mapbox_wrapper::log_pfn pfn);
    void set_verbose_logging(bool value);
}


namespace qt_mapbox_wrapper
{

    namespace
    {
        struct parse_json_error
            : std::runtime_error
        {
            explicit parse_json_error(std::string const &str)
                : std::runtime_error(str)
            {
                
            }
        };

        QVariantMap parse_json(char const *json_cstr)
        {
            QByteArray const ba(json_cstr);
            
            QJsonParseError err;            
            auto const doc = QJsonDocument::fromJson(ba, &err);

            if (err.error != QJsonParseError::NoError)
                throw parse_json_error(err.errorString().toStdString());

            auto const obj = doc.object();
            auto const params = obj.toVariantMap();

            for (auto it = params.begin(); it != params.end(); ++it)
            {
                auto const &k = it.key().toStdString();
                auto const &v = it.value();
                int aaa = 5;
            }


            return params;
        }
    } // namespace

struct wrapper_impl
    : wrapper
{
    explicit wrapper_impl(params_t const& params)
        : params_(params)
        , widget_(create_map_widget(params))
    {
    }

    ~wrapper_impl()
    {
    }

    QWidget* get_widget() override
    {
        return widget_->as_qwidget();
    }


    void add_layer(char const *json, char const *before_name) override
    {
        safe_run("add_layer", [&]()
        {
            auto *m = widget_->get_map();
            auto const params = parse_json(json);

            m->addLayer(params, before_name ? QString(before_name) : QString());
        });
    }

    void add_source(char const* name, char const* json) override
    {
        safe_run("add_source", [&]()
        {
            auto *m = widget_->get_map();
            auto const params = parse_json(json);

            m->addSource(name, params);
        });
    }

    void update_source(char const* name, char const* json) override
    {
        safe_run("update_source", [&]()
        {
            auto *m = widget_->get_map();
            auto const params = parse_json(json);

            if (mbgl::get_verbose_logging())
                mbgl::Log::Debug(mbgl::Event::General, "Updating source '%s': %s", name, json);

            m->updateSource(name, params);
        });
    }


    void add_image(char const* name, char const* path) override
    {
        safe_run("add_image", [&]()
        {
            auto *m = widget_->get_map();

            QImage img(path);
            if (img.isNull())
            {
                mbgl::Log::Error(mbgl::Event::General, "Image for '%s' at '%s' is NULL", name, path);
                return;
            }

            m->addImage(name, img);
        });
    }

    bool ready() const override
    {
        auto map = widget_->get_map();
        if (!map)
            return false;

        if (!map->isFullyLoaded())
            return false;

        return true;
    }

private:
    void safe_run(char const *fn_name, std::function<void()> const &f)
    {
        try
        {
            f();
        } 
        catch (std::exception const &e)
        {
            mbgl::Log::Error(mbgl::Event::General, "%s failed: %s", fn_name, e.what());
        }
    }


private:
    params_t params_;
    map_widget_uptr widget_;
};


QT_MAPBOX_WRAPPER_API wrapper *create_wrapper(wrapper::params_t const &params)
{
    return new wrapper_impl(params);    
}

QT_MAPBOX_WRAPPER_API void delete_wrapper(wrapper *p)
{
    delete p;
}

QT_MAPBOX_WRAPPER_API void set_log_pfn(log_pfn pfn) 
{
    mbgl::set_log_pfn(pfn);
    QMapbox::SetLogPfn(pfn);
}

QT_MAPBOX_WRAPPER_API void set_verbose_logging(bool value)
{
    mbgl::set_verbose_logging(value);
    QMapbox::SetVerboseLogging(value);
}



} // namespace qt_mapbox_wrapper