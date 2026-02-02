#pragma once

#include <QGridLayout>
#include <QStandardItemModel>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

#include <atomic>
#include <vector>
#include <thread>

#include "ui/page/IPage.hpp"
#include "FindValueType.hpp"
#include "process/memory/LinuxSmartMemoryFinder.hpp"
#include "process/analysis/LinuxProcessMemoryParser.hpp"
#include "process/memory/LinuxSmartMemoryWriter.hpp"
#include "ui/page/ListProcessPage.hpp"
#include "process/detail/LinuxProcessFinder.hpp"

class MainWindow;

namespace ui::page {

class ProcessDetailPage final : public IPage {
private:
    MainWindow& _window;
    QWidget* _page;
    QTableView* _table;
    int _selectProcessPid;
    QGridLayout* _gridLayout;
    QPushButton* _findValueButton;
    QPushButton* _changeValueButton;
    QPushButton* _backButton;
    QLineEdit* _findValueEdit;
    QLineEdit* _changeValueEdit;
    QLineEdit* _selectValueAdressEdit;
    QComboBox* _selectTypeValueBox;
    QLabel* _selectValueAddressLabel;
    QLabel* _selectValueLabel;
    process::memory::LinuxSmartMemoryFinder _memoryFinder;
    process::memory::LinuxSmartMemoryWriter _memoryWriter;
    QStandardItemModel* _findedValuesModel;
    ssize_t _selectRow;
    std::atomic<bool> _isSearching;

public:
    ProcessDetailPage(int pid, MainWindow& window);
    ~ProcessDetailPage();

public:
    void show() override;
    void hide() override;

private slots:
    void findValue();
    void changeValue();
    void setActiveRow(const QModelIndex &curr, const QModelIndex &prev);
    void backPage();

private:
    void clearChangeEdits();
};

}
