#pragma once

#include "../observer/ICheckBoxObserver.h"
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
	CustomCheckBox(QWidget* parent, vector<char>& imageData)
		: QCheckBox(parent), toolTip(new ToolTip(imageData, this)), observers(new vector<ICheckBoxObserver*>()) {
		setMouseTracking(true);
		toolTip->hide();
		connect(this, &QCheckBox::stateChanged, this, &CustomCheckBox::onStateChanged);
	}

	~CustomCheckBox() {
		delete toolTip;
		delete observers;
	}

	void setImages(const QPixmap& checkedImage, const QPixmap& uncheckedImage)
	{
		this->checkedPixmap = checkedImage;
		this->uncheckedPixmap = uncheckedImage;
		update();
	}

	void setToolTipText(const QString& text) {
		toolTip->setText(text);
	}

	void addObserver(ICheckBoxObserver* observer) {
		observers->push_back(observer);
	}

	void removeObserver(ICheckBoxObserver* observer) {
		observers->erase(std::remove(observers->begin(), observers->end(), observer), observers->end());
	}

protected:
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

	void mouseReleaseEvent(QMouseEvent* event) override {
		if (event->button() == Qt::RightButton)
			toolTip->hide();
		QCheckBox::mouseReleaseEvent(event);
	}

private:
	ToolTip* toolTip;

	QPixmap checkedPixmap;
	QPixmap uncheckedPixmap;

	vector<ICheckBoxObserver*>* observers;

	void updateFishDetailsBoxPosition() {
		if (toolTip) {
			QPoint globalCursorPos = QCursor::pos();
			toolTip->move(globalCursorPos.x() + 5, globalCursorPos.y() + 5);
		}
	}

	void notifyObservers(bool checked) {
		for (auto observer : *observers) {
			observer->onCheckBoxStateChanged(checked);
		}
	}

signals:
	void stateChanged(bool checked);

private slots:
	void onStateChanged(int state) {
		emit stateChanged(state == Qt::Checked);
		notifyObservers(state == Qt::Checked);
	}
};