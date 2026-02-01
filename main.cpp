#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <cstdlib>

#include <unistd.h>
#include <limits.h>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "webbis-process-analyzer_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    logger::Logger logger("/var/log/test.log");

    MainWindow window(logger);
    window.setWindowTitle("Process Reader");
    window.show();

    return a.exec();
}
