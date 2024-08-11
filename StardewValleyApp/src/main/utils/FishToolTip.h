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
	explicit FishToolTip(vector<char>& imageData, QWidget* parent = nullptr)
		: BackgroundWidget(imageData, parent), titleWidget(new QWidget(this)) {
		setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
		setAttribute(Qt::WA_TranslucentBackground, true);
		setAttribute(Qt::WA_StyledBackground, true);
		setAttribute(Qt::WA_ShowWithoutActivating);
		setCornerRadius(0);
		setFixedSize(200, 300);

		// => TITLE WIDGET
		titleWidget->setFixedSize(200, 100);
		titleWidget->setStyleSheet("background: transparent;");
		QVBoxLayout* titleLayout = new QVBoxLayout(titleWidget);
		titleLayout->setAlignment(Qt::AlignTop);
		titleLabel = new QLabel(titleWidget);
		categoryLabel = new QLabel(titleWidget);
		categoryLabel->setStyleSheet("color: blue;");
		titleLayout->addWidget(titleLabel);
		titleLayout->addWidget(categoryLabel);
		// <= END




		descriptionLabel = new QLabel(this);




		// => MAIN LAYOUT
		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setAlignment(Qt::AlignTop);

		layout->addWidget(titleWidget);
		layout->addWidget(descriptionLabel);
		// <= END
	}

	void setTitle(const string& title, const string& category) {
		titleLabel->setText(QString::fromStdString(title));
		titleLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
		categoryLabel->setText(QString::fromStdString(category));
	}

	void setFishDetails(const Fish& fish) {
		QString fishDetails = QString::fromStdString(fish.getDescription()) + "\n" +
			"Seasons: " + join(fish.getSeason()) + "\n" +
			"Weather: " + join(fish.getWeather()) + "\n" +
			"Locations: " + join(fish.getLocation()) + "\n" +
			"Time: " + QString::fromStdString(fish.getStartCatchingHour()) + "-" + QString::fromStdString(fish.getEndCatchingHour()) + "\n" +
			"Difficulty: " + QString::fromStdString(to_string(fish.getDifficulty())) + " " + QString::fromStdString(fish.getMovement()) + "\n";
		setTitle(fish.getName(), fish.getCategory());
		descriptionLabel->setText(fishDetails);
	}



	QString join(const std::vector<std::string>& vec)
	{
		QStringList list;
		for (const auto& item : vec) {
			list.append(QString::fromStdString(item));
		}
		return list.join(", ");
	}

private:
	QWidget* titleWidget;
	QLabel* titleLabel;
	QLabel* categoryLabel;

	QLabel* descriptionLabel;
};