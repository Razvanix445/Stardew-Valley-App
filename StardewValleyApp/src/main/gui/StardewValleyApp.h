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
    StardewValleyApp(QWidget *parent = nullptr, const string& databasePath = "", const string& username = "");
    ~StardewValleyApp();

private:
    Ui::StardewValleyAppClass ui;
    QStandardItemModel* model;
    FishDBRepository fishRepository;
    string databasePath;
    string username;

    void setupModel();
    QString join(const vector<std::string>& vec);
};