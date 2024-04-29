#include "calendarapp.h"
#include "DayWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QApplication>

CalendarApp::CalendarApp(QWidget *parent) : QWidget(parent) {
    QGridLayout *MainLayout = new QGridLayout(this);
    QGridLayout *layout = new QGridLayout();
    QGridLayout *CalendarLayout = new QGridLayout();
    QHBoxLayout *ButtonsLayout = new QHBoxLayout();

    QPushButton *temp = new QPushButton(this);

    daysOfWeek << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sun" << "Sat";

    currentDate = QDate::currentDate();
    title = new QLabel(currentDate.toString("MMMM yyyy"), this);
    layout->addWidget(title, 0, 0, 1, 0, Qt::AlignCenter);
    layout->addLayout(CalendarLayout, 1, 0);
    layout->addLayout(ButtonsLayout, 2, 0);

    layout->setRowStretch(1, 4);

    for (int i = 0; i < 7; ++i) {
        QLabel *dayLabel = new QLabel(daysOfWeek[i], this);
        CalendarLayout->addWidget(dayLabel, 0, i, Qt::AlignCenter);
    }

    QDate firstDayOfMonth(currentDate.year(), currentDate.month(), 1);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 7; ++j) {
            int col = firstDayOfMonth.dayOfWeek() - 1;
            QPushButton *button = dayLabels[i][j] = new QPushButton("", this);
            button->setFixedSize(120, 120);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            CalendarLayout->addWidget(button, i+1, j, Qt::AlignCenter);
            QDate day = firstDayOfMonth.addDays(7 * i + j - col);
            button->setProperty("date", day);
            connect(button, &QPushButton::clicked, this, &CalendarApp::handleDayButtonClick);
        }
    }

    QPushButton *prevMonthButton = new QPushButton("<<", this);
    QPushButton *nextMonthButton = new QPushButton(">>", this);
    prevMonthButton->setFixedSize(80, 50);
    nextMonthButton->setFixedSize(80, 50);
    connect(prevMonthButton, &QPushButton::clicked, this, &CalendarApp::showPrevMonth);
    connect(nextMonthButton, &QPushButton::clicked, this, &CalendarApp::showNextMonth);
    ButtonsLayout->addWidget(prevMonthButton);
    ButtonsLayout->addWidget(nextMonthButton);
    updateCalendar();

    MainLayout->addLayout(layout, 0, 0);
    MainLayout->addWidget(temp, 0, 1);
}

void CalendarApp::handleDayButtonClick() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;
    QDate day = button->property("date").toDate();
    dayWidget = new DayWidget(day, this);
    dayWidget->show();
    dayWidget->setAttribute(Qt::WA_DeleteOnClose);
    dayWidget->setDate(day);
    dayWidget->raise();
}


void CalendarApp::showPrevMonth() {
    currentDate = currentDate.addMonths(-1);
    qDebug() << "Previous month button clicked";
    updateCalendar();
    updateMonth();
}

void CalendarApp::showNextMonth() {
    currentDate = currentDate.addMonths(1);
    qDebug() << "Next month button clicked";
    updateCalendar();
    updateMonth();
}

void CalendarApp::updateMonth(){
    title->setText(currentDate.toString("MMMM yyyy"));
}

void CalendarApp::updateCalendar() {
    QDate firstDayOfMonth(currentDate.year(), currentDate.month(), 1);
    int startingDayOfWeek = firstDayOfMonth.dayOfWeek();
    int daysInMonth = firstDayOfMonth.daysInMonth();

    int row = 0;
    int col = startingDayOfWeek - 1;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 7; ++j) {
            if ((i == 0 && j < col) || (7 * i + j - col + 1) > daysInMonth) {
                dayLabels[i][j]->setText("");
            } else {
                int day = 7 * i + j - col + 1;
                dayLabels[i][j]->setText(QString::number(day));
                dayLabels[i][j]->setProperty("date", firstDayOfMonth.addDays(7 * i + j - col));
            }
        }
    }
}


