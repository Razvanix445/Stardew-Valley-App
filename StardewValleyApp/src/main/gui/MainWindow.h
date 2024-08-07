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

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent, const string& databasePath, Service& service, const string& username);
    ~MainWindow() override;

private:
    QWidget* createClickableLabel(const QString& imagePath, const QString& description);

    void onFirstImageClicked();
    void onSecondImageClicked();
    void onThirdImageClicked();

    Service& service;
    QWidget* centralWidget;
    string databasePath;
    string username;
};