#pragma once

#include "ToolTip.h"
#include <QCheckBox>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionButton>
#include <QWidget>
#include <QMouseEvent>
#include <QStylePainter>
#include <QEnterEvent>
#include <QMouseEvent>
#include <vector>
#include <string>

using namespace std;

class CustomCheckBox : public QCheckBox
{
	Q_OBJECT

public:
	/*
	* A custom checkbox that displays images instead of the default checkmark
	* @param parent - the parent widget
	* @param imageData - the image data to display in the tooltip
	*/
	CustomCheckBox(QWidget* parent, vector<char>& imageData)
		: QCheckBox(parent), toolTip(new ToolTip(imageData, this)) {
		setMouseTracking(true);
		toolTip->hide();
		connect(this, &QCheckBox::stateChanged, this, &CustomCheckBox::onStateChanged);
	}


	/*
	* Destructor for the CustomCheckBox class
	*/
	~CustomCheckBox() {
		delete toolTip;
	}


	/*
	* Sets the images for the checkbox
	* @param checkedImage - the image to display when the checkbox is checked
	* @param uncheckedImage - the image to display when the checkbox is unchecked
	*/
	void setImages(const QPixmap& checkedImage, const QPixmap& uncheckedImage)
	{
		this->checkedPixmap = checkedImage;
		this->uncheckedPixmap = uncheckedImage;
		update();
	}


	/*
	* @brief - Sets the tooltip text for the checkbox
	* @param text - the text to display in the tooltip
	*/
	void setToolTipText(const QString& text) {
		toolTip->setText(text);
	}

protected:

	/*
	* @brief - Applies an image to the checkbox
	* @param event - the paint event
	*/
	void paintEvent(QPaintEvent* event) override {
		QStylePainter painter(this);
		QStyleOptionButton option;
		initStyleOption(&option);
		
		QPixmap pixmap = isChecked() ? checkedPixmap : uncheckedPixmap;
		QRect iconRect = option.rect;
		iconRect.setSize(QSize(40, 40));
		painter.drawPixmap(iconRect, pixmap);

		QRect textRect = style()->subElementRect(QStyle::SE_CheckBoxContents, &option, this);
		textRect.setLeft(iconRect.right() + 5);
		painter.drawText(textRect, option.text);
	}
	

	/*
	* @brief - Handles the mouse press event
	* If the mouse left button is pressed and the tooltip is not visible, the checkbox is toggled
	* If the mouse right button is pressed, the tooltip is shown
	*/
	void mousePressEvent(QMouseEvent* event) override {
		if (event->button() == Qt::LeftButton && !toolTip->isVisible()) {
			setChecked(!isChecked());
			update();
		}
		else if (event->button() == Qt::RightButton) {
			if (toolTip->isVisible())
				toolTip->hide();
			else {
				updateFishDetailsBoxPosition();
				toolTip->show();
			}
		}
		QCheckBox::mousePressEvent(event);
	}


	/*
	* @brief - Handles the mouse move event
	* If the mouse right button is released, the tooltip is hidden
	*/
	void mouseReleaseEvent(QMouseEvent* event) override {
		if (event->button() == Qt::RightButton)
			toolTip->hide();
		QCheckBox::mouseReleaseEvent(event);
	}

private:
	ToolTip* toolTip;

	QPixmap checkedPixmap;
	QPixmap uncheckedPixmap;


	/*
	* @brief - Updates the position of the tooltip based on the cursor position
	*/
	void updateFishDetailsBoxPosition() {
		if (toolTip) {
			QPoint globalCursorPos = QCursor::pos();
			toolTip->move(globalCursorPos.x() + 5, globalCursorPos.y() + 5);
		}
	}
	

signals:
	void stateChanged(bool checked);

private slots:
	void onStateChanged(int state) {
		emit stateChanged(state == Qt::Checked);
	}
};