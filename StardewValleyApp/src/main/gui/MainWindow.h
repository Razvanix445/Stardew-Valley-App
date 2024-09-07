#pragma once

#include <QtWidgets/QMainWindow>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QFontMetrics>
#include <QEvent>
#include <string>
#include "../service/Service.h"
#include "../utils/ClickableLabel.h"
#include "../utils/BackgroundWidget.h"
#include "FishManagementController.h"

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent, const string& databasePath, Service& service, const string& username);
    ~MainWindow() override;

    void setupLayout();
    void setImageCache(QMap<QString, QPixmap> images);

private:

    FishManagementController* fishController;

    /*
    * Creates a clickable label with the given image data and description.
    */
    QWidget* createClickableLabel(const QPixmap& imageData, const QString& description);

    /*
    * Slot for the first image container click event.
    */
    void onFirstImageClicked();

    /*
    * Slot for the second image container click event.
    */
    void onSecondImageClicked();

    /*
    * Slot for the third image container click event.
    */
    void onThirdImageClicked();

    Service& service;
    QWidget* centralWidget;
    string databasePath;
    string username;

    QPixmap pixmap;
    QMap<QString, QPixmap> imageCache;
};