#pragma once

#include <QMainWindow>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QEasingCurve>
#include <QTimer>
#include <QThread>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <string>
#include "../service/Service.h"
#include "UserAccountsWindow.h"
#include "MainWindow.h"
#include "ui_SplashScreen.h"

class SplashScreen : public QMainWindow
{
	Q_OBJECT

public:
	SplashScreen(QWidget* parent, const string& databasePath, Service& service);
	~SplashScreen() override;

private:
	Ui::SplashScreenClass ui;

	UserAccountsWindow* window;
	Service& service;
	string databasePath;

	QFutureWatcher<void> imageWatcher;
	QFutureWatcher<void> uiWatcher;
	QFutureWatcher<void> databaseWatcher;
	bool imagesLoaded = false;
	bool databaseLoaded = false;
	bool uiInitialized = false;

	QMap<QString, QPixmap> imageCache;
	long currentProgress = 0;
	QTimer* timer;

	void setupLayout();
	void preloadImages();
	void startLoading();

signals:
	void imagesLoadingComplete();
	void databaseLoadingComplete();
	void uiInitializationComplete();

private slots:
	void updateProgress();
	void onImagesLoaded();
	void onDatabaseLoaded();
	void onUIInitialized();
};
