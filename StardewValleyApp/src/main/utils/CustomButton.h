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
    CustomButton(const QPixmap imageData, const QString& text, int width, int height, QWidget* parent = nullptr)
        : QPushButton(text, parent), width(width), height(height), pixmap(imageData) {
        setAttribute(Qt::WA_TranslucentBackground, true);
        setFixedSize(width, height);
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