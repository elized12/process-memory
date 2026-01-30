#pragma once

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <vector>
#include <QObject>

#include "ui/page/IPage.hpp"
#include "ui/page/ProcessDetailPage.hpp"
#include "process/detail/LinuxProcessFinder.hpp"

class MainWindow;

namespace ui::page {

class ListProcessPage final : public IPage {
private:
    MainWindow& _window;
    QWidget* _page;
    QVBoxLayout* _vbox;
    QPushButton* _buttonSelectProcess;
    QPushButton* _buttonUpdateListProcesses;
    QTableView* _tableProcesses;
    QStandardItemModel* _modelProcesses;
    std::unique_ptr<process::detail::LinuxProcessFinder> _finder;
    int _selectProcessPid;

public:
    ListProcessPage(MainWindow& window, std::unique_ptr<process::detail::LinuxProcessFinder> finder);
    ~ListProcessPage();

public:
    void show() override;
    void hide() override;

public slots:
    void selectProcess();

private:
    void changeModelProcesses(const std::vector<process::ProcessInfo>& processes);
};

}
