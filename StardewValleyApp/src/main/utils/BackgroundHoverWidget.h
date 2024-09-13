#pragma once

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QEvent>
#include <QPainterPath>
#include <QImage>
#include <QSizePolicy>
#include <vector>
#include "BackgroundWidget.h"

using namespace std;

class BackgroundHoverWidget : public BackgroundWidget {
    Q_OBJECT

public:

    /*
    * @brief A widget that displays a background image.
    * @param originalPixmap - the original pixmap to display
    * @param hoveredPixmap - the pixmap to display when the widget is hovered
    * @param parent - the parent widget
    */
    BackgroundHoverWidget(const QPixmap& originalPixmap, const QPixmap& hoveredPixmap, QWidget* parent = nullptr)
        : BackgroundWidget(originalPixmap, parent), cornerRadius(0), hoveredPixmap(hoveredPixmap), originalPixmap(originalPixmap) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAttribute(Qt::WA_Hover);
        currentPixmap = originalPixmap;
    }

    BackgroundHoverWidget(const QPixmap& originalPixmap, QWidget* parent = nullptr)
        : BackgroundWidget(originalPixmap, parent), cornerRadius(0), hoveredPixmap(originalPixmap), originalPixmap(originalPixmap) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAttribute(Qt::WA_Hover);
        currentPixmap = originalPixmap;
    }


    /*
    * Set the background image of the widget.
    * @param pixmap - the pixmap to set
    */
    void setBackgroundImage(const QPixmap& pixmap) {
        currentPixmap = pixmap;
        update();
    }

    /*
    * Set the background image of the widget when the widget is hovered.
    * @param pixmap - the pixmap to set
    */
    void setHoveredStateImage(const QPixmap& pixmap) {
        hoveredPixmap = pixmap;
    }

    /*
    * Set the corner radius of the widget.
    * @param radius - the radius to set
    */
    void setCornerRadius(int radius) {
        cornerRadius = radius;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainterPath path;
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        path.addRoundedRect(rect(), cornerRadius, cornerRadius);
        painter.setClipPath(path);

        // => DRAWING THE BACKGROUND IMAGE
        if (!currentPixmap.isNull()) {
            painter.drawPixmap(rect(), currentPixmap);
        }
        else {
            qWarning() << "Background pixmap is null!";
        }
        // <= END
    }


    bool event(QEvent* e) override {
        if (e->type() == QEvent::Enter) {
            emit hoverEnter();
            setBackgroundImage(hoveredPixmap);
        }
        else if (e->type() == QEvent::Leave) {
            emit hoverLeave();
            setBackgroundImage(originalPixmap);
        }
        return QWidget::event(e);
    }

    void mousePressEvent(QMouseEvent* event) override {
        emit clicked();
    }

private:
    QPixmap originalPixmap;
    QPixmap hoveredPixmap;
    QPixmap currentPixmap;
    int cornerRadius;

signals:
    void hoverEnter();
    void hoverLeave();
    void clicked();
};