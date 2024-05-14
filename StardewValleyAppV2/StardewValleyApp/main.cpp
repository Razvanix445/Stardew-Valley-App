#include "StardewValleyApp.h"
#include "FishDBRepository.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QTableView>
#include <QtGui/QStandardItemModel>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "sqlite3.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    std::ofstream logFile("error.log");
    std::streambuf* originalCerr = std::cerr.rdbuf(logFile.rdbuf());
    std::cerr << "Here enter the errors!" << std::endl;

    string databasePath = "stardewValleyDatabase.db";

    FishDBRepository fishRepository(databasePath);

    string fishName = "Pufferfish";

    /*Fish foundFish = fishRepository.findOneByName(fishName);

    if (foundFish.getId() != -1) {
        qDebug() << foundFish.toString();
    }
    else {
        qDebug() << "Fish with name: '" << fishName << "' not found.";
    }*/

    vector<Fish> allFish = fishRepository.findAll();

    if (allFish.size() != 0) {
        for (Fish fish : allFish)
            qDebug() << fish.toString();
    }
    else {
        qDebug() << "Fish not found!";
    }

    /*Fish fish("Sardine", "Spring", "Any", "Ocean", 6, 19, 30, 1);
    const long insertedId = fishRepository.save(fish);

    try {
        qDebug() << fishRepository.findOne(insertedId).toString();
    }
    catch (exception e) {
        qDebug() << "Fish could not be added.";
    }*/

    /*long idToDelete = 4;
    fishRepository.remove(idToDelete);
    qDebug() << "Fish successfully deleted.";*/

    /*Fish fish("Anchovy", "Summer", "Any", "Oceans", 6, 19, 30, 1);
    fishRepository.update(fish);
    qDebug() << "Fish successfully updated.";*/
    
    StardewValleyApp w;
    w.setWindowTitle("Stardew Valley Fish Mock Database Demonstation");

    QTableView* tableView = w.findChild<QTableView*>("tableView");
    if (!tableView) {
        cerr << "Error: tableView not found!" << endl;
        return -1;
    }

    // Create a QStandardItemModel to store the data
    QStandardItemModel* model = new QStandardItemModel(allFish.size(), 8, tableView);

    // Set up the table headers
    model->setHorizontalHeaderLabels({ "Name", "Season", "Weather", "Location", "Start Catching Hour", "End Catching Hour", "Difficulty", "Caught" });

    // Populate the model with fish data
    for (int row = 0; row < allFish.size(); ++row) {
        const Fish& fish = allFish[row];
        model->setItem(row, 0, new QStandardItem(QString::fromStdString(fish.getName())));
        model->setItem(row, 1, new QStandardItem(QString::fromStdString(fish.getSeason())));
        model->setItem(row, 2, new QStandardItem(QString::fromStdString(fish.getWeather())));
        model->setItem(row, 3, new QStandardItem(QString::fromStdString(fish.getLocation())));
        model->setItem(row, 4, new QStandardItem(QString::number(fish.getStartCatchingHour())));
        model->setItem(row, 5, new QStandardItem(QString::number(fish.getEndCatchingHour())));
        model->setItem(row, 6, new QStandardItem(QString::number(fish.getDifficulty())));
        model->setItem(row, 7, new QStandardItem(fish.getIsCaught() ? "Yes" : "No"));
    }

    // Set the model to the tableView
    tableView->setModel(model);

    w.show();

    logFile.close();
    std::cerr.rdbuf(originalCerr);

    return a.exec();
}
