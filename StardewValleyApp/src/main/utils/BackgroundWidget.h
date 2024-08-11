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
    BackgroundWidget(const vector<char>& imageData, QWidget* parent = nullptr)
        : QWidget(parent), cornerRadius(15) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QImage image;
        if (image.loadFromData(reinterpret_cast<const uchar*>(imageData.data()), imageData.size())) {
            backgroundPixmap = QPixmap::fromImage(image);
        } else {
            qWarning() << "Failed to load image from given data!";
        }
    }

    void setCornerRadius(int radius) {
        cornerRadius = radius;
        update(); // Trigger a repaint to apply the new radius
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Create a path for the rounded rectangle
        QPainterPath path;
        path.addRoundedRect(rect(), cornerRadius, cornerRadius);

        // Clip to the rounded rectangle path
        painter.setClipPath(path);

        // Draw the background image
        if (!backgroundPixmap.isNull()) {
            painter.drawPixmap(rect(), backgroundPixmap);
        }
        else {
            qWarning() << "Background pixmap is null!";
        }
    }

private:
    QPixmap backgroundPixmap;
    int cornerRadius;
};