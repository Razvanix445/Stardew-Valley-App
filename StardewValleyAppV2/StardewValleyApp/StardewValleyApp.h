#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_StardewValleyApp.h"

class StardewValleyApp : public QMainWindow
{
    Q_OBJECT

public:
    StardewValleyApp(QWidget *parent = nullptr);
    ~StardewValleyApp();

private:
    Ui::StardewValleyAppClass ui;
};
