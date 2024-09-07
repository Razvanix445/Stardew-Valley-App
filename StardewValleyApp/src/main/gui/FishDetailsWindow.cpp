#include "FishDetailsWindow.h"

FishDetailsWindow::FishDetailsWindow(QWidget *parent, Service& service, Fish fish, const string& username, const QPixmap& backgroundImage)
	: BackgroundWidget(backgroundImage, parent), service(service), fish(fish), username(username), isDragging(false)
{
	qDebug() << "Fish ID in FishDetailsWindow: " << fish.toString();
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setAttribute(Qt::WA_StyledBackground, true);
	setFixedSize(650, 200);
	setCornerRadius(0);
}

void FishDetailsWindow::setImageCache(QMap<QString, QPixmap> images) {
	imageCache = images;
}

void FishDetailsWindow::setupLayout()
{
	QVBoxLayout* windowLayout = new QVBoxLayout(this);

	QHBoxLayout* mainLayout = new QHBoxLayout();
	mainLayout->setAlignment(Qt::AlignTop);

	QHBoxLayout* footerLayout = new QHBoxLayout();

	descriptionLabel = new QLabel(QString::fromStdString(fish.getDescription()));
	descriptionLabel->setStyleSheet("font-size: 10px; color: gray; font-style: italic;");
	descriptionLabel->setWordWrap(true);



	// => MAIN LAYOUT WIDGETS
	imageLabel = new QLabel();
	QPixmap pixmap;
	pixmap.loadFromData(reinterpret_cast<const uchar*>(fish.getImage().data()), fish.getImage().size());
	imageLabel->setPixmap(pixmap);
	imageLabel->setScaledContents(true);
	imageLabel->setFixedSize(60, 60);


	QVBoxLayout* titleLayout = new QVBoxLayout();
	titleLayout->setAlignment(Qt::AlignCenter);

	nameLabel = new QLabel(QString::fromStdString(fish.getName()));
	nameLabel->setStyleSheet("font-size: 20px; color: gray;");

	categoryLabel = new QLabel(QString::fromStdString(fish.getCategory()));
	categoryLabel->setStyleSheet("color: blue;");

	QVBoxLayout* otherDetailsLayout = new QVBoxLayout();
	otherDetailsLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);


	seasonsLabel = new QLabel(this);
	seasonsLabel->setWordWrap(true);
	seasonsLabel->setContentsMargins(5, 0, 0, 0);
	seasonsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	weatherLabel = new QLabel(this);
	weatherLabel->setWordWrap(true);
	weatherLabel->setContentsMargins(5, 0, 0, 0);
	weatherLabel->setStyleSheet("font-size: 14px;");
	locationsLabel = new QLabel(this);
	locationsLabel->setWordWrap(true);
	locationsLabel->setContentsMargins(5, 0, 0, 0);
	locationsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	timeLabel = new QLabel(this);
	timeLabel->setWordWrap(true);
	timeLabel->setContentsMargins(5, 0, 0, 0);
	timeLabel->setStyleSheet("font-size: 14px;");
	difficultyLabel = new QLabel(this);
	difficultyLabel->setWordWrap(true);
	difficultyLabel->setContentsMargins(5, 0, 0, 0);
	difficultyLabel->setStyleSheet("font-size: 14px;");


	setSeasons(fish.getSeason());
	setWeather(fish.getWeather());
	setLocations(fish.getLocation());
	setTime(fish.getStartCatchingHour(), fish.getEndCatchingHour());
	setDifficulty(fish.getDifficulty(), fish.getMovement());



	// => CUSTOM CHECKBOXES FOR FAVORITE AND CAUGHT
	QPixmap uncheckmarkPixmap = imageCache.value("Uncheckmark");
	QPixmap checkmarkPixmap = imageCache.value("Checkmark");
	QPixmap emptyHeartPixmap = imageCache.value("Empty_Heart");
	QPixmap heartPixmap = imageCache.value("Heart");
	QPixmap horizontalPanelPixmap = imageCache.value("Horizontal_Panel");

	// CAUGHT CHECKBOX
	caughtCheckbox = new CustomCheckBox(this, horizontalPanelPixmap);
	caughtCheckbox->setFixedSize(40, 40);
	caughtCheckbox->setToolTipText(QString("Mark as Caught"));
	caughtCheckbox->setChecked(fish.getIsCaught());

	caughtCheckbox->setImages(checkmarkPixmap, uncheckmarkPixmap);

	// FAVORITE CHECKBOX
	favoriteCheckbox = new CustomCheckBox(this, horizontalPanelPixmap);
	favoriteCheckbox->setFixedSize(40, 40);
	favoriteCheckbox->setToolTipText(QString("Mark as Favorite"));
	favoriteCheckbox->setChecked(fish.getIsFavorite());

	favoriteCheckbox->setImages(heartPixmap, emptyHeartPixmap);
	// <= END



	QHBoxLayout* closeButtonLayout = new QHBoxLayout();
	closeButtonLayout->setAlignment(Qt::AlignRight);

	QPushButton* closeButton = new QPushButton("Close");
	closeButton->setStyleSheet("background-color: #f44336; color: white; border: none; padding: 10px 20px; border-radius: 5px;");
	closeButton->setCursor(Qt::PointingHandCursor);
	closeButtonLayout->addWidget(closeButton);

	connect(closeButton, &QPushButton::clicked, this, &FishDetailsWindow::on_closeButton_clicked);
	connect(caughtCheckbox, &CustomCheckBox::stateChanged, this, &FishDetailsWindow::onCheckBoxStateChanged);
	connect(favoriteCheckbox, &CustomCheckBox::stateChanged, this, &FishDetailsWindow::onCheckBoxStateChanged);




	titleLayout->addWidget(imageLabel);
	titleLayout->addWidget(nameLabel);
	titleLayout->addWidget(categoryLabel);

	mainLayout->addLayout(titleLayout);
	mainLayout->addWidget(locationsLabel);
	mainLayout->addWidget(seasonsLabel);

	otherDetailsLayout->addWidget(weatherLabel);
	otherDetailsLayout->addWidget(timeLabel);
	otherDetailsLayout->addWidget(difficultyLabel);
	mainLayout->addLayout(otherDetailsLayout);

	footerLayout->addWidget(descriptionLabel);
	footerLayout->addWidget(favoriteCheckbox);
	footerLayout->addWidget(caughtCheckbox);
	footerLayout->addLayout(closeButtonLayout);

	windowLayout->addLayout(mainLayout);
	windowLayout->addLayout(footerLayout);
}



void FishDetailsWindow::setSeasons(const vector<string>& seasons) {
	QString seasonText = "<span style=\"color:red;\">Seasons: </span><br>";
	for (const auto& season : seasons) {
		seasonText += QString::fromStdString(season) + "<br>";
	}
	if (!seasons.empty())
		seasonText.chop(2);
	locationsLabel->setStyleSheet("white-space:nowrap;");
	seasonsLabel->setText(seasonText);
}

void FishDetailsWindow::setWeather(const vector<string>& weathers) {
	QString weatherText = "<span style=\"color:blue;\">Weather: </span>";
	for (const auto& weather : weathers) {
		weatherText += QString::fromStdString(weather) + ", ";
	}
	if (!weathers.empty())
		weatherText.chop(2);
	weatherLabel->setText(weatherText);
}

void FishDetailsWindow::setLocations(const vector<string>& locations) {
	QString locationText = "<span style=\"color:green;\">Locations: </span><br>";
	for (const auto& location : locations) {
		locationText += QString::fromStdString(location) + "<br>";
	}
	if (!locations.empty())
		locationText.chop(2);
	locationsLabel->setStyleSheet("white-space:nowrap;");
	locationsLabel->setText(locationText);
}

void FishDetailsWindow::setTime(const string& start, const string& end) {
	QString timeText = "<span style=\"color:white;\">Time: </span>";
	if (start == "Any") {
		timeLabel->setText(timeText + "Any");
	}
	else {
		QStringList startSplit = QString::fromStdString(start).split(", ");
		QStringList endSplit = QString::fromStdString(end).split(", ");
		qDebug() << startSplit << endSplit;

		if (startSplit.size() == 2 && endSplit.size() == 2) {
			timeLabel->setText(timeText + startSplit[0] + "-" + endSplit[0] + " and " + startSplit[1] + "-" + endSplit[1]);
		}
		else {
			timeLabel->setText(timeText + QString::fromStdString(start) + "-" + QString::fromStdString(end));
		}

	}
}

void FishDetailsWindow::setDifficulty(int difficulty, const string& movement) {
	QString difficultyText = "<span style=\"color:brown;\">Difficulty: </span>";
	difficultyText += QString::fromStdString(to_string(difficulty)) + " " + QString::fromStdString(movement);
	difficultyLabel->setText(difficultyText);
}



void FishDetailsWindow::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging = true;
		dragStartPosition = event->globalPos() - frameGeometry().topLeft();
	}
}

void FishDetailsWindow::mouseMoveEvent(QMouseEvent* event) {
	if (isDragging) {
		move(event->globalPos() - dragStartPosition);
	}
}

void FishDetailsWindow::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		isDragging = false;
	}
}

void FishDetailsWindow::on_closeButton_clicked() {
	close();
}

void FishDetailsWindow::onCheckBoxStateChanged(bool checked) {
	qDebug() << "Checkbox state changed: " << (checked ? "Checked" : "Unchecked");

	if (sender() == caughtCheckbox) {
		qDebug() << "Caught checkbox changed.";
		fish.setIsCaught(checked);
	}
	else if (sender() == favoriteCheckbox) {
		qDebug() << "Favorite checkbox changed.";
		fish.setIsFavorite(checked);
	}

	service.updateFish(fish, username);

	qDebug() << "Emitting fishDetailsUpdated signal with fish ID: " << fish.getId();
	emit fishDetailsUpdated(fish.getId());
}

FishDetailsWindow::~FishDetailsWindow()
{}
