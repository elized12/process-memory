#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(logger::Logger& logger, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    _logger(logger),
    _page(nullptr),
    _selectProcessPid(0)
{
    ui->setupUi(this);

    std::unique_ptr<process::analysis::LinuxProcessMemoryParser> memoryParser =std::make_unique<process::analysis::LinuxProcessMemoryParser>();
    this->setPage(ui::page::PageType::ProcessSelect);
}


void MainWindow::setPage(const ui::page::PageType& type) {
    delete this->_page;

    switch (type) {
    case ui::page::PageType::ProcessSelect: {
        std::unique_ptr<process::detail::LinuxProcessFinder> finder = std::make_unique<process::detail::LinuxProcessFinder>(this->_logger);
        this->_page = new ui::page::ListProcessPage(*this, std::move(finder));

        break;
    }
    default:
        break;
    }

    this->_page->show();
}

void MainWindow::setPage(ui::page::IPage* page) {
    delete this->_page;
    this->_page = page;
    this->_page->show();
}


MainWindow::~MainWindow()
{
    delete this->ui;
}


