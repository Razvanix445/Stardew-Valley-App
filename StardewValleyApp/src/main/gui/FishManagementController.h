#pragma once

#include "../model/Fish.h"
#include "../repository/FishDBRepository.h"
#include "../utils/BackgroundWidget.h"
#include "../utils/CustomButton.h"
#include "../utils/HoverButton.h"
#include "../utils/ClickableLabel.h"
#include "../utils/FishLabel.h"
#include "../utils/RangeSlider.h"
#include "../service/Service.h"
#include "../gui/FishDetailsWindow.h"
#include <QMainWindow>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDebug>
#include <string>
#include <vector>
#include <QTime>
#include <QTextEdit>
#include "ui_FishManagementController.h"

using namespace std;

class FishManagementController : public QMainWindow, public ICheckBoxObserver
{
	Q_OBJECT

public:
	explicit FishManagementController(QWidget* parent, const string& databasePath, Service& service, const string& username);
	~FishManagementController() override;

	void onCheckBoxStateChanged(bool state) override;

private:

	Service& service;
	string username;
	string databasePath;
	Ui::FishManagementControllerClass ui;

	bool isDragging;
	QPoint dragStartPosition;
	QWidget* dragHandle;
	QMap<int, QTime> timeMap = {
		{0, QTime::fromString("6am", "hAP")},
		{1, QTime::fromString("7am", "hAP")},
		{2, QTime::fromString("8am", "hAP")},
		{3, QTime::fromString("9am", "hAP")},
		{4, QTime::fromString("10am", "hAP")},
		{5, QTime::fromString("11am", "hAP")},
		{6, QTime::fromString("12pm", "hAP")},
		{7, QTime::fromString("1pm", "hAP")},
		{8, QTime::fromString("2pm", "hAP")},
		{9, QTime::fromString("3pm", "hAP")},
		{10, QTime::fromString("4pm", "hAP")},
		{11, QTime::fromString("5pm", "hAP")},
		{12, QTime::fromString("6pm", "hAP")},
		{13, QTime::fromString("7pm", "hAP")},
		{14, QTime::fromString("8pm", "hAP")},
		{15, QTime::fromString("9pm", "hAP")},
		{16, QTime::fromString("10pm", "hAP")},
		{17, QTime::fromString("11pm", "hAP")},
		{18, QTime::fromString("12am", "hAP")},
		{19, QTime::fromString("1am", "hAP")},
		{20, QTime::fromString("2am", "hAP")}
	};
	QTextEdit* startTimeText;
	QTextEdit* endTimeText;
	QTextEdit* seasonText;
	QTextEdit* weatherText;
	QTextEdit* locationText;

	void setupLayout();
	void populateFishLayout(const vector<Fish>& fishList);
    void deleteLayouts(QLayout* layout);

	BackgroundWidget* backgroundWidget;
	QVBoxLayout* fishLayout;
    QHBoxLayout* currentRowLayout;
    QHBoxLayout* closeButtonLayout;
	QLabel* fishLabel;
	QPixmap fishPixmap;
	QPushButton* closeButton;
	RangeSlider* rangeSlider;


private slots:
	void onFishClicked(QMouseEvent* event);
	void onLowerValueChanged(int lowerValue);
	void onUpperValueChanged(int upperValue);
	void populateFishByTimeInterval(const QTime& startTime, const QTime& endTime);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void on_closeButton_clicked();
	void on_lineEditWidget_textChanged(const QString& text);
	void handleDetailBoxButtonClicked(const string& name);
};
