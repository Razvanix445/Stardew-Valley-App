#include "MainWindow.h"
#include "StardewValleyApp.h"

MainWindow::MainWindow(QWidget* parent, const string& databasePath, Service& service, const string& username)
    : QMainWindow(parent), databasePath(databasePath), service(service), username(username)
{
}

void MainWindow::setImageCache(QMap<QString, QPixmap> images) {
    imageCache = images;
}

void MainWindow::setupLayout() {
    pixmap = imageCache.value("stardew_valley_banner");
    BackgroundWidget* centralWidget = new BackgroundWidget(pixmap, this);
    centralWidget->setCornerRadius(0);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout* imageLayout = new QHBoxLayout();
    imageLayout->setAlignment(Qt::AlignCenter);

    QPixmap firstImageContainerImage = imageCache.value("Bream");
    QPixmap secondImageContainerImage = imageCache.value("Dish_O'_The_Sea");
    QPixmap thirdImageContainerImage = imageCache.value("Heart");
    QWidget* firstImageContainer = createClickableLabel(firstImageContainerImage, "Fish");
    QWidget* secondImageContainer = createClickableLabel(secondImageContainerImage, "Cooking");
    QWidget* thirdImageContainer = createClickableLabel(thirdImageContainerImage, "NPCs");

    mainLayout->addStretch();

    imageLayout->addWidget(firstImageContainer);
    imageLayout->addWidget(secondImageContainer);
    imageLayout->addWidget(thirdImageContainer);

    mainLayout->addLayout(imageLayout);
    centralWidget->setLayout(mainLayout);

    fishController = new FishManagementController(nullptr, databasePath, service, username);
    fishController->setImageCache(imageCache);
    fishController->setupLayout();
    fishController->setWindowTitle("Fish Window");

    connect(static_cast<ClickableLabel*>(firstImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onFirstImageClicked);
    connect(static_cast<ClickableLabel*>(secondImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onSecondImageClicked);
    connect(static_cast<ClickableLabel*>(thirdImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onThirdImageClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::onFirstImageClicked() {
    fishController->show();
}

void MainWindow::onSecondImageClicked() {
    StardewValleyApp* stardewValleyApp = new StardewValleyApp(nullptr, databasePath);
    stardewValleyApp->setWindowTitle("Dishes Window");
    stardewValleyApp->show();
}

void MainWindow::onThirdImageClicked() {
    StardewValleyApp* stardewValleyApp = new StardewValleyApp(nullptr, databasePath);
    stardewValleyApp->setWindowTitle("NPCs Window");
    stardewValleyApp->show();
}

QWidget* MainWindow::createClickableLabel(const QPixmap& imageData, const QString& description) {
    QWidget* container = new QWidget(this);
    QVBoxLayout* vLayout = new QVBoxLayout(container);

    ClickableLabel* imageLabel = new ClickableLabel(container);

    // => IMAGE LOADING
    imageLabel->setImage(imageData);
    // <= END

    QLabel* descriptionLabel = new QLabel(description, container);

    // => STYLING
    QFont font;
    font.setPointSize(24);
    font.setBold(true);
    descriptionLabel->setFont(font);
    descriptionLabel->setAlignment(Qt::AlignCenter);
    // <= END

    vLayout->addStretch();
    vLayout->addWidget(imageLabel);
    vLayout->addWidget(descriptionLabel);

    container->setLayout(vLayout);
    container->setStyleSheet("margin-bottom: 30px;");
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return container;
}