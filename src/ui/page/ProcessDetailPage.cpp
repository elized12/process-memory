#include "ui/page/ProcessDetailPage.hpp"
#include "mainwindow.h"

namespace ui::page {

ProcessDetailPage::ProcessDetailPage(
        int selectProcessPid,
        MainWindow& window,
        std::unique_ptr<process::analysis::LinuxProcessMemoryParser> memoryParser
) :
    _selectProcessPid(selectProcessPid),
    _window(window),
    _memoryParser(std::move(memoryParser)),
    _content(new QWidget()),
    _hbox(new QHBoxLayout()),
    _grid(new QGridLayout()),
    _buttonFindAddresses(new QPushButton("Поиск")),
    _searchValue(new QLineEdit()),
    _selectType(new QComboBox()),
    _table(new QTableView()),
    _selectValue(new QLineEdit()),
    _selectValueLabel(new QLabel("Выбранное значение")),
    _selectAddressValueLabel(new QLabel("Адрес")),
    _selectAddressValue(new QLineEdit()),
    _changeValue(new QPushButton("Изменить")),
    _modelMemory(new QStandardItemModel()),
    _reader(process::memory::LinuxMemoryRegionFilter()),
    _selectMemoryBlock(new QComboBox()),
    tableTest(new ui::widget::LazyLoaderTable())

{
    this->_content->setLayout(this->_grid);

    this->_grid->setAlignment(Qt::AlignmentFlag::AlignTop);

    this->_grid->addWidget(this->_searchValue, 0, 0, 1, 5);
    this->_grid->addWidget(this->_buttonFindAddresses, 0, 6, 1, 1);

    this->_selectType->addItems({"Неизвестен", "int8", "int16", "int32", "int64", "double", "float", "string"});
    this->_grid->addWidget(this->_selectType, 0, 5, 1, 1);

    this->_grid->addWidget(this->_selectMemoryBlock, 2, 0, 1, 7);

    this->_selectValue->setReadOnly(true);
    this->_grid->addWidget(this->_selectValue, 1, 1, 1, 2);

    this->_selectAddressValue->setReadOnly(true);
    this->_grid->addWidget(this->_selectAddressValue, 1, 4, 1, 1);

    this->_grid->addWidget(this->_selectValueLabel, 1, 0, 1, 1);
    this->_grid->addWidget(this->_selectAddressValueLabel, 1, 3, 1, 1);
    this->_grid->addWidget(this->_changeValue, 1, 5, 1, 2);

    this->_table->verticalHeader()->setVisible(false);
    this->_table->horizontalHeader()->setStretchLastSection(false);
    this->_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    this->_table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    this->_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    this->_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    this->_grid->addWidget(this->_table, 3, 0, 1, 7);

    this->_memoryBlocks = this->_memoryParser->parseProcess(this->_selectProcessPid);
}

void ProcessDetailPage::show() {
    this->_content->setVisible(true);
    this->_content->setEnabled(true);

    this->_window.setCentralWidget(this->_content);
}

void ProcessDetailPage::hide() {
    this->_content->setVisible(false);
    this->_content->setEnabled(false);

    this->_window.setCentralWidget(nullptr);
}

}
