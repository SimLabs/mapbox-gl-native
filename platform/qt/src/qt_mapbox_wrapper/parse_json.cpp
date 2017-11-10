#include "parse_json.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

namespace qt_mapbox_wrapper 
{

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

} // namespace qt_mapbox_wrapper