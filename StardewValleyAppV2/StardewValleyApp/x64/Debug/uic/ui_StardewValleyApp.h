/********************************************************************************
** Form generated from reading UI file 'StardewValleyApp.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STARDEWVALLEYAPP_H
#define UI_STARDEWVALLEYAPP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StardewValleyAppClass
{
public:
    QWidget *centralWidget;
    QTableView *tableView;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *StardewValleyAppClass)
    {
        if (StardewValleyAppClass->objectName().isEmpty())
            StardewValleyAppClass->setObjectName("StardewValleyAppClass");
        StardewValleyAppClass->resize(600, 400);
        centralWidget = new QWidget(StardewValleyAppClass);
        centralWidget->setObjectName("centralWidget");
        tableView = new QTableView(centralWidget);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(0, 0, 256, 192));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(70, 290, 75, 24));
        StardewValleyAppClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(StardewValleyAppClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        StardewValleyAppClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(StardewValleyAppClass);
        mainToolBar->setObjectName("mainToolBar");
        StardewValleyAppClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(StardewValleyAppClass);
        statusBar->setObjectName("statusBar");
        StardewValleyAppClass->setStatusBar(statusBar);

        retranslateUi(StardewValleyAppClass);

        QMetaObject::connectSlotsByName(StardewValleyAppClass);
    } // setupUi

    void retranslateUi(QMainWindow *StardewValleyAppClass)
    {
        StardewValleyAppClass->setWindowTitle(QCoreApplication::translate("StardewValleyAppClass", "StardewValleyApp", nullptr));
        pushButton->setText(QCoreApplication::translate("StardewValleyAppClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StardewValleyAppClass: public Ui_StardewValleyAppClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STARDEWVALLEYAPP_H
