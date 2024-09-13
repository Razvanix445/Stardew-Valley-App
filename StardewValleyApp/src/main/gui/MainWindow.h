#pragma once

#include <QtWidgets/QMainWindow>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QFontMetrics>
#include <QEvent>
#include <string>
#include "../service/Service.h"
#include "../utils/HoverButton.h"
#include "../utils/ClickableLabel.h"
#include "../utils/BackgroundWidget.h"
#include "FishManagementController.h"

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent, const string& databasePath, Service& service, const long userId);
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

    /*
    * Slot for the return button click event to open the UserAccountsWindow.
    */
    void onReturnButtonClicked();

    Service& service;
    QWidget* centralWidget;
    QHBoxLayout* topLayout;
    string databasePath;
    long userId;

    // Dragging window variables
    bool isDragging;
    QPoint dragStartPosition;
    QWidget* dragHandle;

    QPixmap pixmap;
    QMap<QString, QPixmap> imageCache;

private slots:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void returnToUserAccountsWindow();
};