#pragma once

#include "../model/Fish.h"
#include "BackgroundWidget.h"
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QVBoxLayout>

class FishToolTip : public BackgroundWidget {
	Q_OBJECT

public:
	/*
	* @brief - A custom widget that displays a tooltip with a background image and text
	* @param imageData - the image data for the background of the tooltip
	* @param parent - the parent widget
	*/
	explicit FishToolTip(vector<char>& imageData, QWidget* parent = nullptr)
		: BackgroundWidget(imageData, parent), imageData(imageData), titleWidget(new QWidget(this)) {
		setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
		setAttribute(Qt::WA_TranslucentBackground, true);
		setAttribute(Qt::WA_StyledBackground, true);
		setAttribute(Qt::WA_ShowWithoutActivating);
		setCornerRadius(0);
		setFixedWidth(300);



		// => TITLE WIDGET
		titleWidget->setFixedSize(290, 80);
		titleWidget->setStyleSheet("background: transparent;");
		QVBoxLayout* titleLayout = new QVBoxLayout(titleWidget);
		titleLayout->setAlignment(Qt::AlignTop);
		titleLabel = new QLabel(titleWidget);
		categoryLabel = new QLabel(titleWidget);
		categoryLabel->setStyleSheet("color: blue;");
		titleLayout->addWidget(titleLabel);
		titleLayout->addWidget(categoryLabel);
		// <= END



		// => SEASON, WEATHER, LOCATION, TIME, DIFFICULTY LABELS
		seasonsLabel = new QLabel(this);
		seasonsLabel->setWordWrap(true);
		seasonsLabel->setContentsMargins(10, 0, 10, 0);
		weatherLabel = new QLabel(this);
		weatherLabel->setWordWrap(true);
		weatherLabel->setContentsMargins(10, 0, 10, 0);
		locationsLabel = new QLabel(this);
		locationsLabel->setWordWrap(true);
		locationsLabel->setContentsMargins(10, 0, 10, 0);
		timeLabel = new QLabel(this);
		timeLabel->setWordWrap(true);
		timeLabel->setContentsMargins(10, 0, 10, 0);
		difficultyLabel = new QLabel(this);
		difficultyLabel->setWordWrap(true);
		difficultyLabel->setContentsMargins(10, 0, 10, 0);
		// <= END



		// => ADDING TO MAIN LAYOUT
		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setAlignment(Qt::AlignTop);

		layout->addWidget(titleWidget);
		layout->addWidget(seasonsLabel);
		layout->addWidget(weatherLabel);
		layout->addWidget(locationsLabel);
		layout->addWidget(timeLabel);
		layout->addWidget(difficultyLabel);
		// <= END
	}


	/*
	* @brief - Sets the title of the tooltip
	* @param title - the title of the tooltip
	* @param category - the category of the tooltip
	*/
	void setTitle(const string& title, const string& category) {
		titleLabel->setText(QString::fromStdString(title));
		titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
		categoryLabel->setText(QString::fromStdString(category));
	}


	/*
	* @brief - Sets the seasons of the tooltip
	* @param seasons - the seasons of the tooltip
	*/
	void setSeasons(const vector<string>& seasons) {
		QString seasonText = "<span style=\"color:red;\">Seasons: </span>";
		for (const auto& season : seasons) {
			seasonText += QString::fromStdString(season) + ", ";
		}
		if (!seasons.empty())
			seasonText.chop(2);
		seasonsLabel->setText(seasonText);
	}


	/*
	* @brief - Sets the weather of the tooltip
	* @param weathers - the weathers of the tooltip
	*/
	void setWeather(const vector<string>& weathers) {
		QString weatherText = "<span style=\"color:blue;\">Weather: </span>";
		for (const auto& weather : weathers) {
			weatherText += QString::fromStdString(weather) + ", ";
		}
		if (!weathers.empty())
			weatherText.chop(2);
		weatherLabel->setText(weatherText);
	}


	/*
	* @brief - Sets the locations of the tooltip
	* @param locations - the locations of the tooltip
	*/
	void setLocations(const vector<string>& locations) {
		QString locationText = "<span style=\"color:green;\">Locations: </span>";
		for (const auto& location : locations) {
			locationText += QString::fromStdString(location) + ", ";
		}
		if (!locations.empty())
			locationText.chop(2);
		locationsLabel->setText(locationText);
	}


	/*
	* @brief - Sets the time of the tooltip
	* @param start - the start time of the tooltip
	* @param end - the end time of the tooltip
	*/
	void setTime(const string& start, const string& end) {
		QString timeText = "<span style=\"color:white;\">Time: </span>";
		if (start == "Any") {
			timeLabel->setText(timeText + "Any");
		}
		else {
			QStringList startSplit = QString::fromStdString(start).split(", ");
			QStringList endSplit = QString::fromStdString(end).split(", ");
			qDebug() << startSplit << endSplit;

			if (startSplit.size() == 2 && endSplit.size() == 2) {
				timeLabel->setText(timeText + startSplit[0] + "-" + endSplit[0] + " and " + startSplit[1] + "-" + endSplit[1]);
			}
			else {
				timeLabel->setText(timeText + QString::fromStdString(start) + "-" + QString::fromStdString(end));
			}

		}
	}


	/*
	* @brief - Sets the difficulty of the tooltip
	* @param difficulty - the difficulty of the tooltip
	* @param movement - the movement of the tooltip
	*/
	void setDifficulty(int difficulty, const string& movement) {
		QString difficultyText = "<span style=\"color:brown;\">Difficulty: </span>";
		difficultyText += QString::fromStdString(to_string(difficulty)) + " " + QString::fromStdString(movement);
		difficultyLabel->setText(difficultyText);
	}


	/*
	* @brief - Sets the fish details of the tooltip
	* Calls the other set functions to set the details of the fish
	* @param fish - the fish to set the details from
	*/
	void setFishDetails(const Fish& fish) {
		setTitle(fish.getName(), fish.getCategory());
		setSeasons(fish.getSeason());
		setWeather(fish.getWeather());
		setLocations(fish.getLocation());
		setTime(fish.getStartCatchingHour(), fish.getEndCatchingHour());
		setDifficulty(fish.getDifficulty(), fish.getMovement());
	}


	/*
	* @brief - Joins a vector of strings into a single string
	* @param vector - the vector of strings to join
	*/
	QString join(const vector<string>& vector)
	{
		QStringList list;
		for (const auto& item : vector) {
			list.append(QString::fromStdString(item));
		}
		return list.join(", ");
	}

private:
	QPixmap backgroundPixmap;
	vector<char> imageData;

	QWidget* titleWidget;
	QLabel* titleLabel;
	QLabel* categoryLabel;

	QLabel* seasonsLabel;
	QLabel* weatherLabel;
	QLabel* locationsLabel;
	QLabel* timeLabel;
	QLabel* difficultyLabel;
};