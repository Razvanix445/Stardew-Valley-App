#pragma once

#include "../model/Fish.h"
#include "../service/Service.h"
#include "../utils/BackgroundWidget.h"
#include <QMainWindow>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPushButton>
#include <string>
#include <vector>
#include "ui_FishDetailsWindow.h"

using namespace std;

class FishDetailsWindow : public BackgroundWidget
{
	Q_OBJECT

public:
	explicit FishDetailsWindow(QWidget* parent, Service& service, const Fish fish, const vector<char>& backgroundImage);
	~FishDetailsWindow() override;

private:

	Service& service;
	const Fish fish;
	Ui::FishDetailsWindowClass ui;

	bool isDragging;
	QPoint dragStartPosition;
	QWidget* dragHandle;

	QLabel* imageLabel;
	QLabel* nameLabel;
	QLabel* categoryLabel;
	QLabel* descriptionLabel;

	QLabel* seasonsLabel;
	QLabel* weatherLabel;
	QLabel* locationsLabel;
	QLabel* timeLabel;
	QLabel* difficultyLabel;

	void setupLayout();

	void setSeasons(const vector<string>& seasons);
	void setWeather(const vector<string>& weather);
	void setLocations(const vector<string>& locations);
	void setTime(const string& start, const string& end);
	void setDifficulty(int difficulty, const string& movement);

private slots:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void on_closeButton_clicked();
};
