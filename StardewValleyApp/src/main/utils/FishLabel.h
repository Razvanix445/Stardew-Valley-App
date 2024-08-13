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

	void setFishDetails(const Fish& fish, vector<char>& checkmarkImage, vector<char>& favoriteImage) {
		this->fish = fish;
		fishDetailsBox->setFishDetails(fish);
		qDebug() << "Setting fish details for " + fish.getName() + " " + to_string(fish.getIsCaught());

		QPixmap fishPixmap;
		fishPixmap.loadFromData(reinterpret_cast<const uchar*>(fish.getImage().data()), fish.getImage().size());
		QPixmap finalPixmap = fishPixmap;

		if (fish.getIsCaught()) {
			QPixmap caughtPixmap;
			caughtPixmap.loadFromData(reinterpret_cast<const uchar*>(checkmarkImage.data()), checkmarkImage.size());
			finalPixmap = overlayPixmap(finalPixmap, caughtPixmap, QPoint(0, 0));
		}

		if (fish.getIsFavorite()) {
			QPixmap favoritePixmap;
			favoritePixmap.loadFromData(reinterpret_cast<const uchar*>(favoriteImage.data()), favoriteImage.size());
			finalPixmap = overlayPixmap(finalPixmap, favoritePixmap, QPoint(0, 0));
		}

		setPixmap(finalPixmap);
		setScaledContents(true);
		setFixedSize(60, 60);
		setContentsMargins(5, 5, 5, 5);
		//setStyleSheet("background: transparent;");
		setProperty("fishId", QVariant::fromValue(fish.getId()));
	}

protected:
	void enterEvent(QEnterEvent* event) override {
		QLabel::enterEvent(event);
		if (fishDetailsBox) {
			updateFishDetailsBoxPosition();
			fishDetailsBox->show();
			//qDebug() << "Showing fish details box";
		}
	}

	void mouseMoveEvent(QMouseEvent* event) override {
		QLabel::mouseMoveEvent(event);
		if (fishDetailsBox && fishDetailsBox->isVisible()) {
			updateFishDetailsBoxPosition();
			//qDebug() << "Moving fish details box";
		}
	}

	void leaveEvent(QEvent* event) override {
		QLabel::leaveEvent(event);
		if (fishDetailsBox) {
			fishDetailsBox->hide();
			//qDebug() << "Hiding fish details box";
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
            fishDetailsBox->move(globalCursorPos.x() + 10, globalCursorPos.y() + 10);
        }
	}

	QPixmap overlayPixmap(const QPixmap& basePixmap, const QPixmap& overlayPixmap, const QPoint& position) {
		// Scale the overlay to the full size of the base image
		QPixmap scaledOverlay = overlayPixmap.scaled(basePixmap.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

		// Create a new QPixmap with the size of the base image
		QPixmap result(basePixmap.size());
		result.fill(Qt::transparent);

		// Use QPainter to draw the base image and the scaled overlay
		QPainter painter(&result);
		painter.drawPixmap(0, 0, basePixmap);
		painter.drawPixmap(position, scaledOverlay);

		return result;
	}

signals:
	void clicked(QMouseEvent* event);
};