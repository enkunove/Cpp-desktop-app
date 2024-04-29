#include "DayWidget.h"
#include <QLabel>
#include <QVBoxLayout>

DayWidget::DayWidget(const QDate &date, QWidget *parent)
    : QWidget(parent)
{
    dayLabel = new QLabel(date.toString("dd MMM yyyy"), this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(dayLabel);
}

void DayWidget::setDate(const QDate &date) {
    dayLabel->setText(date.toString("dd MMM yyyy"));
}
