#pragma once

#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QEvent>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    /*
    * A clickable label that emits a signal when clicked.
    */
    explicit ClickableLabel(QWidget* parent = nullptr);
    ~ClickableLabel();

    /*
    * Sets the image of the label.
    * @param pixmap - the image to set
    */
    void setImage(const QPixmap& pixmap);

signals:
    void clicked();

protected:

    /*
    * Resizes the image when the label is resized.
    */
    void resizeEvent(QResizeEvent* event) override;

    /*
    * Emits a signal when the label is clicked.
    */
    void mousePressEvent(QMouseEvent* event) override;

    /*
    * If the label is hovered, the image is scaled to 0.9, otherwise it is scaled to 0.8.
    */
    bool event(QEvent* event) override;

private:
    QPixmap originalPixmap;

    /*
    * Updates the pixmap of the label.
    * @param scale - the scale to apply to the image
    */
    void updatePixmap(double scale = 0.8);

    bool isHovered;
};