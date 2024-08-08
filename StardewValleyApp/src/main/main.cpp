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

void saveImagePng(int id, string& filePath, FishDBRepository& fishRepository) {
    // Open the file in binary mode
    std::ifstream file(filePath, std::ios::binary);

    // Ensure the file is open
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // Get the file size
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Create a vector to hold the file contents
    std::vector<char> buffer(fileSize);

    // Read the file into the buffer
    if (!file.read(buffer.data(), fileSize)) {
        throw std::runtime_error("Error reading file");
    }

    // Close the file
    file.close();

    vector<char> imageBuffer = buffer;

    fishRepository.saveImage(id, imageBuffer);
}

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





    /*string filePath = "src/resources/images/Son_of_Crimsonfish.png";
    saveImagePng(57, filePath, fishRepository);

    string filePath2 = "src/resources/images/Ms_Angler.png";
    saveImagePng(58, filePath2, fishRepository);

    string filePath3 = "src/resources/images/Legend_II.png";
    saveImagePng(59, filePath3, fishRepository);

    string filePath4 = "src/resources/images/Glacierfish_Jr.png";
    saveImagePng(60, filePath4, fishRepository);

    string filePath5 = "src/resources/images/Radioactive_Carp.png";
    saveImagePng(61, filePath5, fishRepository);*/




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