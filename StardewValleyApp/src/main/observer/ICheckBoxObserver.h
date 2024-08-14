#pragma once

#include <QObject>

class ICheckBoxObserver {
public:
    virtual ~ICheckBoxObserver() = default;
    virtual void onCheckBoxStateChanged(bool checked) = 0;
};