#pragma once

#include "../model/Fish.h"
#include "../repository/FishDBRepository.h"
#include "../utils/BackgroundWidget.h"
#include "../utils/CustomButton.h"
#include "../utils/HoverButton.h"
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
#include <QDebug>
#include <string>
#include <vector>
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


private slots:
	void onFishClicked(QMouseEvent* event);
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void on_closeButton_clicked();
	void on_lineEditWidget_textChanged(const QString& text);
	void handleDetailBoxButtonClicked(const string& name);
};
