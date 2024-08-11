#include "MainWindow.h"
#include "StardewValleyApp.h"
#include "FishManagementController.h"

MainWindow::MainWindow(QWidget* parent, const string& databasePath, Service& service, const string& username)
    : QMainWindow(parent), databasePath(databasePath), service(service), username(username)
{
    
    vector<char> centralWidgetImage = service.getImageByName("stardew_valley_banner");
    BackgroundWidget* centralWidget = new BackgroundWidget(centralWidgetImage, this);
    centralWidget->setCornerRadius(0);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);  // Center the contents vertically

    QHBoxLayout* imageLayout = new QHBoxLayout(); // Layout for image containers
    imageLayout->setAlignment(Qt::AlignCenter);

    vector<char> firstImageContainerImage = service.getImageByName("Bream");
    vector<char> secondImageContainerImage = service.getImageByName("Dish_O'_The_Sea");
    vector<char> thirdImageContainerImage = service.getImageByName("Heart");
    QWidget* firstImageContainer = createClickableLabel(firstImageContainerImage, "Fish");
    QWidget* secondImageContainer = createClickableLabel(secondImageContainerImage, "Cooking");
    QWidget* thirdImageContainer = createClickableLabel(thirdImageContainerImage, "NPCs");

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

QWidget* MainWindow::createClickableLabel(const vector<char>& imageData, const QString& description) {
    QWidget* container = new QWidget(this);
    QVBoxLayout* vLayout = new QVBoxLayout(container);

    // Add a spacer item to push the image down

    ClickableLabel* imageLabel = new ClickableLabel(container);

    QImage image;
    if (image.loadFromData(reinterpret_cast<const uchar*>(imageData.data()), imageData.size())) {
		QPixmap pixmap = QPixmap::fromImage(image);
		imageLabel->setImage(pixmap);
    }
    else {
        qWarning() << "Failed to load image from given data!";
    }

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