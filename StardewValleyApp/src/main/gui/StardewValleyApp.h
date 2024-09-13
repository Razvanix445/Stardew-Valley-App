#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableView>
#include <QtGui/QStandardItemModel>
#include <string>
#include "ui_StardewValleyApp.h"
#include "../repository/FishDBRepository.h"

using namespace std;

class StardewValleyApp : public QMainWindow
{
    Q_OBJECT

public:
    StardewValleyApp(QWidget *parent = nullptr, const string& databasePath = "", const long userId = 0);
    ~StardewValleyApp();

private:
    Ui::StardewValleyAppClass ui;
    QStandardItemModel* model;
    FishDBRepository fishRepository;
    string databasePath;
    const long userId;

    void setupModel();
    QString join(const vector<std::string>& vec);
};