#pragma once

#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QEvent>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget* parent = nullptr);
    ~ClickableLabel();

    void setImage(const QPixmap& pixmap);

signals:
    void clicked();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    bool event(QEvent* event) override;

private:
    QPixmap originalPixmap;
    void updatePixmap(double scale = 0.8);

    bool isHovered;
};