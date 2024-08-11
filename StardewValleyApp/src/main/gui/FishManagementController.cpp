#include "FishManagementController.h"

FishManagementController::FishManagementController(QWidget *parent, const string& databasePath, Service& service, const string& username)
    : QMainWindow(parent), databasePath(databasePath), service(service), username(username), isDragging(false)
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  // Hide window title bar
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_StyledBackground, true);

    connect(ui.filteringCondition, &QLineEdit::textChanged, this, &FishManagementController::on_lineEditWidget_textChanged);

    FishDBRepository fishRepository(databasePath);
    vector<Fish> fishList = service.getAllFish(username);
    qDebug() << "Fish count: " << fishList.size();
    
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

    vector<char> seasonDetailBoxImage = service.getImageByName("DescriptionPanel");
    DetailBox* seasonDetailBox = new DetailBox("Filter by Season", seasonDetailBoxImage);
    seasonDetailBox->setCornerRadius(0);
    seasonDetailBox->addButton("Spring");
    seasonDetailBox->addButton("Summer");
    seasonDetailBox->addButton("Fall");
    seasonDetailBox->addButton("Winter");
    vector<char> seasonButtonFilterImage = service.getImageByName("Seasons_On_A_Frame");
    HoverButton* seasonButtonFilter = new HoverButton(seasonButtonFilterImage, "", 70, 70, seasonDetailBox);
    ui.filtersLayout->addWidget(seasonButtonFilter);

    connect(seasonDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    vector<char> weatherDetailBoxImage = service.getImageByName("DescriptionPanel");
    DetailBox* weatherDetailBox = new DetailBox("Filter by Weather", weatherDetailBoxImage);
    weatherDetailBox->setCornerRadius(0);
    weatherDetailBox->addButton("Sun");
    weatherDetailBox->addButton("Rain");
    weatherDetailBox->addButton("Wind");
    vector<char> weatherButtonFilterImage = service.getImageByName("Weather_On_A_Frame");
    HoverButton* weatherButtonFilter = new HoverButton(weatherButtonFilterImage, "", 70, 70, weatherDetailBox);
    ui.filtersLayout->addWidget(weatherButtonFilter);

    connect(weatherDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

    vector<char> locationDetailBoxImage = service.getImageByName("DescriptionPanel");
    DetailBox* locationDetailBox = new DetailBox("Filter by Location", locationDetailBoxImage);
    locationDetailBox->setCornerRadius(0);
    vector<string> locations = service.getAllLocations();
    for (const string& location : locations) {
        locationDetailBox->addButton(location);
    }
    vector<char> locationButtonFilterImage = service.getImageByName("Locations_On_A_Frame");
    HoverButton* locationButtonFilter = new HoverButton(locationButtonFilterImage, "", 70, 70, locationDetailBox);
    ui.filtersLayout->addWidget(locationButtonFilter);

    connect(locationDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

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

    ui.FilteringOptionLayout->addWidget(lineEditWidget);


    // 3. Add fish layout
    vector<char> backgroundWidgetImage = service.getImageByName("EmptyPanel");
    backgroundWidget = new BackgroundWidget(backgroundWidgetImage);
    backgroundWidget->setFixedSize(640, 500);
    fishLayout = new QVBoxLayout();
    fishLayout->setSpacing(0);
    fishLayout->setContentsMargins(40, 40, 40, 40);
    fishLayout->setAlignment(Qt::AlignTop);
    backgroundWidget->setLayout(fishLayout);

    populateFishLayout(service.getAllFish(username));

    // 5. Add close button
    closeButtonLayout = new QHBoxLayout();
    closeButtonLayout->setAlignment(Qt::AlignRight);
    fishLayout->addLayout(closeButtonLayout);

    closeButton = new QPushButton("Close");
    closeButton->setStyleSheet("background-color: #f44336; color: white; border: none; padding: 10px 20px; border-radius: 5px;");
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButtonLayout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &FishManagementController::on_closeButton_clicked);

    // 6. Add the BackgroundWidget to the layout
    ui.fishAndFiltersLayout->addWidget(backgroundWidget);
    qDebug() << backgroundWidget->size();
}



void FishManagementController::populateFishLayout(const std::vector<Fish>& fishList) {
	deleteLayouts(fishLayout);

    currentRowLayout = nullptr;

    vector<char> fishToolTipImage = service.getImageByName("Fish_ToolTip");

    int fishCount = 0;
    for (const auto& fish : fishList) {
        if (fishCount % 10 == 0) {
            currentRowLayout = new QHBoxLayout();
            currentRowLayout->setSpacing(0);
            currentRowLayout->setAlignment(Qt::AlignLeft);
            //currentRowLayout->setContentsMargins(10, 10, 10, 10);
            fishLayout->addLayout(currentRowLayout);
        }

        FishLabel* fishLabel = new FishLabel(fishToolTipImage);
        QPixmap fishPixmap;
        fishPixmap.loadFromData(reinterpret_cast<const uchar*>(fish.getImage().data()), fish.getImage().size());
        fishLabel->setPixmap(fishPixmap);
        fishLabel->setScaledContents(true);
        fishLabel->setFixedSize(60, 60);
        fishLabel->setContentsMargins(5, 5, 5, 5);
        fishLabel->setStyleSheet("background: transparent;");
        fishLabel->setProperty("fishId", QVariant::fromValue(fish.getId()));

        fishLabel->setFishDetails(fish);

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



void FishManagementController::onFishClicked(QMouseEvent* event) {
    QLabel* clickedLabel = qobject_cast<QLabel*>(sender());
    if (clickedLabel) {
        long fishId = clickedLabel->property("fishId").toLongLong();
        qDebug() << "Fish ID: " << fishId;
    }
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