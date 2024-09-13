#include "MainWindow.h"
#include "StardewValleyApp.h"

MainWindow::MainWindow(QWidget* parent, const string& databasePath, Service& service, const long userId)
    : QMainWindow(parent), databasePath(databasePath), service(service), userId(userId)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, true);
    setFixedSize(600, 400);
}

void MainWindow::setImageCache(QMap<QString, QPixmap> images) {
    imageCache = images;
}

void MainWindow::setupLayout() {
    pixmap = imageCache.value("Page");
    BackgroundWidget* centralWidget = new BackgroundWidget(pixmap, this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignCenter);



    // = > TOP LAYOUT
    topLayout = new QHBoxLayout();

    const QSize originalButtonSize(40, 40);
    const QSize hoveredButtonSize(50, 50);

    // Minimize Button
    HoverButton* minimizeButton = new HoverButton(this, originalButtonSize, hoveredButtonSize);
    QPixmap minimizeButtonImage = imageCache.value("Minimize_Panel");
    minimizeButton->setStyleSheet("background: transparent; border: none;");
    minimizeButton->setIcon(minimizeButtonImage);
    connect(minimizeButton, &HoverButton::clicked, this, &MainWindow::showMinimized);
    // End

    // Return Button
    HoverButton* returnButton = new HoverButton(this, originalButtonSize, hoveredButtonSize);
    QPixmap returnButtonImage = imageCache.value("Return_Panel");
    returnButton->setStyleSheet("background: transparent; border: none;");
    returnButton->setIcon(returnButtonImage);
    connect(returnButton, &HoverButton::clicked, this, &MainWindow::onReturnButtonClicked);
    // End
    
    // Close Button
    HoverButton* closeButton = new HoverButton(this, originalButtonSize, hoveredButtonSize);
    QPixmap closeButtonImage = imageCache.value("Close_Panel");
    closeButton->setStyleSheet("background: transparent; border: none;");
    closeButton->setIcon(closeButtonImage);
    connect(closeButton, &HoverButton::clicked, this, &MainWindow::close);
    // End

    topLayout->addWidget(minimizeButton);
    topLayout->addWidget(returnButton);
    topLayout->addWidget(closeButton);
    topLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);
    // <= END



    // = > TITLE LAYOUT
    QHBoxLayout* titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignLeft);
    titleLayout->setContentsMargins(25, 0, 0, 10);

    QLabel* titleLabel = new QLabel("Choose a Section");
    titleLabel->setStyleSheet("font-size: 30px; font-weight: bold;");
    titleLayout->addWidget(titleLabel);
    // <= END



    // => SECTION IMAGES LAYOUT
    QHBoxLayout* imageLayout = new QHBoxLayout();
    imageLayout->setAlignment(Qt::AlignCenter);

    QPixmap firstImageContainerImage = imageCache.value("Bream");
    QPixmap secondImageContainerImage = imageCache.value("Dish_O'_The_Sea");
    QPixmap thirdImageContainerImage = imageCache.value("Heart");
    QWidget* firstImageContainer = createClickableLabel(firstImageContainerImage, "Fish");
    QWidget* secondImageContainer = createClickableLabel(secondImageContainerImage, "Cooking");
    QWidget* thirdImageContainer = createClickableLabel(thirdImageContainerImage, "NPCs");

    imageLayout->addWidget(firstImageContainer);
    imageLayout->addWidget(secondImageContainer);
    imageLayout->addWidget(thirdImageContainer);
    // <= END


    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(titleLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(imageLayout);
    centralWidget->setLayout(mainLayout);

    fishController = new FishManagementController(nullptr, databasePath, service, userId);
    fishController->setImageCache(imageCache);
    fishController->setupLayout();
    fishController->setWindowTitle("Fish Window");

    connect(static_cast<ClickableLabel*>(firstImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onFirstImageClicked);
    connect(static_cast<ClickableLabel*>(secondImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onSecondImageClicked);
    connect(static_cast<ClickableLabel*>(thirdImageContainer->findChild<ClickableLabel*>()), &ClickableLabel::clicked, this, &MainWindow::onThirdImageClicked);
}

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

void MainWindow::onReturnButtonClicked() {
    emit returnToUserAccountsWindow();
    close();
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



// => DRAGGING FUNCTIONALITY
void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Mouse pressed!";
        isDragging = true;
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        qDebug() << "Mouse moved!";
        move(event->globalPos() - dragStartPosition);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Mouse released!";
        isDragging = false;
    }
}
// <= END



MainWindow::~MainWindow() {
	delete fishController;
}