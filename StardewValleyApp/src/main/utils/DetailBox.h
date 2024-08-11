#pragma once

#include "BackgroundWidget.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <string>
#include <QScrollArea>
#include <vector>

using namespace std;

class DetailBox : public BackgroundWidget {
    Q_OBJECT

public:
    DetailBox(const string& labelText, const vector<char>& imageData, QWidget* parent = nullptr)
        : BackgroundWidget(imageData, parent), labelText(labelText) {
        
        setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


        setAttribute(Qt::WA_TranslucentBackground, true);
        setAttribute(Qt::WA_StyledBackground, true);

        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        QLabel* label = new QLabel(QString::fromStdString(labelText), this);
        label->setStyleSheet("background: transparent;"
            "padding: 10 10 10 10; ");
        mainLayout->addWidget(label);

        QWidget* containerWidget = new QWidget(this);
        containerWidget->setStyleSheet("background: transparent;");
        QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget);
        containerLayout->setAlignment(Qt::AlignCenter);
        containerWidget->setLayout(containerLayout);
        containerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QScrollArea* scrollArea = new QScrollArea(this);
        scrollArea->setStyleSheet("background: transparent;");
        scrollArea->setWidget(containerWidget);
        scrollArea->setWidgetResizable(true);
        scrollArea->setMaximumHeight(200);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        mainLayout->addWidget(scrollArea);

        setLayout(mainLayout);
    }

    void addButton(const string& name) {
        QPushButton* button = new QPushButton(QString::fromStdString(name), this);
        button->setStyleSheet("QPushButton {"
        "    background: transparent;"
            "    color: black;"
            "    padding: 5px;"
            "}"
            "QPushButton:hover {"
            "    font-weight: bold;"
            "}"
            "QPushButton:pressed {"
            "    color: grey;"
            "}");
        connect(button, &QPushButton::clicked, this, [this, name]() {
            clickedButtonName = name;
            emit buttonClicked(name);
            qDebug() << "Button clicked: " << QString::fromStdString(name);
        });
        QWidget* containerWidget = findChild<QScrollArea*>()->widget();
        if (containerWidget) {
            containerWidget->layout()->addWidget(button);
        }
    }

    void leaveEvent(QEvent* event) override {
        QWidget::leaveEvent(event);
        hide();
    }

    const string& getClickedButtonName() const {
		return clickedButtonName;
	}

signals:
    void buttonClicked(const string& name);

private:
    string clickedButtonName;
    const string& labelText;
};