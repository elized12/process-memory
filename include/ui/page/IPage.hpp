#pragma once

#include <QObject>

namespace ui::page {

class IPage : public QObject {
public:
    IPage();
    ~IPage() = default;

public:
    virtual void show() = 0;
    virtual void hide() = 0;
};

}
