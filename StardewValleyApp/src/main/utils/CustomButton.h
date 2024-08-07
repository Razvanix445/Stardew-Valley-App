#pragma once

#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QEvent>

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    CustomButton(const QString& imagePath, const QString& text, int width, int height, QWidget* parent = nullptr)
        : QPushButton(text, parent), imagePath(imagePath), width(width), height(height) {
        setAttribute(Qt::WA_TranslucentBackground, true);
        setFixedSize(width, height);
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        // Load the background image
        QPixmap pixmap(imagePath);

        // Define the desired size for the background image
        QSize imageSize(width, height); // Adjust the size as needed
        QPixmap scaledPixmap = pixmap.scaled(size().width(), size().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        painter.drawPixmap(0, 0, scaledPixmap);

        // Draw the button text
        painter.setPen(palette().buttonText().color());
        painter.drawText(rect(), Qt::AlignCenter, text());
    }

private:
    QString imagePath;
    int width;
    int height;
};