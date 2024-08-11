#pragma once

#include "../model/Fish.h"
#include "FishToolTip.h"
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QTimer>

class FishLabel : public QLabel {
	Q_OBJECT

public:
	explicit FishLabel(vector<char>& imageData, QWidget* parent = nullptr)
		: QLabel(parent), fishDetailsBox(new FishToolTip(imageData, this)) {
		setMouseTracking(true);
		fishDetailsBox->hide();
	}

	void setFishDetails(const Fish& fish) {
		this->fish = fish;
		fishDetailsBox->setFishDetails(fish);
		qDebug() << "Setting fish details";
	}

protected:
	void enterEvent(QEnterEvent* event) override {
		QLabel::enterEvent(event);
		if (fishDetailsBox) {
			updateFishDetailsBoxPosition();
			fishDetailsBox->show();
			qDebug() << "Showing fish details box";
		}
	}

	void mouseMoveEvent(QMouseEvent* event) override {
		QLabel::mouseMoveEvent(event);
		if (fishDetailsBox && fishDetailsBox->isVisible()) {
			updateFishDetailsBoxPosition();
			qDebug() << "Moving fish details box";
		}
	}

	void leaveEvent(QEvent* event) override {
		QLabel::leaveEvent(event);
		if (fishDetailsBox) {
			fishDetailsBox->hide();
			qDebug() << "Hiding fish details box";
		}
	}

	void mousePressEvent(QMouseEvent* event) override {
		if (event->button() == Qt::LeftButton) {
			emit clicked(event);
		}
	}

private:
	FishToolTip* fishDetailsBox;
	Fish fish;

	void updateFishDetailsBoxPosition() {
		if (fishDetailsBox) {
			QPoint globalCursorPos = QCursor::pos();
            fishDetailsBox->move(globalCursorPos.x() + 10, globalCursorPos.y() + 10);  // Adjust position to avoid overlap with the cursor
        }
	}

signals:
	void clicked(QMouseEvent* event);
};