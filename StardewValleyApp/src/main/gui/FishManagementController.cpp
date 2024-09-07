#include "FishManagementController.h"

FishManagementController::FishManagementController(QWidget *parent, const string& databasePath, Service& service, const string& username)
    : QMainWindow(parent), databasePath(databasePath), service(service), username(username), isDragging(false)
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, true);

    connect(ui.filteringCondition, &QLineEdit::textChanged, this, &FishManagementController::on_lineEditWidget_textChanged);

    FishDBRepository fishRepository(databasePath);
    vector<Fish> fishList = service.getAllFish(username);
}

void FishManagementController::setImageCache(QMap<QString, QPixmap> images) {
    imageCache = images;
    qDebug() << "imageCache in FishManagementController " << imageCache.size();
}

FishManagementController::~FishManagementController() {}

void FishManagementController::setupLayout() {
    ui.centralWidget->setAttribute(Qt::WA_TranslucentBackground, true);

    ui.filtersLayout->setAlignment(Qt::AlignTop);
    ui.filtersLayout->setSpacing(10);
    ui.filtersLayout->setContentsMargins(0, 0, 0, 0);

    checkmarkPixmap = imageCache.value("Checkmark_Little");
    if (checkmarkPixmap.size().isEmpty())
        qDebug() << "Checkmark_Little not loaded!";
	else
		qDebug() << "Checkmark_Little loaded successfully!";
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
    HoverButton* seasonButtonFilter = new HoverButton(pixmap, "S", 70, 70, seasonDetailBox);
    ui.filtersLayout->addWidget(seasonButtonFilter);

    connect(seasonDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    weatherDetailBox = new DetailBox("Filter by Weather", pixmap);
    weatherDetailBox->setCornerRadius(0);
    weatherDetailBox->addButton("All (No Filter)");
    weatherDetailBox->addButton("Sun");
    weatherDetailBox->addButton("Rain");
    weatherDetailBox->addButton("Wind");
    HoverButton* weatherButtonFilter = new HoverButton(pixmap, "W", 70, 70, weatherDetailBox);
    ui.filtersLayout->addWidget(weatherButtonFilter);

    connect(weatherDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    locationDetailBox = new DetailBox("Filter by Location", pixmap);
    locationDetailBox->setCornerRadius(0);
    vector<string> locations = service.getAllLocations();
    locationDetailBox->addButton("All (No Filter)");
    for (const string& location : locations) {
        locationDetailBox->addButton(location);
    }
    HoverButton* locationButtonFilter = new HoverButton(pixmap, "L", 70, 70, locationDetailBox);
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



    // => FILTERS CHECKBOXES
    QVBoxLayout* filterCheckboxLayout = new QVBoxLayout();
    filterCheckboxLayout->setAlignment(Qt::AlignBottom);

    singleCheckbox = new QCheckBox("One Filter");
    singleCheckbox->setStyleSheet("font-size: 13px;");
    multipleCheckbox = new QCheckBox("Multiple Filters");
    multipleCheckbox->setStyleSheet("font-size: 13px;");

    QButtonGroup* checkboxGroup = new QButtonGroup(this);
    checkboxGroup->addButton(singleCheckbox);
    checkboxGroup->addButton(multipleCheckbox);
    singleCheckbox->setChecked(true);
    
    applyMultipleFiltersButton = new QPushButton("Apply Filters");
    applyMultipleFiltersButton->setStyleSheet("background-color: #4CAF50; color: white; border: none; border-radius: 5px; font-size: 13px;");
    applyMultipleFiltersButton->setCursor(Qt::PointingHandCursor);
    applyMultipleFiltersButton->setFixedHeight(30);

    filterCheckboxLayout->addWidget(singleCheckbox);
    filterCheckboxLayout->addWidget(multipleCheckbox);
    filterCheckboxLayout->addWidget(applyMultipleFiltersButton);

    rightLayout->addLayout(filterCheckboxLayout);

    connect(singleCheckbox, &QCheckBox::toggled, this, &FishManagementController::onSingleCheckboxToggled);
    connect(multipleCheckbox, &QCheckBox::toggled, this, &FishManagementController::onMultipleCheckboxToggled);
    connect(applyMultipleFiltersButton, &QPushButton::clicked, this, &FishManagementController::applyFilters);

    selectedOptions["season"] = "";
    selectedOptions["weather"] = "";
    selectedOptions["location"] = "";
    // <= END




    // => FISH LAYOUT
    pixmap = imageCache.value("EmptyPanel");
    backgroundWidget = new BackgroundWidget(pixmap);
    backgroundWidget->setFixedSize(640, 500);
    fishLayout = new QVBoxLayout();
    fishLayout->setSpacing(0);
    fishLayout->setContentsMargins(40, 40, 40, 40);
    fishLayout->setAlignment(Qt::AlignTop);
    backgroundWidget->setLayout(fishLayout);

    populateFishLayout(service.getAllFish(username));
    // <= END



    // => CLOSE BUTTON
    pixmap = imageCache.value("Uncheckmark");
    closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->setAlignment(Qt::AlignRight);
    fishLayout->addLayout(closeButtonLayout);

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
            currentRowLayout->setAlignment(Qt::AlignLeft);
            fishLayout->addLayout(currentRowLayout);
        }

        FishLabel* fishLabel = new FishLabel(fishToolTipImage);
        fishLabel->setFishDetails(fish, checkmarkImage, favoriteImage);

        connect(fishLabel, &FishLabel::clicked, this, &FishManagementController::onFishClicked);

        currentRowLayout->addWidget(fishLabel, 1);
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
                    Fish updatedFish = service.getFishById(fishId, username);
                    qDebug() << updatedFish.getName().c_str() << " " << updatedFish.toString();

                    fishLabel->setFishDetails(updatedFish, checkmarkPixmap, favoritePixmap);
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
        Fish fish = service.getFishById(fishId, username);
        fish.setId(fishId);
        pixmap = imageCache.value("Horizontal_Panel");

        FishDetailsWindow* fishWindow = new FishDetailsWindow(nullptr, service, fish, username, pixmap);
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
			populateFishLayout(service.getAllFishBySeason(username, option));
		}
        else if (sender() == weatherDetailBox) {
            qDebug() << "Filtered by weather!";
			populateFishLayout(service.getAllFishByWeather(username, option));
		}
        else if (sender() == locationDetailBox) {
            qDebug() << "Filtered by location!";
			populateFishLayout(service.getAllFishByLocation(username, option));
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
        populateFishLayout(service.getAllFishBySeasonWeatherLocation(username, season.toStdString(), weather.toStdString(), location.toStdString()));
    }
}



void FishManagementController::on_lineEditWidget_textChanged(const QString& text) {
	qDebug() << "Text changed: " << text;
    vector<Fish> allFish = service.getAllFishFiltered(username, text.toStdString());

    populateFishLayout(allFish);
}





void FishManagementController::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
    }
}

void FishManagementController::mouseMoveEvent(QMouseEvent* event) {
    if (isDragging) {
        move(event->globalPos() - dragStartPosition);
    }
}

void FishManagementController::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
    }
}

void FishManagementController::on_closeButton_clicked() {
	//close();
    this->hide();
}