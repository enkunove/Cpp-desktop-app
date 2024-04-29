#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include <QWidget>
#include <QDate>
class QLabel;

class DayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DayWidget(const QDate &date, QWidget *parent = nullptr);

    void setDate(const QDate &date);

private:
    QLabel *dayLabel;
};

#endif // DAYWIDGET_H
