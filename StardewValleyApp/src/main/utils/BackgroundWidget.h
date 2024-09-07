#pragma once

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPainterPath>
#include <QImage>
#include <QSizePolicy>
#include <vector>

using namespace std;

class BackgroundWidget : public QWidget {
    Q_OBJECT

public:

    /*
    * @brief A widget that displays a background image.
    * @param imageData - the raw image data to display
    * @param parent - the parent widget
    */
    BackgroundWidget(const QPixmap& pixmap, QWidget* parent = nullptr)
        : QWidget(parent), cornerRadius(0), backgroundPixmap(pixmap) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
        if (!backgroundPixmap.isNull()) {
            painter.drawPixmap(rect(), backgroundPixmap);
        }
        else {
            qWarning() << "Background pixmap is null!";
        }
        // <= END
    }

private:
    QPixmap backgroundPixmap;
    int cornerRadius;
};