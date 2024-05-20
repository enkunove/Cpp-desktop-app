#ifndef CALENDARAPP_H
#define CALENDARAPP_H

#include <QWidget>
#include <QCloseEvent>
#include <QStandardItem>
#include <QDate>
#include <QPushButton>
#include <QTableView>
#include "DayWidget.h"
#include "calculator.h"

class QLabel;
class QGridLayout;
class calculator;

class CalendarApp : public QWidget {
    Q_OBJECT
public:
    explicit CalendarApp(QWidget *parent = nullptr);

private slots:
    void showPrevMonth();
    void showDayWidget(const QDate &date, const QString &sum);
    void showNextMonth();
    void clearData();
    void calculateMonthResult();
    void updateCalendar();
    void sortingShell();
    void sortingInserts();
    void openCalc();

private:
    DayWidget *currentDayWidget;
    calculator *c;
    QDate currentDate;
    QPushButton *dayLabels[6][7];
    QLabel *title;
    QLabel *sumLabels[6][7];
    QGridLayout *calendarLayout;
    QLabel *a;
    QList<QString> *daysOfWeek;
    QGridLayout *MenuLayout;

    QStandardItemModel *model;
    QTableView *tableView;
    void updateTable();

    QString ButtonStyle = "QPushButton {font-size: 16px; color: #ffffff; background-color: #ff5733; border: none; padding: 8px 16px;border-radius: 3px;} QPushButton:hover {background-color: #be3e23; }";
    QString tableStyle =         "QTableView {"
                         "    border: 1px solid #d3d3d3;"
                         "    gridline-color: #d3d3d3;"
                         "    background-color: #f5f5f5;"
                         "    alternate-background-color: #e9e9e9;"
                         "    selection-background-color: #ff5733;"
                         "    selection-color: white;"
                         "}"
                         "QHeaderView::section {"
                         "    background-color: #ff5733;"
                         "    padding: 4px;"
                         "    border: 1px solid #d3d3d3;"
                         "    font-size: 12pt;"
                         "    font-weight: bold;"
                         "    color: white;"
                         "}"
                         "QTableView::item {"
                         "    padding: 4px;"
                         "}";
    bool onlyOne = true;
    bool oneTime = true;

};

#endif // CALENDARAPP_H
