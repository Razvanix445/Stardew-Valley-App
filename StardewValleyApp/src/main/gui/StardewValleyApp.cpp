#include "StardewValleyApp.h"
#include "../model/Fish.h"
#include <QPixmap>
#include <QIcon>
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

StardewValleyApp::StardewValleyApp(QWidget *parent, const string& databasePath, const string& username)
    : QMainWindow(parent), fishRepository(databasePath), username(username)
{
    ui.setupUi(this);

    // Check if tableView exists
    QTableView* tableView = findChild<QTableView*>("tableView");
    if (!tableView) {
        std::cerr << "Error: tableView not found!" << std::endl;
        return;
    }

    // Setup the model and populate it
    setupModel();

    // Set the model to the tableView
    tableView->setModel(model);
}

StardewValleyApp::~StardewValleyApp()
{
    delete model;
}

//std::vector<char> readImageFile(const std::string& relativePath) {
//    std::ifstream file(relativePath, std::ios::binary | std::ios::ate);
//    std::vector<char> image;
//
//    if (file.is_open()) {
//        std::streamsize size = file.tellg();
//        file.seekg(0, std::ios::beg);
//
//        image.resize(size);
//        if (file.read(image.data(), size)) {
//            // Successfully read the file
//        }
//        else {
//            // Handle the error, e.g., clear the vector or throw an exception
//            image.clear();
//        }
//    }
//    else {
//        // Handle the error, e.g., clear the vector or throw an exception
//    }
//
//    return image;
//}

void StardewValleyApp::setupModel()
{
    //vector<char> image = readImageFile("src/resources/images/Void_Salmon.png");

    //if (!image.empty()) {
    //    // Save the image to the database
    //    fishRepository.saveImage(43, image);
    //    qDebug() << "image saved!";
    //}
    //else {
    //    qDebug() << "image not saved!";
    //}



    std::vector<Fish> allFish = fishRepository.findAll(username);

    model = new QStandardItemModel(allFish.size(), 9);

    // Set up the table headers
    model->setHorizontalHeaderLabels({ "Name", "Season", "Weather", "Location", "Start Catching Hour", "End Catching Hour", "Difficulty", "Caught", "Image" });

    // Populate the model with fish data
    for (int row = 0; row < allFish.size(); ++row) {
        const Fish& fish = allFish[row];
        model->setItem(row, 0, new QStandardItem(QString::fromStdString(fish.getName())));
        model->setItem(row, 1, new QStandardItem(join(fish.getSeason())));
        model->setItem(row, 2, new QStandardItem(join(fish.getWeather())));
        model->setItem(row, 3, new QStandardItem(join(fish.getLocation())));
        model->setItem(row, 4, new QStandardItem(QString::number(fish.getStartCatchingHour())));
        model->setItem(row, 5, new QStandardItem(QString::number(fish.getEndCatchingHour())));
        model->setItem(row, 6, new QStandardItem(QString::number(fish.getDifficulty())));
        model->setItem(row, 7, new QStandardItem(fish.getIsCaught() ? "Yes" : "No"));

        // Convert the image data to QPixmap
        QPixmap pixmap;
        pixmap.loadFromData(reinterpret_cast<const uchar*>(fish.getImage().data()), fish.getImage().size());

        // Create a QStandardItem with the QPixmap as icon
        QStandardItem* imageItem = new QStandardItem();
        imageItem->setIcon(QIcon(pixmap));
        model->setItem(row, 8, imageItem);
    }
}

QString StardewValleyApp::join(const std::vector<std::string>& vec)
{
    QStringList list;
    for (const auto& item : vec) {
        list.append(QString::fromStdString(item));
    }
    return list.join(", ");
}