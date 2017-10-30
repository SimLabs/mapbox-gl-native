#include "map_widget.h"

#include <QWidget>
#include <qevent.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QOpenGLWidget>
#include <QMapboxGL>
#include <QPropertyAnimation>
#include "mbgl/util/logging.hpp"

#include "qt_mapbox_wrapper/qt_mapbox_wrapper.h"

namespace qt_mapbox_wrapper {

struct map_widget_impl
    : QOpenGLWidget
    , map_widget
{
    map_widget_impl(params_t const &params)
        : m_bearingAnimation(nullptr)
        , m_zoomAnimation(nullptr) 
        , m_styleUrl(params.style_url)
        , m_initialCoord(params.lat, params.lon)
        , m_initialZoom(params.zoom)
   {

        m_settings.setCacheDatabasePath(params.cache_path);
        m_settings.setCacheDatabaseMaximumSize(uint64_t(params.cache_size_mb) * 1024 * 1024);
        m_settings.setApiBaseUrl(params.server_url);
        m_styleUrl = params.style_url;

        mbgl::Log::Debug(mbgl::Event::General, "Initializing window");

        client_ = params.client;

        //        auto layout = new QHBoxLayout(this);
        //        auto label = new QLabel();
        //
        //        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //        sizePolicy.setHorizontalStretch(0);
        //        sizePolicy.setVerticalStretch(0);
        //        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        //        label->setSizePolicy(sizePolicy);
        //
        //        QFont font;
        //        font.setPointSize(36);
        //        label->setFont(font);
        //        label->setAlignment(Qt::AlignCenter);
        //
        //        label->setText("In progress");
        //
        //        layout->addWidget(label);        
        setMouseTracking(true);
    }

    ~map_widget_impl()
    {
        setParent(nullptr);
    }
    
    QWidget* as_qwidget() override
    {
        return this;
    }


    QMapboxGL* get_map() override
    {
        return m_map.data();
    }

public:
    qreal pixelRatio() {
    #if QT_VERSION >= 0x050600
        return devicePixelRatioF();
    #elif QT_VERSION >= 0x050000
        return devicePixelRatio();
    #else
        return 1;
    #endif
    }


void animationFinished()
{
    //qDebug() << "Animation ticks/s: " <<  m_animationTicks / static_cast<float>(kAnimationDuration) * 1000.;
    //qDebug() << "Frame draws/s: " <<  m_frameDraws / static_cast<float>(kAnimationDuration) * 1000.;
    //
    //qApp->quit();
}

void animationValueChanged()
{
    m_animationTicks++;
}

void changeStyle()
{
    static uint8_t currentStyleIndex;

    auto& styles = QMapbox::defaultStyles();

    m_map->setStyleUrl(styles[currentStyleIndex].first);
    setWindowTitle(QString("Mapbox GL: ") + styles[currentStyleIndex].second);

    if (++currentStyleIndex == styles.size()) {
        currentStyleIndex = 0;
    }

    m_sourceAdded = false;

    on_map_changed();
}

//void keyPressEvent(QKeyEvent *ev)
//{
//    switch (ev->key()) {
//    case Qt::Key_S:
//        changeStyle();
//        break;
//    case Qt::Key_L: {
//            if (m_sourceAdded) {
//                return;
//            }
//
//            m_sourceAdded = true;
//
//            // Not in all styles, but will work on streets
//            QString before = "waterway-label";
//
//            QFile geojson(":source1.geojson");
//            geojson.open(QIODevice::ReadOnly);
//
//            // The data source for the route line and markers
//            QVariantMap routeSource;
//            routeSource["type"] = "geojson";
//            routeSource["data"] = geojson.readAll();
//            m_map->addSource("routeSource", routeSource);
//
//            // The route case, painted before the route
//            QVariantMap routeCase;
//            routeCase["id"] = "routeCase";
//            routeCase["type"] = "line";
//            routeCase["source"] = "routeSource";
//            m_map->addLayer(routeCase, before);
//
//            m_map->setPaintProperty("routeCase", "line-color", QColor("white"));
//            m_map->setPaintProperty("routeCase", "line-width", 20.0);
//            m_map->setLayoutProperty("routeCase", "line-join", "round");
//            m_map->setLayoutProperty("routeCase", "line-cap", "round");
//
//            // The route, painted on top of the route case
//            QVariantMap route;
//            route["id"] = "route";
//            route["type"] = "line";
//            route["source"] = "routeSource";
//            m_map->addLayer(route, before);
//
//            m_map->setPaintProperty("route", "line-color", QColor("blue"));
//            m_map->setPaintProperty("route", "line-width", 8.0);
//            m_map->setLayoutProperty("route", "line-join", "round");
//            m_map->setLayoutProperty("route", "line-cap", "round");
//
//            QVariantList lineDashArray;
//            lineDashArray.append(1);
//            lineDashArray.append(2);
//
//            m_map->setPaintProperty("route", "line-dasharray", lineDashArray);
//
//            // Markers at the beginning and end of the route
//            m_map->addImage("label-arrow", QImage(":label-arrow.svg"));
//            m_map->addImage("label-background", QImage(":label-background.svg"));
//
//            QVariantMap markerArrow;
//            markerArrow["id"] = "markerArrow";
//            markerArrow["type"] = "symbol";
//            markerArrow["source"] = "routeSource";
//            m_map->addLayer(markerArrow);
//
//            m_map->setLayoutProperty("markerArrow", "icon-image", "label-arrow");
//            m_map->setLayoutProperty("markerArrow", "icon-size", 0.5);
//            m_map->setLayoutProperty("markerArrow", "icon-ignore-placement", true);
//
//            QVariantList arrowOffset;
//            arrowOffset.append(0.0);
//            arrowOffset.append(-15.0);
//            m_map->setLayoutProperty("markerArrow", "icon-offset", arrowOffset);
//
//            QVariantMap markerBackground;
//            markerBackground["id"] = "markerBackground";
//            markerBackground["type"] = "symbol";
//            markerBackground["source"] = "routeSource";
//            m_map->addLayer(markerBackground);
//
//            m_map->setLayoutProperty("markerBackground", "icon-image", "label-background");
//            m_map->setLayoutProperty("markerBackground", "text-field", "{name}");
//            m_map->setLayoutProperty("markerBackground", "icon-text-fit", "both");
//            m_map->setLayoutProperty("markerBackground", "icon-ignore-placement", true);
//            m_map->setLayoutProperty("markerBackground", "text-ignore-placement", true);
//            m_map->setLayoutProperty("markerBackground", "text-anchor", "left");
//            m_map->setLayoutProperty("markerBackground", "text-size", 16.0);
//            m_map->setLayoutProperty("markerBackground", "text-padding", 0.0);
//            m_map->setLayoutProperty("markerBackground", "text-line-height", 1.0);
//            m_map->setLayoutProperty("markerBackground", "text-max-width", 8.0);
//
//            QVariantList iconTextFitPadding;
//            iconTextFitPadding.append(15.0);
//            iconTextFitPadding.append(10.0);
//            iconTextFitPadding.append(15.0);
//            iconTextFitPadding.append(10.0);
//            m_map->setLayoutProperty("markerBackground", "icon-text-fit-padding", iconTextFitPadding);
//
//            QVariantList backgroundOffset;
//            backgroundOffset.append(-0.5);
//            backgroundOffset.append(-1.5);
//            m_map->setLayoutProperty("markerBackground", "text-offset", backgroundOffset);
//
//            m_map->setPaintProperty("markerBackground", "text-color", QColor("white"));
//
//            QVariantList filterExpression;
//            filterExpression.append("==");
//            filterExpression.append("$type");
//            filterExpression.append("Point");
//
//            QVariantList filter;
//            filter.append(filterExpression);
//
//            m_map->setFilter("markerArrow", filter);
//            m_map->setFilter("markerBackground", filter);
//
//            // Tilt the labels when tilting the map and make them larger
//            m_map->setLayoutProperty("road-label-large", "text-size", 30.0);
//            m_map->setLayoutProperty("road-label-large", "text-pitch-alignment", "viewport");
//
//            m_map->setLayoutProperty("road-label-medium", "text-size", 30.0);
//            m_map->setLayoutProperty("road-label-medium", "text-pitch-alignment", "viewport");
//
//            m_map->setLayoutProperty("road-label-small", "text-pitch-alignment", "viewport");
//            m_map->setLayoutProperty("road-label-small", "text-size", 30.0);
//
//            // Buildings extrusion
//            QVariantMap buildings;
//            buildings["id"] = "3d-buildings";
//            buildings["source"] = "composite";
//            buildings["source-layer"] = "building";
//            buildings["type"] = "fill-extrusion";
//            buildings["minzoom"] = 15.0;
//            m_map->addLayer(buildings);
//
//            QVariantList buildingsFilterExpression;
//            buildingsFilterExpression.append("==");
//            buildingsFilterExpression.append("extrude");
//            buildingsFilterExpression.append("true");
//
//            QVariantList buildingsFilter;
//            buildingsFilter.append(buildingsFilterExpression);
//
//            m_map->setFilter("3d-buildings", buildingsFilterExpression);
//
//            m_map->setPaintProperty("3d-buildings", "fill-extrusion-color", "#aaa");
//            m_map->setPaintProperty("3d-buildings", "fill-extrusion-opacity", .6);
//
//            QVariantMap extrusionHeight;
//            extrusionHeight["type"] = "identity";
//            extrusionHeight["property"] = "height";
//
//            m_map->setPaintProperty("3d-buildings", "fill-extrusion-height", extrusionHeight);
//
//            QVariantMap extrusionBase;
//            extrusionBase["type"] = "identity";
//            extrusionBase["property"] = "min_height";
//
//            m_map->setPaintProperty("3d-buildings", "fill-extrusion-base", extrusionBase);
//        }
//        break;
//    case Qt::Key_1: {
//            if (m_symbolAnnotationId.isNull()) {
//                QMapbox::Coordinate coordinate = m_map->coordinate();
//                QMapbox::SymbolAnnotation symbol { coordinate, "default_marker" };
//                m_map->addAnnotationIcon("default_marker", QImage(":default_marker.svg"));
//                m_symbolAnnotationId = m_map->addAnnotation(QVariant::fromValue<QMapbox::SymbolAnnotation>(symbol));
//            } else {
//                m_map->removeAnnotation(m_symbolAnnotationId.toUInt());
//                m_symbolAnnotationId.clear();
//            }
//        }
//        break;
//    case Qt::Key_2: {
//            if (m_lineAnnotationId.isNull()) {
//                QMapbox::Coordinate topLeft     = m_map->coordinateForPixel({ 0, 0 });
//                QMapbox::Coordinate bottomRight = m_map->coordinateForPixel({ qreal(size().width()), qreal(size().height()) });
//                QMapbox::CoordinatesCollections lineGeometry { { { topLeft, bottomRight } } };
//                QMapbox::ShapeAnnotationGeometry annotationGeometry { QMapbox::ShapeAnnotationGeometry::LineStringType, lineGeometry };
//                QMapbox::LineAnnotation line;
//                line.geometry = annotationGeometry;
//                line.opacity = 0.5f;
//                line.width = 1.0f;
//                line.color = Qt::red;
//                m_lineAnnotationId = m_map->addAnnotation(QVariant::fromValue<QMapbox::LineAnnotation>(line));
//            } else {
//                m_map->removeAnnotation(m_lineAnnotationId.toUInt());
//                m_lineAnnotationId.clear();
//            }
//        }
//        break;
//    case Qt::Key_3: {
//            if (m_fillAnnotationId.isNull()) {
//                QMapbox::Coordinate topLeft     = m_map->coordinateForPixel({ 0, 0 });
//                QMapbox::Coordinate topRight    = m_map->coordinateForPixel({ 0, qreal(size().height()) });
//                QMapbox::Coordinate bottomLeft  = m_map->coordinateForPixel({ qreal(size().width()), 0 });
//                QMapbox::Coordinate bottomRight = m_map->coordinateForPixel({ qreal(size().width()), qreal(size().height()) });
//                QMapbox::CoordinatesCollections fillGeometry { { { bottomLeft, bottomRight, topRight, topLeft, bottomLeft } } };
//                QMapbox::ShapeAnnotationGeometry annotationGeometry { QMapbox::ShapeAnnotationGeometry::PolygonType, fillGeometry };
//                QMapbox::FillAnnotation fill;
//                fill.geometry = annotationGeometry;
//                fill.opacity = 0.5f;
//                fill.color = Qt::green;
//                fill.outlineColor = QVariant::fromValue<QColor>(QColor(Qt::black));
//                m_fillAnnotationId = m_map->addAnnotation(QVariant::fromValue<QMapbox::FillAnnotation>(fill));
//            } else {
//                m_map->removeAnnotation(m_fillAnnotationId.toUInt());
//                m_fillAnnotationId.clear();
//            }
//        }
//        break;
//    case Qt::Key_5: {
//            if (m_map->layerExists("circleLayer")) {
//                m_map->removeLayer("circleLayer");
//                m_map->removeSource("circleSource");
//            } else {
//                QMapbox::CoordinatesCollections point { { { m_map->coordinate() } } };
//                QMapbox::Feature feature { QMapbox::Feature::PointType, point, {}, {} };
//
//                QVariantMap circleSource;
//                circleSource["type"] = "geojson";
//                circleSource["data"] = QVariant::fromValue<QMapbox::Feature>(feature);
//                m_map->addSource("circleSource", circleSource);
//
//                QVariantMap circle;
//                circle["id"] = "circleLayer";
//                circle["type"] = "circle";
//                circle["source"] = "circleSource";
//                m_map->addLayer(circle);
//
//                m_map->setPaintProperty("circleLayer", "circle-radius", 10.0);
//                m_map->setPaintProperty("circleLayer", "circle-color", QColor("black"));
//            }
//        }
//        break;
//    case Qt::Key_Tab:
//        m_map->cycleDebugOptions();
//        break;
//    default:
//        break;
//    }
//
//    ev->accept();
//}

void mousePressEvent(QMouseEvent *ev) override 
{
    m_lastPos = ev->localPos();

    if (ev->type() == QEvent::MouseButtonPress) {
        if (ev->buttons() == (Qt::LeftButton | Qt::RightButton)) {
            changeStyle();
        }
    }

    if (ev->type() == QEvent::MouseButtonDblClick) {
        if (ev->buttons() == Qt::LeftButton) {
            m_map->scaleBy(2.0, m_lastPos);
        } else if (ev->buttons() == Qt::RightButton) {
            m_map->scaleBy(0.5, m_lastPos);
        }
    }

    ev->accept();
    on_map_changed();
}

void mouseMoveEvent(QMouseEvent *ev)
{
    QPointF delta = ev->localPos() - m_lastPos;

    if (!delta.isNull()) {
        if (ev->buttons() == Qt::LeftButton && ev->modifiers() & Qt::ShiftModifier) {
            m_map->setPitch(m_map->pitch() - delta.y());
        } else if (ev->buttons() == Qt::LeftButton) {
            m_map->moveBy(delta);
        } else if (ev->buttons() == Qt::RightButton) {
            m_map->rotateBy(m_lastPos, ev->localPos());
        }
    }

    m_lastPos = ev->localPos();
    ev->accept();
    on_map_changed();
}

void wheelEvent(QWheelEvent *ev)
{
    if (ev->orientation() == Qt::Horizontal) {
        return;
    }

    float factor = ev->delta() / 1200.;
    if (ev->delta() < 0) {
        factor = factor > -1 ? factor : 1 / factor;
    }

    m_map->scaleBy(1 + factor, ev->pos());
    ev->accept();
    on_map_changed();
}

void initializeGL()
{
    m_map.reset(new QMapboxGL(nullptr, m_settings, size(), pixelRatio()));
    connect(m_map.data(), SIGNAL(needsRendering()), this, SLOT(update()));

    // Set default location to Helsinki.
    m_map->setCoordinateZoom(m_initialCoord, m_initialZoom);

    //QString styleUrl = "http://192.168.1.30:8080/styles/klokantech-basic/style.json";

    if (m_styleUrl.isEmpty()) {
        changeStyle();
    } else {
        m_map->setStyleUrl(m_styleUrl);
        setWindowTitle(QString("Mapbox GL: ") + m_styleUrl);
    }

    m_bearingAnimation = new QPropertyAnimation(m_map.data(), "bearing");
    m_zoomAnimation = new QPropertyAnimation(m_map.data(), "zoom");

    connect(m_zoomAnimation, SIGNAL(finished()), this, SLOT(animationFinished()));
    connect(m_zoomAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(animationValueChanged()));

    on_map_changed();
}   

void paintGL()
{
    m_frameDraws++;
    m_map->resize(size(), size() * pixelRatio());
#if QT_VERSION >= 0x050400
    // When we're using QOpenGLWidget, we need to tell Mapbox GL about the framebuffer we're using.
    m_map->setFramebufferObject(defaultFramebufferObject());
#endif
    m_map->render();
}

private:
    void on_map_changed()
    {
        update_mouse_latlon();
    }

    void update_mouse_latlon()
    {
        if (!m_map)
            return;

        auto const latlon = m_map->coordinateForPixel(m_lastPos);

        if (!client_)
            return;

        client_->on_mouse_latlon_changed(latlon.first, latlon.second);
    }


private:
    QPointF m_lastPos;

    QMapboxGLSettings m_settings;
    QString m_styleUrl;
    QMapbox::Coordinate m_initialCoord;
    double m_initialZoom;

    QScopedPointer<QMapboxGL> m_map;

    QPropertyAnimation *m_bearingAnimation;
    QPropertyAnimation *m_zoomAnimation;

    unsigned m_animationTicks = 0;
    unsigned m_frameDraws = 0;

    QVariant m_symbolAnnotationId;
    QVariant m_lineAnnotationId;
    QVariant m_fillAnnotationId;

    bool m_sourceAdded = false;

    wrapper_client *client_ = nullptr;
};

map_widget_uptr create_map_widget(map_widget::params_t const &params)
{
    return std::make_unique<map_widget_impl>(params);
}


} // namespace qt_mapbox_wrapper