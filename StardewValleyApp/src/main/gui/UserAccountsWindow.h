#pragma once

#include <QMainWindow>
#include <QStackedLayout>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <string>
#include "../service/Service.h"
#include "../utils/BackgroundHoverWidget.h"
#include "../utils/BackgroundWidget.h"
#include "CreateUserWindow.h"
#include "MainWindow.h"
#include "ui_UserAccountsWindow.h"

class UserAccountsWindow : public QMainWindow
{
	Q_OBJECT

public:
	UserAccountsWindow(QWidget* parent, const string& databasePath, Service& service);
	~UserAccountsWindow();
	void setImageCache(QMap<QString, QPixmap> images);
	void setupLayout();

private:
	Ui::UserAccountsWindowClass ui;
	MainWindow* window;

	Service& service;
	string databasePath;

	QPixmap pixmap;
	QMap<QString, QPixmap> imageCache;

	BackgroundHoverWidget* createUserAccountPanel(const User& user, const QPixmap& originalpixmap, const QPixmap& hoveredPixmap);
	BackgroundHoverWidget* createEmptyAccountPanel(const QPixmap& originalpixmap, const QPixmap& hoveredPixmap);
	QString progressBarUnfinishedStyleSheet = "QProgressBar {"
		"background-color: #D7A96B; "
		"color: #4C5550;"
		"border-style: none;"
		"border-radius: 10px;"
		"text-align: center;"
		"}"
		"QProgressBar::chunk {"
		"border-radius: 10px;"
		"background-color: qlineargradient(spread: pad, x1: 0, y1: 0.511364, x2: 1, y2: 0.523, stop: 0 #F2D16B, stop: 1 #87CBB9);"
		"}";
	QString progressBarFinishedStyleSheet = "QProgressBar {"
		"background-color: #D7A96B; "
		"font-weight: bold;"
		"color: #4CAF50;"
		"border-style: none;"
		"border-radius: 10px;"
		"text-align: center;"
		"}"
		"QProgressBar::chunk {"
		"border-radius: 10px;"
		"background-color: #4C5550;"
		"}";
	QStackedLayout* achievementsLayout;
	QWidget* progressBarsWidget;
	QWidget* statisticsWidget;
	QVBoxLayout* createProgressBarsLayout(const User& user);
	QVBoxLayout* createStatisticsLayout(const User& user);

	QTimer* switchTimer;
	bool showingProgressBars;

	void switchLayouts();
	void setFontToStatisticsLabels(bool isHovered);

	// => Objects containted in the layouts from above
	QLabel* masterAnglerText;
	QProgressBar* masterAnglerProgress;
	QLabel* gourmetChefText;
	QProgressBar* gourmetChefProgress;
	QLabel* npcsHeartsText;
	QProgressBar* npcsProgress;

	QLabel* fishText;
	QLabel* caughtFishLabel;
	QLabel* favoriteFishLabel;
	QLabel* dishesText;
	QLabel* discoveredRecipesLabel;
	QLabel* cookedDishesLabel;
	QLabel* npcsText;
	QLabel* eightHeartsNpcs;
	QLabel* tenHeartsNpcs;
	// <= End

	void deleteUser(const User& user);

private slots:
	void on_closeButton_clicked();
	void openMainMenu();
	void openCreateAccountWindow();
	void onSwitchTimerTimeout();
};
