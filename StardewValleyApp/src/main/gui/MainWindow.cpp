#include "MainWindow.h"
#include "StardewValleyApp.h"
#include "FishManagementController.h"

MainWindow::MainWindow(QWidget* parent, const string& databasePath, Service& service, const string& username)
    : QMainWindow(parent), databasePath(databasePath), service(service), username(username)
{
    
    BackgroundWidget* centralWidget = new BackgroundWidget("src/resources/images/stardew_valley_banner.png", this);
    centralWidget->setCornerRadius(0);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);  // Center the contents vertically

    QHBoxLayout* imageLayout = new QHBoxLayout(); // Layout for image containers
    imageLayout->setAlignment(Qt::AlignCenter);

    QWidget* firstImageContainer = createClickableLabel("src/resources/images/Bream.png", "Fish");
    QWidget* secondImageContainer = createClickableLabel("src/resources/images/Dish_O'_The_Sea.png", "Cooking");
    QWidget* thirdImageContainer = createClickableLabel("src/resources/images/Heart.png", "NPCs");

    mainLayout->addStretch();

    imageLayout->addWidget(firstImageContainer);
    imageLayout->addWidget(secondImageContainer);
    imageLayout->addWidget(thirdImageContainer);

    mainLayout->addLayout(imageLayout);
    centralWidget->setLayout(mainLayout);

    connect(static_cast<ClickableLabel*>(firstImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onFirstImageClicked);
    connect(static_cast<ClickableLabel*>(secondImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onSecondImageClicked);
    connect(static_cast<ClickableLabel*>(thirdImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onThirdImageClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::onFirstImageClicked() {
    FishManagementController* fishController = new FishManagementController(nullptr, databasePath, service, username);
    fishController->setWindowTitle("Stardew Valley Fish Mock Database Demonstration");
    fishController->show();
}

void MainWindow::onSecondImageClicked() {
    StardewValleyApp* stardewValleyApp = new StardewValleyApp(nullptr, databasePath);
    stardewValleyApp->setWindowTitle("Stardew Valley Fish Mock Database Demonstration");
    stardewValleyApp->show();
}

void MainWindow::onThirdImageClicked() {
    StardewValleyApp* stardewValleyApp = new StardewValleyApp(nullptr, databasePath);
    stardewValleyApp->setWindowTitle("Stardew Valley Fish Mock Database Demonstration");
    stardewValleyApp->show();
}

QWidget* MainWindow::createClickableLabel(const QString& imagePath, const QString& description) {
    QWidget* container = new QWidget(this);
    QVBoxLayout* vLayout = new QVBoxLayout(container);

    // Add a spacer item to push the image down

    ClickableLabel* imageLabel = new ClickableLabel(container);
    QPixmap pixmap(imagePath);
    imageLabel->setImage(pixmap);
    //imageLabel->setStyleSheet("margin-bottom: 50px;");

    QLabel* descriptionLabel = new QLabel(description, container);

    // Font setup
    QFont font;
    font.setPointSize(24);
    font.setBold(true);
    descriptionLabel->setFont(font);
    descriptionLabel->setAlignment(Qt::AlignCenter);

    // Apply padding with stylesheet
    //descriptionLabel->setStyleSheet("padding-bottom: 100px;");

    vLayout->addStretch();
    vLayout->addWidget(imageLabel);
    vLayout->addWidget(descriptionLabel);

    container->setLayout(vLayout);

    // Apply styles for hover effects if needed
     //container->setStyleSheet("background-color: #dfdfdf;");
    container->setStyleSheet("margin-bottom: 30px;");

    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    return container;
}