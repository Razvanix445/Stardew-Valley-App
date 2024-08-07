#include "ClickableLabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget* parent)
    : QLabel(parent), isHovered(false) {
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAttribute(Qt::WA_Hover);
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::setImage(const QPixmap& pixmap) {
    originalPixmap = pixmap;
    updatePixmap();
}

void ClickableLabel::resizeEvent(QResizeEvent* event) {
    updatePixmap(isHovered ? 0.9 : 0.8);
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

bool ClickableLabel::event(QEvent* event) {
    QWidget* parentContainer = parentWidget();

    if (event->type() == QEvent::HoverEnter) {
        isHovered = true;
        updatePixmap(0.9); // Scale to normal size on hover
        if (parentContainer) {
            parentContainer->setStyleSheet("QWidget { margin-bottom: 30px;}");
        }
        return true;
    }
    else if (event->type() == QEvent::HoverLeave) {
        isHovered = false;
        updatePixmap(0.8);
        if (parentContainer) {
            parentContainer->setStyleSheet("margin-bottom: 30px;"); // Reset to default style
        }
        return true;
    }
    return QLabel::event(event);
}

void ClickableLabel::updatePixmap(double scale) {
    if (!originalPixmap.isNull()) {
        QPixmap scaledPixmap = originalPixmap.scaled(size().width() * scale, size().height() * scale, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(scaledPixmap);
    }
}