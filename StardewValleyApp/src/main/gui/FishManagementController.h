#pragma once

#include "../model/Fish.h"
#include "../repository/FishDBRepository.h"
#include "../utils/BackgroundWidget.h"
#include "../utils/CustomButton.h"
#include "../utils/HoverButton.h"
#include "../utils/ClickableLabel.h"
#include "../service/Service.h"
#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <string>
#include <vector>
#include "ui_FishManagementController.h"

using namespace std;

class FishManagementController : public QMainWindow
{
	Q_OBJECT

public:
	explicit FishManagementController(QWidget* parent, const string& databasePath, Service& service, const string& username);
	~FishManagementController() override;

private:

	Service& service;
	string username;
	string databasePath;
	Ui::FishManagementControllerClass ui;

	bool isDragging;
	QPoint dragStartPosition;
	QWidget* dragHandle;

	void setupLayout();
	void populateFishLayout(const vector<Fish>& fishList);
    void deleteLayouts(QLayout* layout);


	BackgroundWidget* backgroundWidget;
	QVBoxLayout* fishLayout;
    QHBoxLayout* currentRowLayout;
    QHBoxLayout* closeButtonLayout;
	/*QHBoxLayout* currentRowLayout1;
	QHBoxLayout* currentRowLayout2;
	QHBoxLayout* currentRowLayout3;
	QHBoxLayout* currentRowLayout4;
	QHBoxLayout* currentRowLayout5;
	QHBoxLayout* currentRowLayout6;
	QHBoxLayout* currentRowLayout7;
	QHBoxLayout* currentRowLayout8;
	QHBoxLayout* currentRowLayout9;*/
	QLabel* fishLabel;
	QPixmap fishPixmap;
	QPushButton* closeButton;


private slots:
	void onFishClicked(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void on_closeButton_clicked();
	void on_lineEditWidget_textChanged(const QString& text);
	void handleDetailBoxButtonClicked(const string& name);
};


/*
void FishManagementController::setupLayout() {
    ui.centralWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    





    ui.filtersLayout->setAlignment(Qt::AlignTop);
    ui.filtersLayout->setSpacing(10);
    ui.filtersLayout->setContentsMargins(0, 0, 0, 0);
    */
    /*BackgroundWidget* allWidgetFilter = new BackgroundWidget("src/resources/images/EmptyPanel.png");
    QVBoxLayout* allFilterLayout = new QVBoxLayout();
    allWidgetFilter->setLayout(allFilterLayout);
    allWidgetFilter->setFixedSize(80, 80);
    HoverButton* allButtonFilter = new HoverButton("src/resources/images/Stardrop.png", "", 50, 50);
    allFilterLayout->addWidget(allButtonFilter);
    ui.filtersLayout->addWidget(allWidgetFilter);*/
/*
DetailBox* allDetailBox = new DetailBox("Filter by Anything", "src/resources/images/DescriptionPanel.png");
allDetailBox->setCornerRadius(0);
HoverButton* allButtonFilter = new HoverButton("src/resources/images/Stardrop_On_A_Frame.png", "", 70, 70, allDetailBox);
ui.filtersLayout->addWidget(allButtonFilter);

connect(allDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

DetailBox* seasonDetailBox = new DetailBox("Filter by Season", "src/resources/images/DescriptionPanel.png");
seasonDetailBox->setCornerRadius(0);
vector<string> seasons = service.getAllSeasons();
for (const string& season : seasons) {
    seasonDetailBox->addButton(season);
}
HoverButton* seasonButtonFilter = new HoverButton("src/resources/images/Seasons_On_A_Frame.png", "", 70, 70, seasonDetailBox);
ui.filtersLayout->addWidget(seasonButtonFilter);

connect(seasonDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

DetailBox* weatherDetailBox = new DetailBox("Filter by Weather", "src/resources/images/DescriptionPanel.png");
weatherDetailBox->setCornerRadius(0);
vector<string> weathers = service.getAllWeathers();
for (const string& w : weathers) {
    weatherDetailBox->addButton(w);
}
HoverButton* weatherButtonFilter = new HoverButton("src/resources/images/Weather_On_A_Frame.png", "", 70, 70, weatherDetailBox);
ui.filtersLayout->addWidget(weatherButtonFilter);

connect(weatherDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

DetailBox* locationDetailBox = new DetailBox("Filter by Location", "src/resources/images/DescriptionPanel.png");
locationDetailBox->setCornerRadius(0);
vector<string> locations = service.getAllLocations();
for (const string& location : locations) {
    locationDetailBox->addButton(location);
}
HoverButton* locationButtonFilter = new HoverButton("src/resources/images/Locations_On_A_Frame.png", "", 70, 70, locationDetailBox);
ui.filtersLayout->addWidget(locationButtonFilter);

connect(locationDetailBox, &DetailBox::buttonClicked, this, &FishManagementController::handleDetailBoxButtonClicked);

ui.filteringCondition->setStyleSheet("QLineEdit {"
    "background: transparent;"
    "border: none;"
    "}");

BackgroundWidget* lineEditWidget = new BackgroundWidget("src/resources/images/Label.png");
lineEditWidget->setCornerRadius(0);
QVBoxLayout* lineEditLayout = new QVBoxLayout();
lineEditWidget->setLayout(lineEditLayout);
lineEditLayout->addWidget(ui.filteringCondition);

ui.FilteringOptionLayout->addWidget(lineEditWidget);
}



void FishManagementController::populateFishLayout(const std::vector<Fish>& fishList) {
    // Create the BackgroundWidget
    backgroundWidget = new BackgroundWidget("src/resources/images/EmptyPanel.png");
    fishLayout = new QVBoxLayout();  // Create a new layout for fish items
    fishLayout->setSpacing(0);
    fishLayout->setContentsMargins(40, 40, 40, 40);
    fishLayout->setAlignment(Qt::AlignTop);

    backgroundWidget->setLayout(fishLayout);

    currentRowLayout1 = new QHBoxLayout();
    currentRowLayout2 = new QHBoxLayout();
    currentRowLayout3 = new QHBoxLayout();
    currentRowLayout4 = new QHBoxLayout();
    currentRowLayout5 = new QHBoxLayout();
    currentRowLayout6 = new QHBoxLayout();
    currentRowLayout7 = new QHBoxLayout();
    currentRowLayout8 = new QHBoxLayout();

    fishLayout->addLayout(currentRowLayout1);
    fishLayout->addLayout(currentRowLayout2);
    fishLayout->addLayout(currentRowLayout3);
    fishLayout->addLayout(currentRowLayout4);
    fishLayout->addLayout(currentRowLayout5);
    fishLayout->addLayout(currentRowLayout6);
    fishLayout->addLayout(currentRowLayout7);
    fishLayout->addLayout(currentRowLayout8);

    currentRowLayout1->setSpacing(0);
    currentRowLayout1->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout2);

    currentRowLayout2->setSpacing(0);
    currentRowLayout2->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout2);

    currentRowLayout3->setSpacing(0);
    currentRowLayout3->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout3);

    currentRowLayout4->setSpacing(0);
    currentRowLayout4->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout4);

    currentRowLayout5->setSpacing(0);
    currentRowLayout5->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout5);

    currentRowLayout6->setSpacing(0);
    currentRowLayout6->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout6);

    currentRowLayout7->setSpacing(0);
    currentRowLayout7->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout7);

    currentRowLayout8->setSpacing(0);
    currentRowLayout8->setContentsMargins(10, 10, 10, 10);
    fishLayout->addLayout(currentRowLayout8);


    int fishCount = 0;
    for (const auto& fish : fishList) {
        fishLabel = new QLabel();
        fishPixmap.loadFromData(reinterpret_cast<const uchar*>(fish.getImage().data()), fish.getImage().size());
        fishLabel->setPixmap(fishPixmap);
        fishLabel->setScaledContents(true);
        fishLabel->setFixedSize(60, 60);  // Adjust size as needed

        fishLabel->setStyleSheet("background: transparent;");  // Ensure QLabel background is transparent

        fishLabel->setProperty("fishId", QVariant::fromValue(fish.getId()));  // Store fish ID for later use

        if (fishCount % 10 == 0) {
            currentRowLayout1->addWidget(fishLabel, 1);
        }
        else if (fishCount % 10 == 1) {
            currentRowLayout2->addWidget(fishLabel, 1);
        }
        else if (fishCount % 10 == 2) {
            currentRowLayout3->addWidget(fishLabel, 1);
        }
        else if (fishCount % 10 == 3) {
            currentRowLayout4->addWidget(fishLabel, 1);
        }
        else if (fishCount % 10 == 4) {
            currentRowLayout5->addWidget(fishLabel, 1);
        }
        else if (fishCount % 10 == 5) {
            currentRowLayout6->addWidget(fishLabel, 1);
        }
        else if (fishCount % 10 == 6) {
            currentRowLayout7->addWidget(fishLabel, 1);
        }
        else if (fishCount % 10 == 7) {
            currentRowLayout8->addWidget(fishLabel, 1);
        }

        fishCount++;
    }

    currentRowLayout9 = new QHBoxLayout();
    currentRowLayout9->setAlignment(Qt::AlignRight);
    fishLayout->addLayout(currentRowLayout9);

    closeButton = new QPushButton("Close");
    closeButton->setStyleSheet("background-color: #f44336; color: white; border: none; padding: 10px 20px; border-radius: 5px;");
    closeButton->setCursor(Qt::PointingHandCursor);
    currentRowLayout9->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &FishManagementController::on_closeButton_clicked);

    // Add the BackgroundWidget to the central layout
    ui.fishAndFiltersLayout->addWidget(backgroundWidget);




}
*/