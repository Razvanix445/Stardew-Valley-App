#include "gui/MainWindow.h"
#include "repository/FishDBRepository.h"
#include "service/Service.h"
#include <QtWidgets/QApplication>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <QFontDatabase>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);


    // => ERROR LOGGING
    std::ofstream logFile("error.log");
    std::streambuf* originalCerr = std::cerr.rdbuf(logFile.rdbuf());
    std::cerr << "Here enter the errors!" << std::endl;
    // <= END


    // => DATABASE EXISTENCE ASSURING
    string databasePath = "stardewValleyDatabase.db";

    if (!std::filesystem::exists(databasePath)) {
        std::cerr << "Database file does not exist: " << databasePath << std::endl;
        return -1;
    }

    string username = "viorel";
    // <= END


    // => FONT LOADING
    QFontDatabase fontDatabase;
    int fontId = fontDatabase.addApplicationFont("fonts/stardew-valley-stonks.ttf");
    QString fontFamily = fontDatabase.applicationFontFamilies(fontId).at(0);
    QFont customFont(fontFamily);
    a.setFont(customFont);
    // <= END


    // => SERVICE INITIALIZATION
    FishDBRepository fishRepository(databasePath);
    Service service(fishRepository);
    // <= END


    // => OPENING MAIN WINDOW
    MainWindow w(nullptr, databasePath, service, username);
    w.setWindowTitle("Stardew Valley Fish Mock Database Demonstration");
    w.showMaximized();
    w.show();
    // <= END


    logFile.close();
    std::cerr.rdbuf(originalCerr);

    return a.exec();
}