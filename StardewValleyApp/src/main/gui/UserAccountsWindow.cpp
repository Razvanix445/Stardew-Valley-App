#include "UserAccountsWindow.h"

UserAccountsWindow::UserAccountsWindow(QWidget* parent, const string& databasePath, Service& service)
	: QMainWindow(parent), databasePath(databasePath), service(service)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setAttribute(Qt::WA_StyledBackground, true);
}

void UserAccountsWindow::setImageCache(QMap<QString, QPixmap> images) {
	imageCache = images;
}

void UserAccountsWindow::setupLayout() {
	pixmap = imageCache.value("Horizontal_Panel");
	BackgroundWidget* centralWidget = new BackgroundWidget(pixmap, this);
	centralWidget->setCornerRadius(0);
	setCentralWidget(centralWidget);

	QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
	mainLayout->setAlignment(Qt::AlignTop);

	// => TOP LAYOUT
	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->setContentsMargins(25, 25, 25, 0);
	
	// => Title Label
	QLabel* titleLabel = new QLabel("Choose Your Account");
	titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
	// <= End

	// => Close Button
	QHBoxLayout* closeButtonLayout = new QHBoxLayout();
	closeButtonLayout->setAlignment(Qt::AlignCenter | Qt::AlignRight);

	pixmap = imageCache.value("Uncheckmark");
	QPushButton* closeButton = new QPushButton();
	closeButton->setStyleSheet("background: transparent; border: none;");
	closeButton->setIcon(pixmap);
	closeButton->setIconSize(QSize(40, 40));

	closeButtonLayout->addWidget(closeButton);
	connect(closeButton, &QPushButton::clicked, this, &UserAccountsWindow::on_closeButton_clicked);
	// <= End

	topLayout->addWidget(titleLabel);
	topLayout->addLayout(closeButtonLayout);
	// <= END




	// => CENTER LAYOUT
	QHBoxLayout* centerLayout = new QHBoxLayout();
	centerLayout->setContentsMargins(25, 10, 25, 25);

	// => User Accounts
	QPixmap horizontalPanelPixmap = imageCache.value("LargePanel_Unhovered");
	QPixmap horizontalPanelHoveredPixmap = imageCache.value("LargePanel");

	int userCount = 0;

	vector<User> users = service.getAllUsers();
	for (const User& user : users) {
		BackgroundHoverWidget* userAccountsPanel = createUserAccountPanel(user, horizontalPanelPixmap, horizontalPanelHoveredPixmap);
		userAccountsPanel->setProperty("userId", QVariant::fromValue(user.getId()));
		centerLayout->addWidget(userAccountsPanel);
		userCount++;
	}

	for (int userNo = userCount; userNo < 5; userNo++) {
		BackgroundHoverWidget* userAccountsPanel = createEmptyAccountPanel(horizontalPanelPixmap, horizontalPanelHoveredPixmap);
		centerLayout->addWidget(userAccountsPanel);
	}
	// <= End
	// <= END

	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(centerLayout);
}



BackgroundHoverWidget* UserAccountsWindow::createEmptyAccountPanel(const QPixmap& originalpixmap, const QPixmap& hoveredPixmap) {
	BackgroundHoverWidget* widget = new BackgroundHoverWidget(originalpixmap, hoveredPixmap, this);

	QVBoxLayout* layout = new QVBoxLayout(widget);
	layout->setAlignment(Qt::AlignCenter);

	QPixmap addButtonPixmap = imageCache.value("AddButton");
	QPixmap addButtonHoveredPixmap = imageCache.value("AddButton_Hovered");

	QPushButton* addButton = new QPushButton();
	addButton->setStyleSheet("background: transparent; border: none;");
	addButton->setIcon(imageCache.value("AddButton"));
	addButton->setIconSize(QSize(100, 100));

	layout->addWidget(addButton);
	widget->setLayout(layout);

	connect(widget, &BackgroundHoverWidget::hoverEnter, this, [=]() {
		addButton->setIcon(addButtonHoveredPixmap);
		});

	connect(widget, &BackgroundHoverWidget::hoverLeave, this, [=]() {
		addButton->setIcon(addButtonPixmap);
		});

	connect(widget, &BackgroundHoverWidget::clicked, this, &UserAccountsWindow::openCreateAccountWindow);
	connect(addButton, &QPushButton::clicked, this, &UserAccountsWindow::openCreateAccountWindow);

	return widget;
}



BackgroundHoverWidget* UserAccountsWindow::createUserAccountPanel(const User& user, const QPixmap& originalpixmap, const QPixmap& hoveredPixmap) {
	BackgroundHoverWidget* widget = new BackgroundHoverWidget(originalpixmap, hoveredPixmap, this);

	QVBoxLayout* layout = new QVBoxLayout(widget);
	layout->setAlignment(Qt::AlignTop);



	// => DELETE ACCOUNT LAYOUT
	QHBoxLayout* deleteAccountLayout = new QHBoxLayout();
	deleteAccountLayout->setAlignment(Qt::AlignRight | Qt::AlignTop);
	deleteAccountLayout->setContentsMargins(0, 25, 20, 0);

	QPixmap deleteAccountPixmap = imageCache.value("Delete");
	HoverButton* deleteAccountButton = new HoverButton(nullptr, QSize(40, 40), QSize(60, 60));
	deleteAccountButton->setStyleSheet("background: transparent; border: none;");
	deleteAccountButton->setIcon(deleteAccountPixmap);

	deleteAccountLayout->addWidget(deleteAccountButton);

	connect(deleteAccountButton, &HoverButton::clicked, this, [=]() {
		deleteUser(user);
		setupLayout();
		});
	// <= END



	// => USER NAME LAYOUT
	QHBoxLayout* userNameLayout = new QHBoxLayout();
	userNameLayout->setAlignment(Qt::AlignCenter);
	userNameLayout->setContentsMargins(0, 0, 0, 10);

	QLabel* userNameLabel = new QLabel(QString::fromStdString(user.getName()));
	userNameLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: brown;");
	userNameLayout->addWidget(userNameLabel);
	// <= END



	// => USER IMAGE LAYOUT
	QVBoxLayout* userImageLayout = new QVBoxLayout();
	userImageLayout->setContentsMargins(0, 0, 0, 0);
	userImageLayout->setAlignment(Qt::AlignCenter);

	QPixmap userImage = user.getImage();
	QLabel* imageLabel = new QLabel();
	imageLabel->setPixmap(userImage.scaled(175, 175));

	userImageLayout->addWidget(imageLabel);
	// <= END

	masterAnglerText = new QLabel();
	masterAnglerProgress = new QProgressBar();
	gourmetChefText = new QLabel();
	gourmetChefProgress = new QProgressBar();
	npcsHeartsText = new QLabel();
	npcsProgress = new QProgressBar();

	fishText = new QLabel();
	caughtFishLabel = new QLabel();
	favoriteFishLabel = new QLabel();
	dishesText = new QLabel();
	discoveredRecipesLabel = new QLabel();
	cookedDishesLabel = new QLabel();
	npcsText = new QLabel();
	eightHeartsNpcs = new QLabel();
	tenHeartsNpcs = new QLabel();

	// => ACHIEVEMENTS LAYOUT
	progressBarsWidget = new QWidget();
	progressBarsWidget->setLayout(createProgressBarsLayout(user));
	statisticsWidget = new QWidget();
	statisticsWidget->setLayout(createStatisticsLayout(user));

	achievementsLayout = new QStackedLayout();
	achievementsLayout->addWidget(progressBarsWidget);
	achievementsLayout->addWidget(statisticsWidget);
	achievementsLayout->setAlignment(Qt::AlignBottom);
	setFontToStatisticsLabels(false);

	QWidget* containerWidget = new QWidget();
	containerWidget->setLayout(achievementsLayout);

	switchTimer = new QTimer(this);
	connect(switchTimer, &QTimer::timeout, this, &UserAccountsWindow::onSwitchTimerTimeout);
	switchTimer->start(5000);
	// <= END

	layout->addLayout(deleteAccountLayout);
	layout->addLayout(userNameLayout);
	layout->addLayout(userImageLayout);
	layout->addSpacerItem(new QSpacerItem(0, 50, QSizePolicy::Minimum, QSizePolicy::Expanding));
	layout->addWidget(containerWidget);

	widget->setLayout(layout);

	connect(widget, &BackgroundHoverWidget::hoverEnter, this, [=]() {
		deleteAccountLayout->setContentsMargins(0, 5, 12, 0);
		deleteAccountButton->setOriginalSize(QSize(50, 50));
		userNameLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: brown;");
		imageLabel->setPixmap(userImage.scaled(200, 200));

		masterAnglerText->setStyleSheet("color: blue; font-size: 22px;");
		masterAnglerProgress->setFixedSize(200, 30);

		gourmetChefText->setStyleSheet("color: green; font-size: 22px;");
		gourmetChefProgress->setFixedSize(200, 30);

		npcsHeartsText->setStyleSheet("color: red; font-size: 22px;");
		npcsProgress->setFixedSize(200, 30);

		setFontToStatisticsLabels(true);

		achievementsLayout->setContentsMargins(0, 0, 0, 0);
		});

	connect(widget, &BackgroundHoverWidget::hoverLeave, this, [=]() {
		deleteAccountLayout->setContentsMargins(0, 25, 20, 0);
		deleteAccountButton->setOriginalSize(QSize(40, 40));
		userNameLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: brown;");
		imageLabel->setPixmap(userImage.scaled(175, 175));

		masterAnglerText->setStyleSheet("color: blue; font-size: 18px;");
		masterAnglerProgress->setFixedSize(175, 25);

		gourmetChefText->setStyleSheet("color: green; font-size: 18px;");
		gourmetChefProgress->setFixedSize(175, 25);

		npcsHeartsText->setStyleSheet("color: red; font-size: 18px;");
		npcsProgress->setFixedSize(175, 25);

		setFontToStatisticsLabels(false);

		achievementsLayout->setContentsMargins(0, 0, 0, 35);
		});

	connect(widget, &BackgroundHoverWidget::clicked, this, &UserAccountsWindow::openMainMenu);

	return widget;
}


void UserAccountsWindow::openMainMenu() {
	BackgroundHoverWidget* widget = qobject_cast<BackgroundHoverWidget*>(sender());
	int userId = widget->property("userId").toInt();
	qDebug() << "User ID: " << userId;

	window = new MainWindow(nullptr, databasePath, service, userId);
	window->setImageCache(imageCache);
	window->setupLayout();

	// Listen to the return button click event from the MainWindow to return to the UserAccountsWindow
	connect(window, &MainWindow::returnToUserAccountsWindow, this, &UserAccountsWindow::show);

	window->show();
	close();
}

void UserAccountsWindow::openCreateAccountWindow() {
	CreateUserWindow* createUserWindow = new CreateUserWindow(nullptr, service);
	createUserWindow->setImageCache(imageCache);
	createUserWindow->setupLayout();
	createUserWindow->exec();
}


// => CREATE PROGRESS BARS LAYOUT
QVBoxLayout* UserAccountsWindow::createProgressBarsLayout(const User& user) {
	// => ACHIEVEMENTS LAYOUT
	QVBoxLayout* achievementsLayout = new QVBoxLayout();
	achievementsLayout->setAlignment(Qt::AlignBottom);
	achievementsLayout->setContentsMargins(0, 0, 0, 35);


	// => FISH PROGRESS LAYOUT
	QVBoxLayout* masterAnglerLayout = new QVBoxLayout();
	masterAnglerLayout->setAlignment(Qt::AlignCenter);

	masterAnglerText->setText("Master Angler: ");
	masterAnglerText->setStyleSheet("color: blue; font-size: 18px;");
	masterAnglerText->setAlignment(Qt::AlignCenter);


	masterAnglerProgress->setFixedSize(175, 25);
	masterAnglerProgress->setValue(service.getCaughtFishNumber(user.getId()) * 100 / service.getAllFishNumber());
	if (masterAnglerProgress->value() == 100)
		masterAnglerProgress->setStyleSheet(progressBarFinishedStyleSheet);
	else
		masterAnglerProgress->setStyleSheet(progressBarUnfinishedStyleSheet);

	masterAnglerLayout->addWidget(masterAnglerText);
	masterAnglerLayout->addWidget(masterAnglerProgress);
	// <= END



	// => DISHES PROGRESS LAYOUT
	QVBoxLayout* gourmetChefLayout = new QVBoxLayout();
	gourmetChefLayout->setAlignment(Qt::AlignCenter);

	gourmetChefText->setText("Gourmet Chef: ");
	gourmetChefText->setStyleSheet("color: green; font-size: 18px;");
	gourmetChefText->setAlignment(Qt::AlignCenter);


	gourmetChefProgress->setFixedSize(175, 25);
	gourmetChefProgress->setValue(0);
	if (gourmetChefProgress->value() == 100)
		gourmetChefProgress->setStyleSheet(progressBarFinishedStyleSheet);
	else
		gourmetChefProgress->setStyleSheet(progressBarUnfinishedStyleSheet);

	gourmetChefLayout->addWidget(gourmetChefText);
	gourmetChefLayout->addWidget(gourmetChefProgress);
	// <= END



	// => NPCs PROGRESS LAYOUT
	QVBoxLayout* npcsLayout = new QVBoxLayout();
	npcsLayout->setAlignment(Qt::AlignCenter);

	npcsHeartsText->setText("Full-Hearts NPCs: ");
	npcsHeartsText->setStyleSheet("color: red; font-size: 18px;");
	npcsHeartsText->setAlignment(Qt::AlignCenter);


	npcsProgress->setFixedSize(175, 25);
	npcsProgress->setValue(0);
	if (npcsProgress->value() == 100)
		npcsProgress->setStyleSheet(progressBarFinishedStyleSheet);
	else
		npcsProgress->setStyleSheet(progressBarUnfinishedStyleSheet);

	npcsLayout->addWidget(npcsHeartsText);
	npcsLayout->addWidget(npcsProgress);
	// <= END

	achievementsLayout->addLayout(masterAnglerLayout);
	achievementsLayout->addLayout(gourmetChefLayout);
	achievementsLayout->addLayout(npcsLayout);
	// <= END

	return achievementsLayout;
}


// => CREATE STATISTICS LAYOUT
QVBoxLayout* UserAccountsWindow::createStatisticsLayout(const User& user) {
	// => ACHIEVEMENTS LAYOUT
	QVBoxLayout* achievementsLayout = new QVBoxLayout();
	achievementsLayout->setAlignment(Qt::AlignBottom);
	achievementsLayout->setContentsMargins(0, 0, 0, 35);


	// => FISH PROGRESS LAYOUT
	QVBoxLayout* mainFishLayout = new QVBoxLayout();
	mainFishLayout->setContentsMargins(0, 0, 0, 10);

	fishText->setText("Fish: ");
	fishText->setStyleSheet("color: blue; font-size: 22px;");


	QVBoxLayout* fishLayout = new QVBoxLayout();

	caughtFishLabel->setText(QString("Caught: %1").arg(service.getCaughtFishNumber(user.getId())));
	caughtFishLabel->setAlignment(Qt::AlignCenter);
	caughtFishLabel->setStyleSheet("color: brown; font-size: 18px;");

	favoriteFishLabel->setText(QString("Favorite: %1").arg(service.getFavoriteFishNumber(user.getId())));
	favoriteFishLabel->setAlignment(Qt::AlignCenter);
	favoriteFishLabel->setStyleSheet("color: red; font-size: 18px;");

	fishLayout->addWidget(caughtFishLabel);
	fishLayout->addWidget(favoriteFishLabel);

	mainFishLayout->addWidget(fishText);
	mainFishLayout->addLayout(fishLayout);
	// <= END



	// => DISHES PROGRESS LAYOUT
	QVBoxLayout* mainDishesLayout = new QVBoxLayout();

	dishesText->setText("Gourmet Chef: ");
	dishesText->setStyleSheet("color: green; font-size: 22px;");


	QVBoxLayout* dishesLayout = new QVBoxLayout();
	dishesLayout->setContentsMargins(0, 0, 0, 10);

	discoveredRecipesLabel->setText("Discovered Recipes: 0");
	discoveredRecipesLabel->setAlignment(Qt::AlignCenter);
	discoveredRecipesLabel->setStyleSheet("color: #90D5FF; font-size: 18px;");

	cookedDishesLabel->setText("Cooked Dishes: 0");
	cookedDishesLabel->setAlignment(Qt::AlignCenter);
	cookedDishesLabel->setStyleSheet("color: #582900; font-size: 18px;");

	dishesLayout->addWidget(discoveredRecipesLabel);
	dishesLayout->addWidget(cookedDishesLabel);

	mainDishesLayout->addWidget(dishesText);
	mainDishesLayout->addLayout(dishesLayout);
	// <= END



	// => NPCs PROGRESS LAYOUT
	QVBoxLayout* mainNpcsLayout = new QVBoxLayout();

	npcsText->setText("Full-Hearts NPCs: 0");
	npcsText->setStyleSheet("color: red; font-size: 22px;");


	QVBoxLayout* npcsLayout = new QVBoxLayout();


	eightHeartsNpcs->setText("8-Hearts Npcs: 0");
	eightHeartsNpcs->setAlignment(Qt::AlignCenter);
	eightHeartsNpcs->setStyleSheet("color: #808080; font-size: 18px;");

	tenHeartsNpcs->setText("10-Hearts Npcs: 0");
	tenHeartsNpcs->setAlignment(Qt::AlignCenter);
	tenHeartsNpcs->setStyleSheet("color: red; font-size: 18px;");

	npcsLayout->addWidget(eightHeartsNpcs);
	npcsLayout->addWidget(tenHeartsNpcs);

	mainNpcsLayout->addWidget(npcsText);
	mainNpcsLayout->addLayout(npcsLayout);
	// <= END

	achievementsLayout->addLayout(mainFishLayout);
	achievementsLayout->addLayout(mainDishesLayout);
	achievementsLayout->addLayout(mainNpcsLayout);
	// <= END

	return achievementsLayout;
}


void UserAccountsWindow::setFontToStatisticsLabels(bool isHovered) {
	if (isHovered) {
		fishText->setStyleSheet("color: blue; font-size: 24px; margin-left: 20px;");
		caughtFishLabel->setStyleSheet("color: brown; font-size: 20px;");
		favoriteFishLabel->setStyleSheet("color: red; font-size: 20px;");
		dishesText->setStyleSheet("color: green; font-size: 24px; margin-left: 20px;");
		discoveredRecipesLabel->setStyleSheet("color: #90D5FF; font-size: 20px;");
		cookedDishesLabel->setStyleSheet("color: #582900; font-size: 20px;");
		npcsText->setStyleSheet("color: red; font-size: 24px; margin-left: 20px;");
		eightHeartsNpcs->setStyleSheet("color: #808080; font-size: 20px;");
		tenHeartsNpcs->setStyleSheet("color: red; font-size: 20px;");
	}
	else {
		fishText->setStyleSheet("color: blue; font-size: 22px; margin-left: 35px;");
		caughtFishLabel->setStyleSheet("color: brown; font-size: 18px;");
		favoriteFishLabel->setStyleSheet("color: red; font-size: 18px;");
		dishesText->setStyleSheet("color: green; font-size: 22px; margin-left: 35px;");
		discoveredRecipesLabel->setStyleSheet("color: #90D5FF; font-size: 18px;");
		cookedDishesLabel->setStyleSheet("color: #582900; font-size: 18px;");
		npcsText->setStyleSheet("color: red; font-size: 22px; margin-left: 35px;");
		eightHeartsNpcs->setStyleSheet("color: #808080; font-size: 18px;");
		tenHeartsNpcs->setStyleSheet("color: red; font-size: 18px;");
	}
}


void UserAccountsWindow::onSwitchTimerTimeout() {
	switchLayouts();
}


void UserAccountsWindow::switchLayouts() {
	QWidget* currentWidget = showingProgressBars ? progressBarsWidget : statisticsWidget;
	QWidget* nextWidget = showingProgressBars ? statisticsWidget : progressBarsWidget;

	// Fade out animation for the current widget
	QGraphicsOpacityEffect* fadeOutEffect = new QGraphicsOpacityEffect(currentWidget);
	currentWidget->setGraphicsEffect(fadeOutEffect);
	QPropertyAnimation* fadeOutAnimation = new QPropertyAnimation(fadeOutEffect, "opacity");
	fadeOutAnimation->setDuration(250);
	fadeOutAnimation->setStartValue(1.0);
	fadeOutAnimation->setEndValue(0.0);

	// Fade in animation for the next widget
	QGraphicsOpacityEffect* fadeInEffect = new QGraphicsOpacityEffect(nextWidget);
	nextWidget->setGraphicsEffect(fadeInEffect);
	QPropertyAnimation* fadeInAnimation = new QPropertyAnimation(fadeInEffect, "opacity");
	fadeInAnimation->setDuration(250);
	fadeInAnimation->setStartValue(0.0);
	fadeInAnimation->setEndValue(1.0);

	// Switching the layouts after the animations are done
	connect(fadeOutAnimation, &QPropertyAnimation::finished, this, [this, nextWidget]() {
		achievementsLayout->setCurrentWidget(nextWidget);
		});

	fadeOutAnimation->start();
	fadeInAnimation->start();

	showingProgressBars = !showingProgressBars;
}


void UserAccountsWindow::deleteUser(const User& user) {
	qDebug() << "You stay alive yet!";
}


void UserAccountsWindow::on_closeButton_clicked() {
	close();
}

UserAccountsWindow::~UserAccountsWindow()
{}
