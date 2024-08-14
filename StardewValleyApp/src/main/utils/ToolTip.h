#pragma once

#include "BackgroundWidget.h"
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QVBoxLayout>

class ToolTip : public BackgroundWidget {
	Q_OBJECT

public:
	explicit ToolTip(vector<char>& imageData, QWidget* parent = nullptr)
		: BackgroundWidget(imageData, parent), imageData(imageData) {
		setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
		setAttribute(Qt::WA_TranslucentBackground, true);
		setAttribute(Qt::WA_StyledBackground, true);
		setAttribute(Qt::WA_ShowWithoutActivating);
		setCornerRadius(0);

		textLabel = new QLabel(this);

		// => ADDING TO MAIN LAYOUT
		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setAlignment(Qt::AlignCenter);
		layout->addWidget(textLabel);
		// <= END

		setLayout(layout);
	}

	void setText(const QString& text) {
		textLabel->setText(text);
		textLabel->setWordWrap(true);
		textLabel->setStyleSheet("font-size: 10px;");
		textLabel->adjustSize();
		adjustSize();
	}

private:
	QLabel* textLabel;
	QPixmap backgroundPixmap;
	vector<char> imageData;
};