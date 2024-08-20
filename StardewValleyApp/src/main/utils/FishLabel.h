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

	/*
	* @brief - A custom QLabel that displays a fish image and shows a tooltip with fish details when hovered
	* @param imageData - the image data for the fish image
	* @param parent - the parent widget
	*/
	explicit FishLabel(vector<char>& imageData, QWidget* parent = nullptr)
		: QLabel(parent), fishDetailsBox(new FishToolTip(imageData, this)) {
		setMouseTracking(true);
		fishDetailsBox->hide();
	}


	/*
	* @brief - Sets the fish details for the label
	* Sets the details in the tooltip and overlays the fish image with checkmark and favorite images if needed
	* @param fish - the fish object
	* @param checkmarkImage - the image data for the checkmark image
	* @param favoriteImage - the image data for the favorite image
	*/
	void setFishDetails(const Fish& fish, vector<char>& checkmarkImage, vector<char>& favoriteImage) {
		this->fish = fish;

		fishDetailsBox->setFishDetails(fish);
		//qDebug() << "Setting fish details for " + this->fish.toString();

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
		setProperty("fishId", QVariant::fromValue(fish.getId()));
	}

protected:

	/*
	* @brief - Event handler for when the mouse enters the label
	* Shows the fish details box and updates its position
	*/
	void enterEvent(QEnterEvent* event) override {
		QLabel::enterEvent(event);
		if (fishDetailsBox) {
			updateFishDetailsBoxPosition();
			fishDetailsBox->show();
			//qDebug() << "Showing fish details box";
		}
	}


	/*
	* @brief - Event handler for when the mouse moves over the label
	* Updates the position of the fish details box to the cursor
	*/
	void mouseMoveEvent(QMouseEvent* event) override {
		QLabel::mouseMoveEvent(event);
		if (fishDetailsBox && fishDetailsBox->isVisible()) {
			updateFishDetailsBoxPosition();
			//qDebug() << "Moving fish details box";
		}
	}


	/*
	* @brief - Event handler for when the mouse leaves the label
	* Hides the fish details box
	*/
	void leaveEvent(QEvent* event) override {
		QLabel::leaveEvent(event);
		if (fishDetailsBox) {
			fishDetailsBox->hide();
			//qDebug() << "Hiding fish details box";
		}
	}


	/*
	* @brief - Event handler for when the mouse is pressed on the label
	* Emits a signal with the mouse event
	*/
	void mousePressEvent(QMouseEvent* event) override {
		if (event->button() == Qt::LeftButton) {
			emit clicked(event);
		}
	}

private:
	FishToolTip* fishDetailsBox;
	Fish fish;


	/*
	* @brief - Updates the position of the fish details box to the cursor
	* If the fish details box is not null, it moves it to the cursor position
	*/
	void updateFishDetailsBoxPosition() {
		if (fishDetailsBox) {
			QPoint globalCursorPos = QCursor::pos();
            fishDetailsBox->move(globalCursorPos.x() + 10, globalCursorPos.y() + 10);
        }
	}


	/*
	* @brief - Overlays an image on top of another image
	* The overlay image is scaled to the size of the base image and drawn on top of it
	* @param basePixmap - the base image
	* @param overlayPixmap - the image to overlay
	* @param position - the position to overlay the image
	* @return - the resulting image
	*/
	QPixmap overlayPixmap(const QPixmap& basePixmap, const QPixmap& overlayPixmap, const QPoint& position) {
		QPixmap scaledOverlay = overlayPixmap.scaled(basePixmap.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

		QPixmap result(basePixmap.size());
		result.fill(Qt::transparent);

		QPainter painter(&result);
		painter.drawPixmap(0, 0, basePixmap);
		painter.drawPixmap(position, scaledOverlay);

		return result;
	}

signals:
	void clicked(QMouseEvent* event);
};