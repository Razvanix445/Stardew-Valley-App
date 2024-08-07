#pragma once

#include "CustomButton.h"
#include "DetailBox.h"

#include <QEvent>
#include <QTimer>
#include <QVBoxLayout>

class HoverButton : public CustomButton {
	Q_OBJECT

public:
	HoverButton(const QString& imagePath, const QString& text, int width, int height, DetailBox* detailBox, QWidget* parent = nullptr)
		: CustomButton(imagePath, text, width, height, parent), isHovered(false), detailBox(detailBox), originalWidth(width), originalHeight(height) {
    	setAttribute(Qt::WA_Hover);
        detailBox->hide();
    }

protected:
    bool event(QEvent* event) override
    {
        if (event->type() == QEvent::HoverEnter) {
            //qDebug() << "Hover Enter";
            isHovered = true;
            //setFixedSize(originalWidth + 5, originalHeight);
            update();
            showDetailBox();
            return true;
        }
        else if (event->type() == QEvent::HoverLeave) {
            //qDebug() << "Hover Leave";
            isHovered = false;
            //setFixedSize(originalWidth, originalHeight);
            update();
            QTimer::singleShot(0, this, [this]() {
                checkMousePosition();
            });
            return true;
        }
        return CustomButton::event(event);
    }

private:
    bool isHovered;
    DetailBox* detailBox;
    int originalWidth;
    int originalHeight;

    void showDetailBox() {
        QPoint position = mapToGlobal(QPoint(0, originalHeight - 70));
        detailBox->move(position);
		detailBox->show();
	}

	void checkMousePosition() {
        bool hoverButtonUnderMouse = this->underMouse();
        bool detailBoxUnderMouse = detailBox->underMouse();
        qDebug() << "Check Mouse Position: HoverButton underMouse:" << hoverButtonUnderMouse << ", DetailBox underMouse:" << detailBoxUnderMouse;

        if (!hoverButtonUnderMouse && !detailBoxUnderMouse) {
            detailBox->hide();
            qDebug() << "DetailBox hidden";
        }
	}
};