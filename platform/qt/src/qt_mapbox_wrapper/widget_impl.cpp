#include <QWidget>
#include <qevent.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QOpenGLWidget>
#include <QMapboxGL>
#include <QPropertyAnimation>
#include "mbgl/util/logging.hpp"

#include "qt_mapbox_wrapper_internal.h"

namespace qt_mapbox_wrapper {

struct widget_impl
    : QOpenGLWidget
    , widget
{
    explicit widget_impl(params_t const &params, map_internal_ptr map_internal)
        : m_map_internal_(map_internal)
        , m_bearingAnimation(nullptr)
        , m_zoomAnimation(nullptr) 
    {
        mbgl::Log::Debug(mbgl::Event::General, "Initializing window");

        client_ = params.client;
        setMouseTracking(true);
    }

    ~widget_impl()
    {
        setParent(nullptr);
    }
    
    QWidget* get_qwidget() override
    {
        return this;
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
    }

    void animationValueChanged()
    {
        m_animationTicks++;
    }

    void mousePressEvent(QMouseEvent *ev) override 
    {
        m_lastPos = ev->localPos();

        if (ev->type() == QEvent::MouseButtonDblClick) {
            if (ev->buttons() == Qt::LeftButton) {
                m_map()->scaleBy(2.0, m_lastPos);
            } else if (ev->buttons() == Qt::RightButton) {
                m_map()->scaleBy(0.5, m_lastPos);
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
                m_map()->setPitch(m_map()->pitch() - delta.y());
            } else if (ev->buttons() == Qt::LeftButton) {
                m_map()->moveBy(delta);
            } else if (ev->buttons() == Qt::RightButton) {
                m_map()->rotateBy(m_lastPos, ev->localPos());
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

        m_map()->scaleBy(1 + factor, ev->pos());
        ev->accept();
        on_map_changed();
    }

    void initializeGL()
    {
        auto const sz = size();
        
        m_map_internal_->init(sz.width(), sz.height(), pixelRatio());
        connect(m_map(), SIGNAL(needsRendering()), this, SLOT(update()));

        m_bearingAnimation = new QPropertyAnimation(m_map(), "bearing");
        m_zoomAnimation = new QPropertyAnimation(m_map(), "zoom");

        connect(m_zoomAnimation, SIGNAL(finished()), this, SLOT(animationFinished()));
        connect(m_zoomAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(animationValueChanged()));

        on_map_changed();
    }   

    void paintGL()
    {
        m_frameDraws++;
        m_map()->resize(size(), size() * pixelRatio());
    #if QT_VERSION >= 0x050400
        // When we're using QOpenGLWidget, we need to tell Mapbox GL about the framebuffer we're using.
        m_map()->setFramebufferObject(defaultFramebufferObject());
    #endif
        m_map()->render();
    }

private:
    void on_map_changed()
    {
        update_mouse_latlon();
    }

    void update_mouse_latlon()
    {
        if (!m_map())
            return;

        auto const latlon = m_map()->coordinateForPixel(m_lastPos);

        if (!client_)
            return;

        client_->on_mouse_latlon_changed(latlon.first, latlon.second);
    }

private:

    QMapboxGL *m_map() const
    {
        return m_map_internal_->get_qmapboxgl();
    }

private:
    QPointF m_lastPos;

    map_internal_ptr m_map_internal_;

    QPropertyAnimation *m_bearingAnimation;
    QPropertyAnimation *m_zoomAnimation;

    unsigned m_animationTicks = 0;
    unsigned m_frameDraws = 0;

    QVariant m_symbolAnnotationId;
    QVariant m_lineAnnotationId;
    QVariant m_fillAnnotationId;

    bool m_sourceAdded = false;

    widget_client *client_ = nullptr;
};


QT_MAPBOX_WRAPPER_API widget *create_widget(widget::params_t const &params)
{
    return new widget_impl(params, dynamic_cast<map_proxy *>(params.m)->get_internal());
}

QT_MAPBOX_WRAPPER_API void release_widget(widget *p)
{
    delete p;
}


} // namespace qt_mapbox_wrapper