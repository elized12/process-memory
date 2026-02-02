#include "ui/page/ProcessDetailPage.hpp"
#include "mainwindow.h"

namespace ui::page {

ProcessDetailPage::ProcessDetailPage(int pid, MainWindow& window) :
    _window(window),
    _page(new QWidget(&window)),
    _selectProcessPid(pid),
    _table(new QTableView()),
    _gridLayout(new QGridLayout()),
    _findValueButton(new QPushButton("Поиск")),
    _changeValueButton(new QPushButton("Изменить")),
    _findValueEdit(new QLineEdit()),
    _selectTypeValueBox(new QComboBox()),
    _changeValueEdit(new QLineEdit()),
    _selectValueAdressEdit(new QLineEdit()),
    _selectValueAddressLabel(new QLabel("Адрес")),
    _selectValueLabel(new QLabel("Значение")),
    _findedValuesModel(new QStandardItemModel()),
    _backButton(new QPushButton("Назад")),
    _isSearching(false),
    _memoryWriter(process::memory::LinuxMemoryWriter(this->_window.getLogger())),
    _memoryFinder(process::memory::LinuxMemoryFinder(process::memory::LinuxMemoryRegionFilter(), this->_window.getLogger()))
{
    this->_page->setLayout(this->_gridLayout);

    this->_gridLayout->addWidget(this->_findValueEdit, 0, 0, 1, 4);

    this->_selectTypeValueBox->addItem("uint8",  QVariant::fromValue(FindValueType::uint8));
    this->_selectTypeValueBox->addItem("uint16", QVariant::fromValue(FindValueType::uint16));
    this->_selectTypeValueBox->addItem("uint32", QVariant::fromValue(FindValueType::uint32));
    this->_selectTypeValueBox->addItem("uint64", QVariant::fromValue(FindValueType::uint64));
    this->_selectTypeValueBox->addItem("int8", QVariant::fromValue(FindValueType::int8));
    this->_selectTypeValueBox->addItem("int16", QVariant::fromValue(FindValueType::int16));
    this->_selectTypeValueBox->addItem("int32", QVariant::fromValue(FindValueType::int32));
    this->_selectTypeValueBox->addItem("int64", QVariant::fromValue(FindValueType::int64));
    this->_selectTypeValueBox->addItem("float8",  QVariant::fromValue(FindValueType::float8));
    this->_selectTypeValueBox->addItem("double8", QVariant::fromValue(FindValueType::double8));
    this->_selectTypeValueBox->addItem("string", QVariant::fromValue(FindValueType::string));

    this->_gridLayout->addWidget(this->_selectTypeValueBox, 0, 4, 1, 1);

    this->connect(this->_findValueButton, &QPushButton::clicked, this, &ProcessDetailPage::findValue);
    this->_gridLayout->addWidget(this->_findValueButton, 0, 5, 1, 2);

    this->_selectValueAdressEdit->setReadOnly(true);
    this->_gridLayout->addWidget(this->_selectValueAdressEdit, 1, 1, 1, 1);

    this->_gridLayout->addWidget(this->_selectValueAddressLabel, 1, 0, 1, 1);
    this->_gridLayout->addWidget(this->_selectValueLabel, 1, 2, 1, 1);

    this->_changeValueEdit->setReadOnly(true);
    this->_gridLayout->addWidget(this->_changeValueEdit, 1, 3, 1, 1);

    this->connect(this->_changeValueButton, &QPushButton::clicked, this, &ProcessDetailPage::changeValue);
    this->_gridLayout->addWidget(this->_changeValueButton, 1, 4, 1, 3);

    this->_table->horizontalHeader()->setStretchLastSection(false);
    this->_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    this->_table->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    this->_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    this->_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    this->_gridLayout->addWidget(this->_table, 3, 0, 1, 7);

    this->connect(this->_backButton, &QPushButton::clicked, this, &ProcessDetailPage::backPage);
    this->_gridLayout->addWidget(this->_backButton, 4, 0, 1, 7);
}


void ProcessDetailPage::show() {
    this->_page->setVisible(true);
    this->_page->setEnabled(true);

    this->_window.setCentralWidget(this->_page);
}

void ProcessDetailPage::hide() {
    this->_page->setVisible(false);
    this->_page->setEnabled(false);

    this->_window.setCentralWidget(nullptr);
}

ProcessDetailPage::~ProcessDetailPage() {
    this->hide();
}

void ProcessDetailPage::findValue() {
    if (this->_isSearching.load()) {
        QMessageBox::information(&this->_window, "Поиск уже запущен", "Подождите, поиск уже идет...");
        return;
    }

    this->clearChangeEdits();
    this->_isSearching.store(true);
    this->_findValueButton->setEnabled(false);
    this->_backButton->setEnabled(false);

    std::thread searchValues([this]() {
        const FindValueType selectType = qvariant_cast<FindValueType>(this->_selectTypeValueBox->currentData());
        const process::memory::TypeValue target = process::memory::parseTargetValue(this->_findValueEdit->text(), selectType);

        process::analysis::LinuxProcessMemoryParser parser;

        std::vector<process::memory::MemoryRecord> findedValues = this->_memoryFinder.findValues(
            this->_selectProcessPid,
            parser.parseProcess(this->_selectProcessPid),
            target,
            selectType
        );

        QMetaObject::invokeMethod(this, [this, findedValues, selectType] () {
            this->_findedValuesModel->clear();
            this->_findedValuesModel->setHorizontalHeaderLabels({
                "Адрес",
                "Значение"
            });

            for (const process::memory::MemoryRecord& value : findedValues) {
                this->_findedValuesModel->appendRow({
                    new QStandardItem(QString("0x%1").arg(value.address, 0, 16).toUpper()),
                    new QStandardItem(process::memory::variantToString(value.value, selectType))
                });
            }

            this->_table->setModel(this->_findedValuesModel);

            this->connect(
                this->_table->selectionModel(),
                &QItemSelectionModel::currentRowChanged,
                this,
                &ProcessDetailPage::setActiveRow
            );

            this->_isSearching.store(false);
            this->_findValueButton->setEnabled(true);
            this->_backButton->setEnabled(true);
        });
    });

    searchValues.detach();
}

void ProcessDetailPage::setActiveRow(const QModelIndex &curr, const QModelIndex &prev) {
    this->_changeValueEdit->setReadOnly(false);

    this->_selectRow = curr.row();

    this->_selectValueAdressEdit->setText(this->_findedValuesModel->data(this->_findedValuesModel->index(this->_selectRow, 0)).toString());
    this->_changeValueEdit->setText(this->_findedValuesModel->data(this->_findedValuesModel->index(this->_selectRow, 1)).toString());
}

void ProcessDetailPage::changeValue() {
    if (
            this->_selectValueAdressEdit->text().isEmpty() ||
            this->_changeValueEdit->text().isEmpty()
    ) {
        QMessageBox::critical(
            &this->_window,
            "Ошибка записи памяти",
            "Адрес и значение должны быть заполнены!"
        );
        return;
    }

    std::uint64_t address = this->_selectValueAdressEdit->text().toULongLong(nullptr, 16);

    const FindValueType selectType = qvariant_cast<FindValueType>(this->_selectTypeValueBox->currentData());
    const process::memory::TypeValue value = process::memory::parseTargetValue(this->_changeValueEdit->text(), selectType);

    try {
        this->_memoryWriter.write(this->_selectProcessPid, address, value, selectType);
        this->_findedValuesModel->item(this->_selectRow, 1)->setText(this->_changeValueEdit->text());
    }
    catch (const std::exception& ex) {
        QMessageBox::critical(&this->_window, "Ошибка записи памяти", ex.what());
    }
}

void ProcessDetailPage::clearChangeEdits() {
    this->_changeValueEdit->clear();
    this->_selectValueAdressEdit->clear();
}

void ProcessDetailPage::backPage() {
    std::unique_ptr<process::detail::LinuxProcessFinder> finder = std::make_unique<process::detail::LinuxProcessFinder>(this->_window.getLogger());
    ui::page::ListProcessPage* nextPage = new ui::page::ListProcessPage(this->_window, std::move(finder));

    this->_window.setPage(nextPage);
}

}
