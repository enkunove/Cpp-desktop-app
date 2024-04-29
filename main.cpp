#include <QApplication>
#include "calendarapp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CalendarApp window;
    window.setWindowTitle("Calendar");

    QString style = "QPushButton {"
                    "    background-color: #79c2d0; /* Green background color */"
                    "    color: white; /* White text color */"
                    "    font-size: 20px; /* Font size */"
                    "    border: 2px solid #5585b5; /* color border */"
                    "    border-radius: 10px; /* Rounded corners */"
                    "    padding: 10px 20px; /* Padding inside the button */"
                    "    outline: none; /* Remove button highlight on click */"
                    "}"

                    "QPushButton:hover {"
                    "    background-color: #53a8b6; /* Dark green color on hover */"
                    "}"

                    "QLabel {"
                    "    font-size: 24px; /* Font size */"
                    "    color: #333; /* Text color */"
                    "}"

                    "CalendarApp {"
                    "    background-color: #bbe4e9; /* Background color of the window */"
                    "}";

    window.setStyleSheet(style);

    window.showMaximized();

    return app.exec();
}

