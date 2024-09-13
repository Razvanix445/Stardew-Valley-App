#pragma once

#include "CustomButton.h"
#include "DetailBox.h"

#include <QEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <vector>
#include <string>

using namespace std;

class ComplexHoverButton : public CustomButton {
	Q_OBJECT

public:
    /*
    * @brief - Custom button that shows a detail box when hovered
    * @param imageData - the image data for the button
    * @param text - the text to display on the button
    * @param width - the width of the button
    * @param height - the height of the button
    * @param detailBox - the detail box to show when the button is hovered
    * @param parent - the parent widget
    */
    ComplexHoverButton(const QPixmap& imageData, const QString& text, int width, int height, DetailBox* detailBox, QWidget* parent = nullptr)
		: CustomButton(imageData, text, width, height, parent), isHovered(false), detailBox(detailBox), originalWidth(width), originalHeight(height) {
    	setAttribute(Qt::WA_Hover);
        detailBox->hide();
    }

protected:
    /*
    * @brief - Event handler for the button
    * When the button is hovered, the detail box is shown
    * When the button is not hovered, the detail box is hidden
    * @param event - the event to handle
    * @return true if the event was handled, false otherwise
    */
    bool event(QEvent* event) override
    {
        if (event->type() == QEvent::HoverEnter) {
            isHovered = true;
            update();
            showDetailBox();
            return true;
        }
        else if (event->type() == QEvent::HoverLeave) {
            isHovered = false;
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

    /*
    * Shows the detail box below the button
    */
    void showDetailBox() {
        QPoint position = mapToGlobal(QPoint(0, originalHeight - 70));
        detailBox->move(position);
		detailBox->show();
	}

    /*
    * Checks if the mouse is over the button or the detail box
    * If the mouse is not over the button or the detail box, the detail box is hidden
    */
	void checkMousePosition() {
        bool hoverButtonUnderMouse = this->underMouse();
        bool detailBoxUnderMouse = detailBox->underMouse();

        if (!hoverButtonUnderMouse && !detailBoxUnderMouse) {
            detailBox->hide();
        }
	}
};