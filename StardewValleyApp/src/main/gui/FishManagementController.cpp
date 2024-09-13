#include "FishManagementController.h"

FishManagementController::FishManagementController(QWidget *parent, const string& databasePath, Service& service, const long userId)
    : QMainWindow(parent), databasePath(databasePath), service(service), userId(userId), isDragging(false)
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, true);

    connect(ui.filteringCondition, &QLineEdit::textChanged, this, &FishManagementController::on_lineEditWidget_textChanged);

    FishDBRepository fishRepository(databasePath);
    vector<Fish> fishList = service.getAllFish(userId);
}

void FishManagementController::setImageCache(QMap<QString, QPixmap> images) {
    imageCache = images;
}

FishManagementController::~FishManagementController() {}

void FishManagementController::setupLayout() {
    ui.centralWidget->setAttribute(Qt::WA_TranslucentBackground, true);

    ui.filtersLayout->setAlignment(Qt::AlignTop);
    ui.filtersLayout->setSpacing(10);
    ui.filtersLayout->setContentsMargins(0, 0, 0, 0);

    checkmarkPixmap = imageCache.value("Checkmark_Little");
    favoritePixmap = imageCache.value("Favorite_Little");


    // => CREATING FILTER BOXES
    pixmap = imageCache.value("DescriptionPanel");
    seasonDetailBox = new DetailBox("Filter by Season", pixmap);
    seasonDetailBox->setCornerRadius(0);
    seasonDetailBox->addButton("All (No Filter)");
    seasonDetailBox->addButton("Spring");
    seasonDetailBox->addButton("Summer");
    seasonDetailBox->addButton("Fall");
    seasonDetailBox->addButton("Winter");
    ComplexHoverButton* seasonButtonFilter = new ComplexHoverButton(pixmap, "S", 70, 70, seasonDetailBox);
    ui.filtersLayout->addWidget(seasonButtonFilter);

    connect(seasonDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    weatherDetailBox = new DetailBox("Filter by Weather", pixmap);
    weatherDetailBox->setCornerRadius(0);
    weatherDetailBox->addButton("All (No Filter)");
    weatherDetailBox->addButton("Sun");
    weatherDetailBox->addButton("Rain");
    weatherDetailBox->addButton("Wind");
    ComplexHoverButton* weatherButtonFilter = new ComplexHoverButton(pixmap, "W", 70, 70, weatherDetailBox);
    ui.filtersLayout->addWidget(weatherButtonFilter);

    connect(weatherDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    locationDetailBox = new DetailBox("Filter by Location", pixmap);
    locationDetailBox->setCornerRadius(0);
    vector<string> locations = service.getAllLocations();
    locationDetailBox->addButton("All (No Filter)");
    for (const string& location : locations) {
        locationDetailBox->addButton(location);
    }
    ComplexHoverButton* locationButtonFilter = new ComplexHoverButton(pixmap, "L", 70, 70, locationDetailBox);
    ui.filtersLayout->addWidget(locationButtonFilter);

    connect(locationDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);
    // <= END

    
    // => LINE EDIT WIDGET
    ui.filteringCondition->setStyleSheet("QLineEdit {"
        "background: transparent;"
        "border: none;"
        "}");

    pixmap = imageCache.value("Label");
    BackgroundWidget* lineEditWidget = new BackgroundWidget(pixmap);
    lineEditWidget->setCornerRadius(0);
    QVBoxLayout* lineEditLayout = new QVBoxLayout();
    lineEditWidget->setLayout(lineEditLayout);
    lineEditLayout->addWidget(ui.filteringCondition);
    lineEditWidget->setMaximumWidth(641);

    ui.FilteringOptionLayout->addWidget(lineEditWidget);
    ui.FilteringOptionLayout->setAlignment(Qt::AlignLeft);
    // <= END



    // => TEXT EDIT WIDGET
    pixmap = imageCache.value("LargePanel");
    BackgroundWidget* rightWidget = new BackgroundWidget(pixmap);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(10, 10, 10, 10);
    rightLayout->setAlignment(Qt::AlignTop);
    rightWidget->setFixedWidth(130);

    QString styleSheet = "QTextEdit {"
        "background-color: rgba(0, 0, 0, 20);"
        "border-radius: 10px;"
        "font-size: 10px;"
        "}";

    seasonText = new QTextEdit();
    seasonText->setMaximumSize(120, 50);
    seasonText->setStyleSheet(styleSheet);
    seasonText->setText("Season:\n?");
    seasonText->setAlignment(Qt::AlignCenter);
    seasonText->setHtml("<div style='text-align: center;'>Season:<br/>?</div>");

    weatherText = new QTextEdit();
    weatherText->setMaximumSize(120, 50);
    weatherText->setStyleSheet(styleSheet);
    weatherText->setText("Weather:\n?");
    weatherText->setAlignment(Qt::AlignCenter);
    weatherText->setHtml("<div style='text-align: center;'>Weather:<br/>?</div>");

    locationText = new QTextEdit();
    locationText->setMaximumSize(120, 50);
    locationText->setStyleSheet(styleSheet);
    locationText->setText("Location:\n?");
    locationText->setAlignment(Qt::AlignCenter);
    locationText->setHtml("<div style='text-align: center;'>Location:<br/>?</div>");

    filterChoosesLayout = new QVBoxLayout();
    
    
    filterChoosesLayout->addWidget(seasonText);
    filterChoosesLayout->addWidget(weatherText);
    filterChoosesLayout->addWidget(locationText);
    seasonText->hide();
    weatherText->hide();
    locationText->hide();

    connect(seasonDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);
    connect(weatherDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);
    connect(locationDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    rightLayout->addLayout(filterChoosesLayout);
    // <= END



    // => RIGHT LAYOUT
    QVBoxLayout* filterCheckboxLayout = new QVBoxLayout();
    filterCheckboxLayout->setAlignment(Qt::AlignBottom);

    // => Checkboxes
    QPixmap uncheckmarkPixmap = imageCache.value("Uncheckmark_Panel");
    QPixmap checkmarkPixmap = imageCache.value("Checkmark_Panel");
    QPixmap horizontalPanelPixmap = imageCache.value("Horizontal_Panel");

    singleCheckbox = new CustomCheckBox(this, horizontalPanelPixmap);
    singleCheckbox->setFixedSize(100, 20);
    singleCheckbox->setStyleSheet("font-size: 13px;");
    singleCheckbox->setImages(checkmarkPixmap, uncheckmarkPixmap);
    singleCheckbox->setText("Single Filters");

    multipleCheckbox = new CustomCheckBox(this, horizontalPanelPixmap);
    multipleCheckbox->setFixedSize(120, 20);
    multipleCheckbox->setStyleSheet("font-size: 13px;");
    multipleCheckbox->setImages(checkmarkPixmap, uncheckmarkPixmap);
    multipleCheckbox->setText("Multiple Filters");

    QButtonGroup* checkboxGroup = new QButtonGroup(this);
    checkboxGroup->addButton(singleCheckbox);
    checkboxGroup->addButton(multipleCheckbox);
    singleCheckbox->setChecked(true);
    // <= END
    
    // => Apply Filters Button
    applyMultipleFiltersButton = new QPushButton("Apply Filters");
    applyMultipleFiltersButton->setStyleSheet("background-color: #4CAF50; color: white; border: none; border-radius: 5px; font-size: 13px;");
    applyMultipleFiltersButton->setCursor(Qt::PointingHandCursor);
    applyMultipleFiltersButton->setFixedHeight(30);
    // <= END

    // => Caught Fish Checkbox
    uncaughtFishCheckbox = new CustomCheckBox(this, horizontalPanelPixmap);
    uncaughtFishCheckbox->setFixedSize(130, 20);
    uncaughtFishCheckbox->setStyleSheet("font-size: 13px;");
    uncaughtFishCheckbox->setImages(checkmarkPixmap, uncheckmarkPixmap);
    uncaughtFishCheckbox->setText("Show Uncaught");
    // <= END

    // => Favorite Fish Checkbox
    favoriteFishCheckbox = new CustomCheckBox(this, horizontalPanelPixmap);
    favoriteFishCheckbox->setFixedSize(130, 20);
    favoriteFishCheckbox->setStyleSheet("font-size: 13px;");
    favoriteFishCheckbox->setImages(checkmarkPixmap, uncheckmarkPixmap);
    favoriteFishCheckbox->setText("Show Favorite");
    // <= END

    // => Refresh Button
    QPixmap refreshPixmap = imageCache.value("refreshButton");
    refreshButton = new QPushButton("");
    refreshButton->setIcon(QIcon(refreshPixmap));
    refreshButton->setIconSize(QSize(50, 50));
    refreshButton->setStyleSheet("background: transparent; border: none;");
    refreshButton->setCursor(Qt::PointingHandCursor);
    // <= END

    filterCheckboxLayout->addWidget(singleCheckbox);
    filterCheckboxLayout->addWidget(multipleCheckbox);
    filterCheckboxLayout->addWidget(applyMultipleFiltersButton);
    QSpacerItem* spacer1 = new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);
    filterCheckboxLayout->addItem(spacer1);
    filterCheckboxLayout->addWidget(uncaughtFishCheckbox);
    filterCheckboxLayout->addWidget(favoriteFishCheckbox);
    QSpacerItem* spacer2 = new QSpacerItem(0, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);
    filterCheckboxLayout->addItem(spacer2);
    filterCheckboxLayout->addWidget(refreshButton);

    rightLayout->addLayout(filterCheckboxLayout);

    connect(singleCheckbox, &QCheckBox::toggled, this, &FishManagementController::onSingleCheckboxToggled);
    connect(multipleCheckbox, &QCheckBox::toggled, this, &FishManagementController::onMultipleCheckboxToggled);
    connect(applyMultipleFiltersButton, &QPushButton::clicked, this, &FishManagementController::applyFilters);
    connect(uncaughtFishCheckbox, &QCheckBox::toggled, this, &FishManagementController::onUncaughtFishCheckboxToggled);
    connect(favoriteFishCheckbox, &QCheckBox::toggled, this, &FishManagementController::onFavoriteFishCheckboxToggled);
    connect(refreshButton, &QPushButton::clicked, this, &FishManagementController::refresh);

    selectedOptions["season"] = "";
    selectedOptions["weather"] = "";
    selectedOptions["location"] = "";

    rightLayout->setStretch(0, 1);
    rightLayout->setStretch(1, 0);
    // <= END


    // => FISH LAYOUT
    pixmap = imageCache.value("EmptyPanel");
    backgroundWidget = new BackgroundWidget(pixmap);
    backgroundWidget->setFixedWidth(640);
    fishLayout = new QVBoxLayout();
    fishLayout->setSpacing(0);
    fishLayout->setContentsMargins(0, 20, 0, 0);
    fishLayout->setAlignment(Qt::AlignTop);
    backgroundWidget->setLayout(fishLayout);

    populateFishLayout(service.getAllFish(userId));
    // <= END



    // => CLOSE BUTTON LAYOUT
    pixmap = imageCache.value("Uncheckmark");
    closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->setAlignment(Qt::AlignCenter | Qt::AlignRight);

    // Close button
    closeButton = new QPushButton();
    closeButton->setIcon(QIcon(pixmap));
    closeButton->setIconSize(QSize(40, 40));
    
    closeButton->setStyleSheet("background: transparent; border: none;");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButtonLayout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &FishManagementController::on_closeButton_clicked);
    // <= END

    ui.dataFiltersLayout->addWidget(rightWidget);
    ui.fishAndFiltersLayout->addWidget(backgroundWidget);



    // => BOTTOM LAYOUT
    pixmap = imageCache.value("Label");
    QHBoxLayout* achievementLayout = new QHBoxLayout();
    achievementLayout->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

    QTextEdit* achievementText = new QTextEdit("Master Angler Progression: ");
    achievementText->setStyleSheet("background: transparent; color: #4C4A50; border-radius: 10px; font-size: 14px;");
    achievementText->setAlignment(Qt::AlignCenter);
    achievementText->setReadOnly(true);
    achievementText->setFixedSize(150, 40);

    achievementProgress = new QProgressBar();
    achievementProgress->setFixedSize(200, 30);
    achievementProgress->setStyleSheet(progressBarUnfinishedStyleSheet);
    achievementProgress->setValue(service.getCaughtFishNumber(userId) * 100 / service.getAllFishNumber());

    achievementLayout->addWidget(achievementText);
    achievementLayout->addWidget(achievementProgress);
    // <= END

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0, 40, 20, 30);

    bottomLayout->addLayout(achievementLayout);
    bottomLayout->addLayout(closeButtonLayout);

    fishLayout->addLayout(bottomLayout);
}



void FishManagementController::populateFishLayout(const std::vector<Fish>& fishList) {
	deleteLayouts(fishLayout);

    currentRowLayout = nullptr;

    QPixmap fishToolTipImage = imageCache.value("Fish_ToolTip");
    QPixmap checkmarkImage = imageCache.value("Checkmark_Little");
    QPixmap favoriteImage = imageCache.value("Favorite_Little");

    int fishCount = 0;
    for (const auto& fish : fishList) {
        if (fishCount % 10 == 0) {
            currentRowLayout = new QHBoxLayout();
            currentRowLayout->setSpacing(0);
            currentRowLayout->setContentsMargins(20, 0, 20, 0);
            currentRowLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
            fishLayout->addLayout(currentRowLayout);
        }

        FishLabel* fishLabel = new FishLabel(fishToolTipImage);
        fishLabel->setFishDetails(fish, checkmarkImage, favoriteImage);

        connect(fishLabel, &FishLabel::clicked, this, &FishManagementController::onFishClicked);

        currentRowLayout->addWidget(fishLabel);
        fishCount++;
    }
}

void FishManagementController::deleteLayouts(QLayout* layout) {
    if (fishLayout) {
        QLayoutItem* item;
        while ((item = fishLayout->takeAt(0))) {
            if (item->layout()) {
                QLayout* hboxLayout = item->layout();
                QLayoutItem* hboxItem;
                while ((hboxItem = hboxLayout->takeAt(0))) {
                    if (hboxItem->widget()) {
                        QLabel* label = qobject_cast<QLabel*>(hboxItem->widget());
                        if (label) {
                            // Clear the QPixmap
                            label->clear();
                        }
                        // Delete the QLabel
                        delete label;
                    }
                    // Delete the layout item
                    delete hboxItem;
                }
                // Delete the empty QHBoxLayout
                delete hboxLayout;
            }
        }
    }
}



void FishManagementController::onFishDetailsUpdated(long fishId) {
    qDebug() << "Fish details updated (FishManagementController). Updating fish with ID: " << fishId << "!";
	
    for (int i = 0; i < fishLayout->count(); i++) {
        QHBoxLayout* rowLayout = qobject_cast<QHBoxLayout*>(fishLayout->itemAt(i)->layout());
        if (rowLayout) {
            for (int j = 0; j < rowLayout->count(); j++) {
                FishLabel* fishLabel = qobject_cast<FishLabel*>(rowLayout->itemAt(j)->widget());
                if (fishLabel && fishLabel->property("fishId").toLongLong() == fishId) {
                    Fish updatedFish = service.getFishById(fishId, userId);
                    qDebug() << updatedFish.getName().c_str() << " " << updatedFish.toString();

                    fishLabel->setFishDetails(updatedFish, checkmarkPixmap, favoritePixmap);
                    achievementProgress->setValue(service.getCaughtFishNumber(userId) * 100 / service.getAllFishNumber());
                    if (achievementProgress->value() == 100)
                        achievementProgress->setStyleSheet(progressBarFinishedStyleSheet);
                    else
                        achievementProgress->setStyleSheet(progressBarUnfinishedStyleSheet);
                    return;
                }
            }
        }
    }
}



void FishManagementController::onFishClicked(QMouseEvent* event) {
    QLabel* clickedLabel = qobject_cast<QLabel*>(sender());
    if (clickedLabel) {
        long fishId = clickedLabel->property("fishId").toLongLong();
        qDebug() << "Fish ID (FishManagementController): " << fishId;
        Fish fish = service.getFishById(fishId, userId);
        fish.setId(fishId);
        pixmap = imageCache.value("Horizontal_Panel");

        FishDetailsWindow* fishWindow = new FishDetailsWindow(nullptr, service, fish, userId, pixmap);
        fishWindow->setImageCache(imageCache);
        fishWindow->setupLayout();

        connect(fishWindow, &FishDetailsWindow::destroyed, fishWindow, &FishDetailsWindow::deleteLater);

        connect(fishWindow, &FishDetailsWindow::fishDetailsUpdated, this, &FishManagementController::onFishDetailsUpdated);
        fishWindow->show();
    }
}


void FishManagementController::onSingleCheckboxToggled(bool checked) {
    if (checked) {
		multipleCheckbox->setChecked(false);
	}
}

void FishManagementController::onMultipleCheckboxToggled(bool checked) {
    if (checked) {
		singleCheckbox->setChecked(false);
	}
}


void FishManagementController::handleDetailBoxButtonClicked(const string& option) {
    if (singleCheckbox->isChecked()) {
        seasonText->hide();
        weatherText->hide();
        locationText->hide();
        if (sender() == seasonDetailBox) {
            qDebug() << "Filtered by season!";
			populateFishLayout(service.getAllFishBySeason(userId, option));
		}
        else if (sender() == weatherDetailBox) {
            qDebug() << "Filtered by weather!";
			populateFishLayout(service.getAllFishByWeather(userId, option));
		}
        else if (sender() == locationDetailBox) {
            qDebug() << "Filtered by location!";
			populateFishLayout(service.getAllFishByLocation(userId, option));
		}
    }
    else if (multipleCheckbox->isChecked()) {
        if (sender() == seasonDetailBox) {
            if (option == "All (No Filter)") {
                selectedOptions["season"] = "";
				seasonText->hide();
			}
            else {
                selectedOptions["season"] = QString::fromStdString(option);
                seasonText->setHtml(
                    "<div style='text-align: center;'>"
                    "<span style='color: red; font-size: 14px;'>Season:</span><br/>"
                    + QString::fromStdString(option) +
                    "</div>");
                seasonText->show();
            }
        }
        else if (sender() == weatherDetailBox) {
            if (option == "All (No Filter)") {
                selectedOptions["weather"] = "";
                weatherText->hide();
            }
            else {
                selectedOptions["weather"] = QString::fromStdString(option);
                weatherText->setHtml(
                    "<div style='text-align: center;'>"
                    "<span style='color: blue; font-size: 14px;'>Weather:</span><br/>"
                    + QString::fromStdString(option) +
                    "</div>");
                weatherText->show();
            }
        }
        else if (sender() == locationDetailBox) {
            if (option == "All (No Filter)") {
                selectedOptions["location"] = "";
				locationText->hide();
			}
            else {
                selectedOptions["location"] = QString::fromStdString(option);
                locationText->setHtml(
                    "<div style='text-align: center;'>"
                    "<span style='color: green; font-size: 14px;'>Location:</span><br/>"
                    + QString::fromStdString(option) +
                    "</div>"
                );
                locationText->show();
            }
        }
    }
}


void FishManagementController::applyFilters() {
    if (multipleCheckbox->isChecked()) {
        QString season = selectedOptions["season"];
        QString weather = selectedOptions["weather"];
        QString location = selectedOptions["location"];
        populateFishLayout(service.getAllFishBySeasonWeatherLocation(userId, season.toStdString(), weather.toStdString(), location.toStdString()));
    }
}


void FishManagementController::onUncaughtFishCheckboxToggled(bool checked) {
    if (checked && !favoriteFishCheckbox->isChecked()) {
		populateFishLayout(service.getAllUncaughtFish(userId));
	}
    else if (checked) {
        favoriteFishCheckbox->setChecked(false);
		populateFishLayout(service.getAllUncaughtFish(userId));
	}
    else {
		populateFishLayout(service.getAllFish(userId));
	
    }
}


void FishManagementController::onFavoriteFishCheckboxToggled(bool checked) {
    if (checked && !uncaughtFishCheckbox->isChecked()) {
        populateFishLayout(service.getAllFavoriteFish(userId));
    }
    else if (checked) {
        uncaughtFishCheckbox->setChecked(false);
        populateFishLayout(service.getAllFavoriteFish(userId));
    }
    else {
		populateFishLayout(service.getAllFish(userId));
	}
}


void FishManagementController::refresh() {
	populateFishLayout(service.getAllFish(userId));
    refreshChosenFilters();
    if (uncaughtFishCheckbox->isChecked())
        uncaughtFishCheckbox->setChecked(false);
    if (favoriteFishCheckbox->isChecked())
        favoriteFishCheckbox->setChecked(false);
}

void FishManagementController::refreshChosenFilters() {
	selectedOptions["season"] = "All (No Filter)";
	selectedOptions["weather"] = "All (No Filter)";
	selectedOptions["location"] = "All (No Filter)";
	seasonText->hide();
	weatherText->hide();
	locationText->hide();
}


void FishManagementController::on_lineEditWidget_textChanged(const QString& text) {
	qDebug() << "Text changed: " << text;
    vector<Fish> allFish = service.getAllFishFiltered(userId, text.toStdString());

    populateFishLayout(allFish);
}





void FishManagementController::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Mouse pressed!";
        isDragging = true;
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
    }
}

void FishManagementController::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        qDebug() << "Mouse moved!";
        move(event->globalPos() - dragStartPosition);
    }
}

void FishManagementController::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Mouse released!";
        isDragging = false;
    }
}

void FishManagementController::on_closeButton_clicked() {
    // Hide the window
    this->hide();

    // Refresh the window to appear as if it was closed
    refresh();

    // Get the screen size
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Get the window size
    QSize windowSize = size();

    // Center the window relative to the screen
    int x = (screenWidth - windowSize.width()) / 2;
    int y = (screenHeight - windowSize.height()) / 2;
    move(x, y);
}