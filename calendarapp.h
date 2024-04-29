#ifndef CALENDARAPP_H
#define CALENDARAPP_H

#include <QWidget>
#include <QDate>
#include <QPushButton>
#include "DayWidget.h"

class QLabel;
class QTextBrowser;

class CalendarApp : public QWidget {
    Q_OBJECT
public:
    explicit CalendarApp(QWidget *parent = nullptr);
    void updateCalendar();

private slots:
    void showPrevMonth();
    void showNextMonth();
    void updateMonth();
    void handleDayButtonClick();
private:
    DayWidget* dayWidget = nullptr;
    QDate currentDate;
    QPushButton *dayLabels[5][7];
    QLabel *title;
    QStringList daysOfWeek;

};

#endif // CALENDARAPP_H
