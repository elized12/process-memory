#pragma once

#include <QMainWindow>

#include "ui/page/PageType.hpp"
#include "process/detail/LinuxProcessFinder.hpp"
#include "ui/page/ListProcessPage.hpp"
#include "ui/page/ProcessDetailPage.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(logger::Logger& logger, QWidget *parent = nullptr);
    ~MainWindow();

public:
    void setPage(const ui::page::PageType& type);
    void setPage(ui::page::IPage* page);
    logger::Logger& getLogger();

private:
    Ui::MainWindow *ui;
    ui::page::IPage* _page;
    logger::Logger& _logger;
    int _selectProcessPid;
};
