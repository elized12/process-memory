#include "ui/page/ListProcessPage.hpp"
#include "mainwindow.h"

namespace ui::page {

ListProcessPage::ListProcessPage(MainWindow& window, std::unique_ptr<process::detail::LinuxProcessFinder> finder) :
    _window(window),
    _finder(std::move(finder)),
    _modelProcesses(nullptr),
    _selectProcessPid(0)
{
    this->_vbox = new QVBoxLayout();

    this->_page = new QWidget(&this->_window);
    this->_page->setLayout(this->_vbox);

    this->_tableProcesses = new QTableView();
    this->_tableProcesses->horizontalHeader()->setStretchLastSection(false);
    this->_tableProcesses->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    this->_tableProcesses->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    this->_tableProcesses->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    this->_tableProcesses->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    this->_vbox->addWidget(this->_tableProcesses);

    this->_buttonUpdateListProcesses = new QPushButton("Обновить список процессов");
    this->_vbox->addWidget(this->_buttonUpdateListProcesses);
    this->connect(this->_buttonUpdateListProcesses, &QPushButton::clicked, this, [this]() {
        std::vector<process::ProcessInfo> processes = this->_finder->getProcesses();
        this->changeModelProcesses(processes);
    });

    this->_buttonSelectProcess = new QPushButton("Выбрать процесс");
    this->connect(this->_buttonSelectProcess, &QPushButton::clicked, this, &ListProcessPage::selectProcess);
    this->_vbox->addWidget(this->_buttonSelectProcess);
}

void ListProcessPage::selectProcess() {
     QModelIndexList selectedRows = this->_tableProcesses->selectionModel()->selectedRows();
     if (selectedRows.isEmpty()) {
           QMessageBox::information(&this->_window, "Ошибка", "Выберите процесс");
           return;
     }

     int selectRow = selectedRows[0].row();
     this->_selectProcessPid = this->_modelProcesses->item(selectRow, 0)->text().toInt();

    ui::page::ProcessDetailPage* nextPage = new ui::page::ProcessDetailPage(this->_selectProcessPid, this->_window);

    this->_window.setPage(nextPage);
}

void ListProcessPage::show() {
    this->_page->setVisible(true);
    this->_page->setEnabled(true);

    this->_window.setCentralWidget(this->_page);

    std::vector<process::ProcessInfo> processes = this->_finder->getProcesses();
    this->changeModelProcesses(processes);
}

void ListProcessPage::hide() {
    this->_page->setVisible(false);
    this->_page->setEnabled(false);

    this->_window.setCentralWidget(nullptr);
}

ListProcessPage::~ListProcessPage() {
    this->hide();
}

void ListProcessPage::changeModelProcesses(const std::vector<process::ProcessInfo>& processes) {
    delete this->_modelProcesses;
    this->_modelProcesses = new QStandardItemModel(static_cast<int>(processes.size()), 3);
    this->_modelProcesses->setHorizontalHeaderLabels({
        "PID",
        "Name",
        "Status"
    });

    int row = 0;
    for (const auto& process : processes) {
        this->_modelProcesses->setItem(
            row,
            0,
            new QStandardItem(QString::number(process.pid))
        );

        this->_modelProcesses->setItem(
            row,
            1,
            new QStandardItem(QString::fromStdString(process.name))
        );

        this->_modelProcesses->setItem(
            row,
            2,
            new QStandardItem(QString::fromStdString(convertProcessStateToString(process.state)))
        );

        ++row;
    }

    this->_tableProcesses->setModel(this->_modelProcesses);
}

}
