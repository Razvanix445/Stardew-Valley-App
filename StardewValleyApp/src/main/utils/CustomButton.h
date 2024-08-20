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

    /*
    * A custom button that displays an image as background and text on top of it.
    * @param imageData - the image data to be displayed as background
    * @param text - the text to be displayed on top of the image
    * @param width - the width of the button
    * @param height - the height of the button
    * @param parent - the parent widget
    * @return an instance of CustomButton
    */
    CustomButton(const vector<char> imageData, const QString& text, int width, int height, QWidget* parent = nullptr)
        : QPushButton(text, parent), width(width), height(height) {
        setAttribute(Qt::WA_TranslucentBackground, true);
        setFixedSize(width, height);

        // => LOAD THE IMAGE FROM THE GIVEN DATA
        QImage image;
        if (image.loadFromData(reinterpret_cast<const uchar*>(imageData.data()), imageData.size())) {
			pixmap = QPixmap::fromImage(image);
		} else {
			qWarning() << "Failed to load image from given data!";
		}
        // <= END
    }

protected:

    /*
    * Paints the button with the background image and text.
    */
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // => LOAD THE BACKGROUND IMAGE
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(size().width(), size().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            painter.drawPixmap(0, 0, scaledPixmap);
        } else {
            painter.fillRect(rect(), Qt::gray);
        }
        // <= END


        // => DRAW THE TEXT
        painter.setPen(palette().buttonText().color());
        painter.drawText(rect(), Qt::AlignCenter, text());
        // <= END
    }

private:
    QPixmap pixmap;
    int width;
    int height;
};