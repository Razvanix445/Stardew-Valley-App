#include "SplashScreen.h"

SplashScreen::SplashScreen(QWidget *parent, const string& databasePath, Service& service)
	: QMainWindow(parent), databasePath(databasePath), service(service)
{
	ui.setupUi(this);

	setupLayout();
	startLoading();
}

void SplashScreen::setupLayout()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);


	// => CREATING TIMER FOR THE PROGRESS BAR
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &SplashScreen::updateProgress);
	timer->start(35);
	// <= END


	// => ALIGNING LABELS
	ui.titleLabel->setAlignment(Qt::AlignCenter);
	ui.descriptionLabel->setAlignment(Qt::AlignCenter);
	ui.creditsLabel->setAlignment(Qt::AlignRight);
	ui.versionLabel->setAlignment(Qt::AlignLeft);
	ui.loadingLabel->setAlignment(Qt::AlignCenter);
	// <= END

	ui.centralWidget->setStyleSheet("font-family: Segoe UI;");
}


void SplashScreen::startLoading()
{
    // Load images
    QFuture<void> imageFuture = QtConcurrent::run([this]() {
        imageCache = service.populateImagesCacheMap();
        QMetaObject::invokeMethod(this, &SplashScreen::imagesLoadingComplete, Qt::QueuedConnection);
        });

    // Load database
    QFuture<void> databaseFuture = QtConcurrent::run([this]() {
        QThread::sleep(1);
        QMetaObject::invokeMethod(this, &SplashScreen::databaseLoadingComplete, Qt::QueuedConnection);
        });

    // Initialize MainWindow
    connect(this, &SplashScreen::databaseLoadingComplete, [this]() {
        QMetaObject::invokeMethod(this, [this]() {
            window = new UserAccountsWindow(nullptr, databasePath, service);
            window->setImageCache(imageCache);
            window->setupLayout();
            QMetaObject::invokeMethod(this, &SplashScreen::uiInitializationComplete, Qt::QueuedConnection);
            }, Qt::QueuedConnection);
        });

    connect(this, &SplashScreen::imagesLoadingComplete, this, &SplashScreen::onImagesLoaded);
    connect(this, &SplashScreen::databaseLoadingComplete, this, &SplashScreen::onDatabaseLoaded);
    connect(this, &SplashScreen::uiInitializationComplete, this, &SplashScreen::onUIInitialized);
}

void SplashScreen::updateProgress()
{
    ui.progressBar->setValue(currentProgress);

    if (currentProgress >= 100 && imagesLoaded && databaseLoaded && uiInitialized) {
        timer->stop();
        if (window) {
            window->showMaximized();
            connect(window, &MainWindow::destroyed, window, &MainWindow::deleteLater);
        }
        close();
    }
}

void SplashScreen::onImagesLoaded()
{
    ui.descriptionLabel->setText("<strong>IMAGES</strong> LOADED");
    currentProgress = 60;
    imagesLoaded = true;
    updateProgress();
}

void SplashScreen::onDatabaseLoaded()
{
    ui.descriptionLabel->setText("<strong>DATABASE</strong> LOADED");
    currentProgress = 80;
    databaseLoaded = true;
    updateProgress();
}

void SplashScreen::onUIInitialized()
{
    ui.descriptionLabel->setText("<strong>USER INTERFACE</strong> INITIALIZED");
    currentProgress = 100;
    uiInitialized = true;
    updateProgress();
}

SplashScreen::~SplashScreen()
{
    delete timer;
}