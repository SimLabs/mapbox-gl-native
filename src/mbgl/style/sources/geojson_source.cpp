#include <mbgl/style/sources/geojson_source.hpp>
#include <mbgl/style/sources/geojson_source_impl.hpp>
#include <mbgl/style/source_observer.hpp>
#include <mbgl/style/conversion/json.hpp>
#include <mbgl/style/conversion/geojson.hpp>
#include <mbgl/storage/file_source.hpp>
#include <mbgl/util/logging.hpp>

namespace mbgl {
namespace style {

GeoJSONSource::GeoJSONSource(const std::string& id, const GeoJSONOptions& options)
    : Source(makeMutable<Impl>(std::move(id), options)) {
}

GeoJSONSource::~GeoJSONSource() = default;

const GeoJSONSource::Impl& GeoJSONSource::impl() const {
    return static_cast<const Impl&>(*baseImpl);
}

void GeoJSONSource::setURL(const std::string& url_) {
    url = std::move(url_);

    // Signal that the source description needs a reload
    if (loaded || req) {
        loaded = false;
        req.reset();
        observer->onSourceDescriptionChanged(*this);
    }
}

void GeoJSONSource::setGeoJSON(const mapbox::geojson::geojson& geoJSON) {
    if (get_verbose_logging())
    {
        std::stringstream ss;
        ss << "setGeoJson for '" << getID() << "': ";
        
        if (geoJSON.is<mapbox::geojson::feature_collection>())
        {
            auto const &fc = geoJSON.get<mapbox::geojson::feature_collection>();

            ss << "Feature collection with " << fc.size() << " items:\n";
            for (auto const &feature : fc)
            {
                ss << " -- ";

                auto const &geom = feature.geometry;

                if (geom.is<mapbox::geojson::point>())
                {
                    auto const &p = geom.get<mapbox::geojson::point>();
                    ss << "Point: " << p.x << ", " << p.y;
                }
                else
                {
                    ss << "other geometry";
                }
                
                ss << "\n";
            }
        }
        else
        {
            ss << "Other source type";
        }
        
        mbgl::Log::Debug(mbgl::Event::General, ss.str().c_str());
    }

    req.reset();
    baseImpl = makeMutable<Impl>(impl(), geoJSON);
    observer->onSourceChanged(*this);
}

optional<std::string> GeoJSONSource::getURL() const {
    return url;
}

void GeoJSONSource::loadDescription(FileSource& fileSource) {
    if (!url) {
        loaded = true;
        return;
    }

    if (req) {
        return;
    }

    req = fileSource.request(Resource::source(*url), [this](Response res) {
        if (res.error) {
            observer->onSourceError(
                *this, std::make_exception_ptr(std::runtime_error(res.error->message)));
        } else if (res.notModified) {
            return;
        } else if (res.noContent) {
            observer->onSourceError(
                *this, std::make_exception_ptr(std::runtime_error("unexpectedly empty GeoJSON")));
        } else {
            conversion::Error error;
            optional<GeoJSON> geoJSON = conversion::convertJSON<GeoJSON>(*res.data, error);
            if (!geoJSON) {
                Log::Error(Event::ParseStyle, "Failed to parse GeoJSON data: %s",
                           error.message.c_str());
                // Create an empty GeoJSON VT object to make sure we're not infinitely waiting for
                // tiles to load.
                baseImpl = makeMutable<Impl>(impl(), GeoJSON{ FeatureCollection{} });
            } else {
                baseImpl = makeMutable<Impl>(impl(), *geoJSON);
            }

            loaded = true;
            observer->onSourceLoaded(*this);
        }
    });
}

} // namespace style
} // namespace mbgl
