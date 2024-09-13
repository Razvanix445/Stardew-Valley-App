#pragma once

#include "../model/Fish.h"
#include "../repository/FishDBRepository.h"
#include "../utils/BackgroundWidget.h"
#include "../utils/CustomButton.h"
#include "../utils/ComplexHoverButton.h"
#include "../utils/ClickableLabel.h"
#include "../utils/FishLabel.h"
#include "../service/Service.h"
#include "../gui/FishDetailsWindow.h"
#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QButtonGroup>
#include <QDebug>
#include <QProgressBar>
#include <QScreen>
#include <string>
#include <vector>
#include <QTime>
#include <QTextEdit>
#include "ui_FishManagementController.h"

using namespace std;

class FishManagementController : public QMainWindow
{
	Q_OBJECT

public:
	explicit FishManagementController(QWidget* parent, const string& databasePath, Service& service, const long userId);
	~FishManagementController() override;
	void setImageCache(QMap<QString, QPixmap> images);
	void setupLayout();

private:

	Service& service;
	const long userId;
	string databasePath;
	Ui::FishManagementControllerClass ui;

	bool isDragging;
	QPoint dragStartPosition;
	QWidget* dragHandle;

	QTextEdit* seasonText;
	QTextEdit* weatherText;
	QTextEdit* locationText;

	CustomCheckBox* singleCheckbox;
	CustomCheckBox* multipleCheckbox;
	QPushButton* applyMultipleFiltersButton;
	QPushButton* refreshButton;
	CustomCheckBox* uncaughtFishCheckbox;
	CustomCheckBox* favoriteFishCheckbox;

	QMap<QString, QString> selectedOptions;

	QProgressBar* achievementProgress;

	void populateFishLayout(const vector<Fish>& fishList);
    void deleteLayouts(QLayout* layout);
	void refreshChosenFilters();

	BackgroundWidget* backgroundWidget;
	QVBoxLayout* fishLayout;
    QHBoxLayout* currentRowLayout;
    QHBoxLayout* closeButtonLayout;
	QHBoxLayout* achievementLayout;
	QHBoxLayout* bottomLayout;
	QLabel* fishLabel;
	QPixmap fishPixmap;
	QPushButton* closeButton;
	QVBoxLayout* filterChoosesLayout;

	DetailBox* seasonDetailBox;
	DetailBox* weatherDetailBox;
	DetailBox* locationDetailBox;

	QMap<QString, QPixmap> imageCache;
	QPixmap pixmap;
	QPixmap checkmarkPixmap;
	QPixmap favoritePixmap;

	QString progressBarUnfinishedStyleSheet = "QProgressBar {"
		"background-color: #D7A96B; "
		"color: #4C5550;"
		"border-style: none;"
		"border-radius: 10px;"
		"text-align: center;"
		"}"
		"QProgressBar::chunk {"
		"border-radius: 10px;"
		"background-color: qlineargradient(spread: pad, x1: 0, y1: 0.511364, x2: 1, y2: 0.523, stop: 0 #F2D16B, stop: 1 #87CBB9);"
		"}";

	QString progressBarFinishedStyleSheet = "QProgressBar {"
		"background-color: #D7A96B; "
		"font-weight: bold;"
		"color: #4CAF50;"
		"border-style: none;"
		"border-radius: 10px;"
		"text-align: center;"
		"}"
		"QProgressBar::chunk {"
		"border-radius: 10px;"
		"background-color: #4C5550;"
		"}";

private slots:
	void onFishClicked(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void on_closeButton_clicked();
	void on_lineEditWidget_textChanged(const QString& text);
	void handleDetailBoxButtonClicked(const string& name);
	void onFishDetailsUpdated(long fishId);

	void onSingleCheckboxToggled(bool checked);
	void onMultipleCheckboxToggled(bool checked);
	void applyFilters();
	void onUncaughtFishCheckboxToggled(bool checked);
	void onFavoriteFishCheckboxToggled(bool checked);
	void refresh();
};
