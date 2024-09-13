#pragma once

#include <QtWidgets/QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>
#include "../service/Service.h"
#include "../utils/HoverButton.h"
#include "../utils/BackgroundWidget.h"

class CreateUserWindow  : public QDialog
{
	Q_OBJECT

public:
	CreateUserWindow(QWidget* parent, Service& service);
	~CreateUserWindow();

	void setupLayout();
	void setImageCache(QMap<QString, QPixmap> images);

private:
	const Service& service;

	QMap<QString, QPixmap> imageCache;
};
