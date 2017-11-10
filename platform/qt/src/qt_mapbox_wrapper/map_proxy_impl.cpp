#include "qt_mapbox_wrapper_internal.h"
#include "safe_run.h"

namespace qt_mapbox_wrapper 
{

struct map_proxy_impl
    : map_proxy
{
    explicit map_proxy_impl(map_internal_ptr map_internal)
        : internal_(map_internal) 
    {
    }

// map
    void add_layer(char const* json, char const* before_name) override
    {
        SAFE_RUN_BEGIN()

        return internal_->add_layer(json, before_name);

        SAFE_RUN_END()
    }

    void add_source(char const* name, char const* json) override
    {
        SAFE_RUN_BEGIN()

        return internal_->add_source(name, json);

        SAFE_RUN_END()
    }

    void update_source(char const* name, char const* json) override
    {
        SAFE_RUN_BEGIN()

        return internal_->update_source(name, json);
        
        SAFE_RUN_END()
    }

    void add_image(char const* name, char const* path) override
    {
        SAFE_RUN_BEGIN()

        return internal_->add_image(name, path);

        SAFE_RUN_END()
    }

    bool ready() const override
    {
        SAFE_RUN_BEGIN()

        return internal_->ready();

        SAFE_RUN_END_RET(false)
    }

// map_proxy
    map_internal_ptr get_internal() override
    {
        return internal_;
    }

private:
    map_internal_ptr internal_;
};

QT_MAPBOX_WRAPPER_API map *create_map(map::params_t const &params)
{
    SAFE_RUN_BEGIN()

    map_internal_ptr mi = create_map_internal(params);
    return new map_proxy_impl(mi);
    
    SAFE_RUN_END_RET(nullptr)
}

QT_MAPBOX_WRAPPER_API void release_map(map *p)
{
    SAFE_RUN_BEGIN()

    delete p;

    SAFE_RUN_END()
}


} // namespace qt_mapbox_wrapper