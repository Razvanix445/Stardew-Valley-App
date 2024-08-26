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
    //qDebug() << "Fish count: " << fishList.size();
    
    setupLayout();
}

FishManagementController::~FishManagementController() {}

void FishManagementController::setupLayout() {
    ui.centralWidget->setAttribute(Qt::WA_TranslucentBackground, true);

    ui.filtersLayout->setAlignment(Qt::AlignTop);
    ui.filtersLayout->setSpacing(10);
    ui.filtersLayout->setContentsMargins(0, 0, 0, 0);

    /*BackgroundWidget* allWidgetFilter = new BackgroundWidget("src/resources/images/EmptyPanel.png");
    QVBoxLayout* allFilterLayout = new QVBoxLayout();
    allWidgetFilter->setLayout(allFilterLayout);
    allWidgetFilter->setFixedSize(80, 80);
    HoverButton* allButtonFilter = new HoverButton("src/resources/images/Stardrop.png", "", 50, 50);
    allFilterLayout->addWidget(allButtonFilter);
    ui.filtersLayout->addWidget(allWidgetFilter);*/


    // => CREATING FILTER BOXES
    vector<char> descriptionPanel = service.getImageByName("DescriptionPanel");
    DetailBox* seasonDetailBox = new DetailBox("Filter by Season", descriptionPanel);
    seasonDetailBox->setCornerRadius(0);
    seasonDetailBox->addButton("Spring");
    seasonDetailBox->addButton("Summer");
    seasonDetailBox->addButton("Fall");
    seasonDetailBox->addButton("Winter");
    //vector<char> seasonButtonFilterImage = service.getImageByName("Bream");
    HoverButton* seasonButtonFilter = new HoverButton(descriptionPanel, "S", 70, 70, seasonDetailBox);
    ui.filtersLayout->addWidget(seasonButtonFilter);

    connect(seasonDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    DetailBox* weatherDetailBox = new DetailBox("Filter by Weather", descriptionPanel);
    weatherDetailBox->setCornerRadius(0);
    weatherDetailBox->addButton("Sun");
    weatherDetailBox->addButton("Rain");
    weatherDetailBox->addButton("Wind");
    //vector<char> weatherButtonFilterImage = service.getImageByName("Bream");
    HoverButton* weatherButtonFilter = new HoverButton(descriptionPanel, "W", 70, 70, weatherDetailBox);
    ui.filtersLayout->addWidget(weatherButtonFilter);

    connect(weatherDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    DetailBox* locationDetailBox = new DetailBox("Filter by Location", descriptionPanel);
    locationDetailBox->setCornerRadius(0);
    vector<string> locations = service.getAllLocations();
    for (const string& location : locations) {
        locationDetailBox->addButton(location);
    }
    //vector<char> locationButtonFilterImage = service.getImageByName("Bream");
    HoverButton* locationButtonFilter = new HoverButton(descriptionPanel, "L", 70, 70, locationDetailBox);
    ui.filtersLayout->addWidget(locationButtonFilter);

    connect(locationDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);
    // <= END


    // => TIME SLIDER
    BackgroundWidget* sliderWidget = new BackgroundWidget(descriptionPanel);
    rangeSlider = new RangeSlider(Qt::Horizontal, RangeSlider::DoubleHandles, this);
    rangeSlider->SetRange(0, 20);
    rangeSlider->SetLowerValue(0);
    rangeSlider->SetUpperValue(20);
    sliderWidget->setFixedSize(70, 50);

    QVBoxLayout* layout = new QVBoxLayout(sliderWidget);
    layout->addWidget(rangeSlider);
    layout->setContentsMargins(5, 10, 5, 10);

    ui.filtersLayout->addWidget(sliderWidget);
    
    connect(rangeSlider, &RangeSlider::lowerValueChanged, this, &FishManagementController::onLowerValueChanged);
    connect(rangeSlider, &RangeSlider::upperValueChanged, this, &FishManagementController::onUpperValueChanged);
    // <= END



    // => LINE EDIT WIDGET
    ui.filteringCondition->setStyleSheet("QLineEdit {"
        "background: transparent;"
        "border: none;"
        "}");

    vector<char> lineEditWidgetImage = service.getImageByName("Label");
    BackgroundWidget* lineEditWidget = new BackgroundWidget(lineEditWidgetImage);
    lineEditWidget->setCornerRadius(0);
    QVBoxLayout* lineEditLayout = new QVBoxLayout();
    lineEditWidget->setLayout(lineEditLayout);
    lineEditLayout->addWidget(ui.filteringCondition);
    lineEditWidget->setMaximumWidth(641);

    ui.FilteringOptionLayout->addWidget(lineEditWidget);
    ui.FilteringOptionLayout->setAlignment(Qt::AlignLeft);
    // <= END



    // => TEXT EDIT WIDGET
    ui.dataFiltersLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    vector<char> verticalPanel = service.getImageByName("LargePanel");
    BackgroundWidget* filterChoosesWidget = new BackgroundWidget(verticalPanel);
    /*QWidget* filterChoosesWidget = new QWidget();*/
    QVBoxLayout* filterChoosesLayout = new QVBoxLayout(filterChoosesWidget);
    filterChoosesLayout->setContentsMargins(5, 10, 5, 10);
    filterChoosesWidget->setFixedWidth(70);

    QString styleSheet = "QTextEdit {"
        "background: transparent;"
        "border: none;"
        "font-size: 10px;"
        "}";

    seasonText = new QTextEdit();
    seasonText->setStyleSheet(styleSheet);
    seasonText->setText("Season: ?");

    weatherText = new QTextEdit();
    weatherText->setStyleSheet(styleSheet);
    weatherText->setText("Weather: ?");

    locationText = new QTextEdit();
    locationText->setStyleSheet(styleSheet);
    locationText->setText("Location: ?");
    
    startTimeText = new QTextEdit();
    startTimeText->setStyleSheet(styleSheet);
    startTimeText->setText("Start time:\n6AM");
    startTimeText->setReadOnly(true);

    endTimeText = new QTextEdit();
    endTimeText->setStyleSheet(styleSheet);
    endTimeText->setText("End time:\n2AM");
    endTimeText->setReadOnly(true);

    filterChoosesLayout->addWidget(seasonText);
    filterChoosesLayout->addWidget(weatherText);
    filterChoosesLayout->addWidget(locationText);
    filterChoosesLayout->addWidget(startTimeText);
    filterChoosesLayout->addWidget(endTimeText);

    ui.dataFiltersLayout->addWidget(filterChoosesWidget);
    // <= END



    // => FISH LAYOUT
    vector<char> backgroundWidgetImage = service.getImageByName("EmptyPanel");
    backgroundWidget = new BackgroundWidget(backgroundWidgetImage);
    backgroundWidget->setFixedSize(640, 500);
    fishLayout = new QVBoxLayout();
    fishLayout->setSpacing(0);
    fishLayout->setContentsMargins(40, 40, 40, 40);
    fishLayout->setAlignment(Qt::AlignTop);
    backgroundWidget->setLayout(fishLayout);

    populateFishLayout(service.getAllFish(username));
    // <= END



    // => CLOSE BUTTON
    closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->setAlignment(Qt::AlignRight);
    fishLayout->addLayout(closeButtonLayout);

    closeButton = new QPushButton("Close");
    closeButton->setStyleSheet("background-color: #f44336; color: white; border: none; padding: 10px 20px; border-radius: 5px;");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButtonLayout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &FishManagementController::on_closeButton_clicked);
    // <= END

    ui.fishAndFiltersLayout->addWidget(backgroundWidget);
}



void FishManagementController::populateFishLayout(const std::vector<Fish>& fishList) {
	deleteLayouts(fishLayout);

    currentRowLayout = nullptr;

    vector<char> fishToolTipImage = service.getImageByName("Fish_ToolTip");
    vector<char> checkmarkImage = service.getImageByName("Checkmark_Little");
    vector<char> favoriteImage = service.getImageByName("Favorite_Little");

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
        // Iterate through each item in the fishLayout
        QLayoutItem* item;
        while ((item = fishLayout->takeAt(0))) {
            if (item->layout()) {
                // If the item is a layout (e.g., QHBoxLayout), clear its contents
                QLayout* hboxLayout = item->layout();
                QLayoutItem* hboxItem;
                while ((hboxItem = hboxLayout->takeAt(0))) {
                    if (hboxItem->widget()) {
                        QLabel* label = qobject_cast<QLabel*>(hboxItem->widget());
                        if (label) {
                            // Clear the QPixmap
                            label->clear();  // This releases the pixmap associated with the label
                            qDebug() << "Cleared label";
                        }
                        // Delete the QLabel
                        qDebug() << "Deleting label";
                        delete label;
                    }
                    qDebug() << "Deleting hboxItem";
                    delete hboxItem;  // Delete the layout item
                }
                delete hboxLayout;  // Delete the QHBoxLayout itself
            }
        }
    }

    
}



void FishManagementController::onCheckBoxStateChanged(bool state) {
	qDebug() << "Checkbox state changed (FishManagementController): " << state;
    populateFishLayout(service.getAllFish(username));
}



void FishManagementController::onFishClicked(QMouseEvent* event) {
    QLabel* clickedLabel = qobject_cast<QLabel*>(sender());
    if (clickedLabel) {
        long fishId = clickedLabel->property("fishId").toLongLong();
        qDebug() << "Fish ID (FishManagementController): " << fishId;
        Fish fish = service.getFishById(fishId, username);
        fish.setId(fishId);
        const vector<char>& backgroundImage = service.getImageByName("Horizontal_Panel");
        FishDetailsWindow* fishWindow = new FishDetailsWindow(nullptr, service, fish, username, backgroundImage);
        fishWindow->show();
    }
}


void FishManagementController::onLowerValueChanged(int lowerValue) {
    QTime startTime = timeMap.value(lowerValue, QTime());
    QTime endTime = timeMap.value(rangeSlider->GetUpperValue(), QTime());

    startTimeText->setText("Start time:\n" + startTime.toString("hAP"));
    endTimeText->setText("End time:\n" + endTime.toString("hAP"));

    populateFishByTimeInterval(startTime, endTime);
}

void FishManagementController::onUpperValueChanged(int upperValue) {
    QTime endTime = timeMap.value(upperValue, QTime());
    QTime startTime = timeMap.value(rangeSlider->GetLowerValue(), QTime());

    startTimeText->setText("Start time:\n" + startTime.toString("hAP"));
    endTimeText->setText("End time:\n" + endTime.toString("hAP"));

    populateFishByTimeInterval(startTime, endTime);
}

void FishManagementController::populateFishByTimeInterval(const QTime& startTime, const QTime& endTime) {
    qDebug() << "Start time: " << startTime.toString("hAP");
    qDebug() << "End time: " << endTime.toString("hAP");

    //vector<Fish> allFish = service.getAllFish();
    //vector<Fish> filteredFish;

    //for (const Fish& fish : allFish) {
    //    // Parse the catching times from strings
    //    QTime fishStart = QTime::fromString(fish.getStartCatchingHour(), "hAP");
    //    QTime fishEnd = QTime::fromString(fish.getEndCatchingHour(), "hAP");

    //    // Ensure the fish start and end times are within the selected interval
    //    if (fishStart >= startTime && fishEnd <= endTime) {
    //        filteredFish.push_back(fish);
    //    }
    //}

    //populateFishLayout(filteredFish);
}


void FishManagementController::handleDetailBoxButtonClicked(const string& name) {
	qDebug() << "DetailBox button clicked: " << QString::fromStdString(name);
    vector<Fish> allFish = service.getAllFishFiltered(username, name);

	populateFishLayout(allFish);
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
	close();
}