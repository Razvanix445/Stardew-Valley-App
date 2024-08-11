#pragma once

#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <vector>
#include <string>

using namespace std;

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    CustomButton(const vector<char> imageData, const QString& text, int width, int height, QWidget* parent = nullptr)
        : QPushButton(text, parent), width(width), height(height) {
        setAttribute(Qt::WA_TranslucentBackground, true);
        setFixedSize(width, height);

        QImage image;
        if (image.loadFromData(reinterpret_cast<const uchar*>(imageData.data()), imageData.size())) {
			pixmap = QPixmap::fromImage(image);
		} else {
			qWarning() << "Failed to load image from given data!";
		}
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // Load the background image
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(size().width(), size().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(0, 0, scaledPixmap);
        } else {
            painter.fillRect(rect(), Qt::gray);
        }

        // Draw the button text
        painter.setPen(palette().buttonText().color());
        painter.drawText(rect(), Qt::AlignCenter, text());
    }

private:
    QPixmap pixmap;
    int width;
    int height;
};