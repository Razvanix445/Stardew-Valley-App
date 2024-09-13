#include "CreateUserWindow.h"

CreateUserWindow::CreateUserWindow(QWidget *parent, Service& service)
	: QDialog(parent), service(service)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	setAttribute(Qt::WA_StyledBackground, true);
	setFixedSize(400, 700);
}

void CreateUserWindow::setImageCache(QMap<QString, QPixmap> images) {
	imageCache = images;
}

void CreateUserWindow::setupLayout()
{
	QPixmap pixmap = imageCache.value("Vertical_Panel");
	BackgroundWidget* centralWidget = new BackgroundWidget(pixmap, this);

	QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
	mainLayout->setAlignment(Qt::AlignTop);

	// => TOP LAYOUT
	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->setContentsMargins(0, 4, 0, 0);
	topLayout->setAlignment(Qt::AlignRight);

	// Close Button
	const QSize originalButtonSize(40, 40);
	const QSize hoveredButtonSize(50, 50);

	HoverButton* closeButton = new HoverButton(this, originalButtonSize, hoveredButtonSize);
	QPixmap closeButtonImage = imageCache.value("Uncheckmark");
	closeButton->setStyleSheet("background: transparent; border: none;");
	closeButton->setIcon(closeButtonImage);
	connect(closeButton, &HoverButton::clicked, this, &CreateUserWindow::close);
	// End

	topLayout->addWidget(closeButton);
	// <= END


	// => TITLE LAYOUT
	QHBoxLayout* titleLayout = new QHBoxLayout();
	titleLayout->setAlignment(Qt::AlignCenter);
	
	QLabel* titleLabel = new QLabel("Create User");
	titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
	titleLayout->addWidget(titleLabel);
	// <= END

	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(titleLayout);

	// Set the main layout on the central widget
	centralWidget->setLayout(mainLayout);

	// Set the central widget as the layout's main widget
	QVBoxLayout* dialogLayout = new QVBoxLayout(this);
	dialogLayout->addWidget(centralWidget);

	setLayout(dialogLayout);
}

CreateUserWindow::~CreateUserWindow()
{}
