#include <QApplication>
#include "calendarapp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CalendarApp window;

    window.setWindowTitle("Investment tracker");

    QString style = "QPushButton {"
                    "    background-color: #FAEED1;"
                    "    color: 322C2B;"
                    "    font-size: 36px;"
                    "    border: 2px solid #322C2B;"
                    "    border-radius: 10px; "
                    "    padding: 10px 20px; "
                    "    outline: none; "
                    "}"

                    "QPushButton:hover {"
                    "    background-color: #DED0B6;"
                    "}"
                    "QPushButton#b {font-size: 16px; color: #ffffff; background-color: #ff5733; border: none; padding: 8px 16px;border-radius: 3px;} QPushButton#b:hover {background-color: #be3e23; }"
                    "QLabel {"
                    "    font-size: 26px; "
                    "    color: #322C2B; "
                    "}"

                    "CalendarApp {"
                    "    background-color: #FDF7E4; "
                    "}";

    window.setStyleSheet(style);
    window.showMaximized();

    return app.exec();
}

