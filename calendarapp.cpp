#include "calendarapp.h"
#include "DayWidget.h"
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QStandardItem>
#include <QApplication>
#include <QHeaderView>

CalendarApp::CalendarApp(QWidget *parent) : QWidget(parent)
{
    QGridLayout *MainLayout = new QGridLayout(this);
    QGridLayout *layout = new QGridLayout();
    QGridLayout *CalendarLayout = new QGridLayout();
    QHBoxLayout *ButtonsLayout = new QHBoxLayout();
    QGridLayout *MenuLayout = new QGridLayout();

    daysOfWeek = new QList<QString>();
    *daysOfWeek << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";

    currentDate = QDate::currentDate();
    title = new QLabel(currentDate.toString("MMMM yyyy"), this);

    layout->addWidget(title, 0, 0, 1,1, Qt::AlignCenter);
    layout->addLayout(CalendarLayout, 1, 0, 6, 1, Qt::AlignTop);
    layout->addLayout(ButtonsLayout, 7, 0, 1, 1, Qt::AlignVCenter);

    for (int i = 0; i < 7; ++i)
    {
        QLabel *dayLabel = new QLabel(daysOfWeek->at(i), this);
        CalendarLayout->addWidget(dayLabel, 0, i, Qt::AlignCenter | Qt::AlignTop);
    }

    QDate firstDayOfMonth(currentDate.year(), currentDate.month(), 1);
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            int col = firstDayOfMonth.dayOfWeek() - 1;
            QPushButton *button = dayLabels[i][j] = new QPushButton("", this);
            QLabel *labs = sumLabels[i][j] = new QLabel("", this);

            button->setFixedSize(140, 140);
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            CalendarLayout->addWidget(button, i+1, j, Qt::AlignCenter);
            CalendarLayout->addWidget(labs, i+1, j, Qt::AlignHCenter | Qt::AlignBottom);

            labs->setStyleSheet("font-size: px; color: #00aa00;");
            QDate day = firstDayOfMonth.addDays(7 * i + j - col);
            button->setProperty("date", day);
            QString sumValue = labs->text();
            button->setProperty("sum", sumValue);

            connect(button, &QPushButton::clicked, [=]() {
                QString buttonSumValue = button->property("sum").toString();
                showDayWidget(button->property("date").toDate(), buttonSumValue);
                qDebug() << buttonSumValue;});
        }
    }

    QPushButton *prevMonthButton = new QPushButton("<", this);
    QPushButton *nextMonthButton = new QPushButton(">", this);
    prevMonthButton->setObjectName("b");
    nextMonthButton->setObjectName("b");
    connect(prevMonthButton, &QPushButton::clicked, this, &CalendarApp::showPrevMonth);
    connect(nextMonthButton, &QPushButton::clicked, this, &CalendarApp::showNextMonth);
    ButtonsLayout->addWidget(prevMonthButton);
    ButtonsLayout->addWidget(nextMonthButton);

    a = new QLabel("0", this);
    QLabel *calcRes = new QLabel("Month result:", this);
    QPushButton *sortBut2 = new QPushButton("Sort (decrease)", this);
    QPushButton *clear = new QPushButton("Clear Data (ne nado)", this);
    QPushButton *sortBut = new QPushButton("Sort (increase)", this);
    QPushButton *calc = new QPushButton("Open calculator", this);

    clear->setStyleSheet(ButtonStyle);
    calc->setStyleSheet(ButtonStyle);
    sortBut2->setStyleSheet(ButtonStyle);
    sortBut->setStyleSheet(ButtonStyle);

    connect(clear, SIGNAL(clicked(bool)), this, SLOT(clearData()));
    connect(clear, SIGNAL(clicked(bool)), this, SLOT(calculateMonthResult()));
    connect(calc, SIGNAL(clicked(bool)), this, SLOT(openCalc()));
    connect(sortBut, SIGNAL(clicked(bool)), this, SLOT(sortingShell()));
    connect(sortBut2, SIGNAL(clicked(bool)), this, SLOT(sortingInserts()));

    model = new QStandardItemModel(this);
    tableView = new QTableView(this);
    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << "Date" << "Amount";
    model->setHorizontalHeaderLabels(horizontalHeaderLabels);
    tableView->setModel(model);
    tableView->show();

    MenuLayout->addWidget(clear, 0, 0, 1, 2);
    MenuLayout->addWidget(calc, 1, 0, 1, 2);
    MenuLayout->addWidget(calcRes, 5, 0, 1, 1, Qt::AlignRight | Qt::AlignBottom);
    MenuLayout->addWidget(tableView, 2, 0, 2, 2, Qt::AlignVCenter);
    MenuLayout->addWidget(a, 5, 1, 1 , 1, Qt::AlignBottom);
    MenuLayout->addWidget(sortBut2, 4, 0, 1, 1);
    MenuLayout->addWidget(sortBut, 4, 1, 1, 1);

    MainLayout->addLayout(layout, 0, 0);
    MainLayout->addLayout(MenuLayout, 0, 1);

    updateCalendar();
    updateTable();
    calculateMonthResult();
}

void CalendarApp::sortingInserts()
{
    struct TableRow
    {
        QString date;
        double amount;
    };

    QVector<TableRow> rows;
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QString date = model->item(row, 0)->text();
        double amount = model->item(row, 1)->text().toDouble();
        rows.append({date, amount});
    }

    // Insertion sort
    int n = rows.size();
    for (int i = 1; i < n; i++) {
        TableRow key = rows[i];
        int j = i - 1;

        while (j >= 0 && rows[j].amount < key.amount)
        {
            rows[j + 1] = rows[j];
            j--;
        }
        rows[j + 1] = key;
    }
    model->clear();
    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << "Date" << "Amount";
    model->setHorizontalHeaderLabels(horizontalHeaderLabels);

    for (int row = 0; row < rows.size(); ++row)
    {
        QStandardItem *dateItem = new QStandardItem(rows[row].date);
        QStandardItem *amountItem = new QStandardItem(QString::number(rows[row].amount));
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        amountItem->setFlags(amountItem->flags() & ~Qt::ItemIsEditable);

        QFont font("Arial", 10, QFont::Bold);
        dateItem->setFont(font);
        amountItem->setFont(font);

        dateItem->setTextAlignment(Qt::AlignCenter);
        amountItem->setTextAlignment(Qt::AlignCenter);

        model->setItem(row, 0, dateItem);
        model->setItem(row, 1, amountItem);
    }
    tableView->setStyleSheet(tableStyle);
}


void CalendarApp::sortingShell()
{
    struct TableRow
    {
        QString date;
        double amount;
    };
    QVector<TableRow> rows;
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QString date = model->item(row, 0)->text();
        double amount = model->item(row, 1)->text().toDouble();
        rows.append({date, amount});
    }
    int n = rows.size();
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            TableRow temp = rows[i];
            int j;
            for (j = i; j >= gap && rows[j - gap].amount > temp.amount; j -= gap)
            {
                rows[j] = rows[j - gap];
            }
            rows[j] = temp;
        }
    }
    model->clear();
    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << "Date" << "Amount";
    model->setHorizontalHeaderLabels(horizontalHeaderLabels);

    for (int row = 0; row < rows.size(); ++row)
    {
        QStandardItem *dateItem = new QStandardItem(rows[row].date);
        QStandardItem *amountItem = new QStandardItem(QString::number(rows[row].amount));
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        amountItem->setFlags(amountItem->flags() & ~Qt::ItemIsEditable);

        QFont font("Arial", 10, QFont::Bold);
        dateItem->setFont(font);
        amountItem->setFont(font);

        dateItem->setTextAlignment(Qt::AlignCenter);
        amountItem->setTextAlignment(Qt::AlignCenter);

        model->setItem(row, 0, dateItem);
        model->setItem(row, 1, amountItem);
    }
}


void CalendarApp::showDayWidget(const QDate &date, const QString &sum)
{
    currentDayWidget = new DayWidget(date, sum);
    connect(currentDayWidget, &DayWidget::Save, this, &CalendarApp::updateCalendar);
    connect(currentDayWidget, &DayWidget::Save, this, &CalendarApp::updateTable);
    connect(currentDayWidget, &DayWidget::Save, this, &CalendarApp::calculateMonthResult);
    currentDayWidget->setAttribute(Qt::WA_DeleteOnClose);
    currentDayWidget->show();
}

void CalendarApp::openCalc()
{
    if (onlyOne == true)
    {
    c = new calculator();
    c->setAttribute(Qt::WA_DeleteOnClose);
    connect(c, &calculator::Close, [=]() {onlyOne = true;});
    c->show();
    onlyOne = false;
    }
    else c->activateWindow();
}

void CalendarApp::showPrevMonth()
{
    currentDate = currentDate.addMonths(-1);
    title->setText(currentDate.toString("MMMM yyyy"));
    updateCalendar();
    calculateMonthResult();
    updateTable();
}

void CalendarApp::showNextMonth()
{
    currentDate = currentDate.addMonths(1);
    title->setText(currentDate.toString("MMMM yyyy"));
    updateCalendar();
    calculateMonthResult();
    updateTable();
}

void CalendarApp::updateCalendar() {
    QDate firstDayOfMonth(currentDate.year(), currentDate.month(), 1);
    int col = firstDayOfMonth.dayOfWeek() - 1;

    QFile file("/home/egor/calendarapp/database.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);
    QMap<QDate, QString> dataMap;

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() >= 2)
        {
            QString dateStr = parts[0];
            QString valueStr = parts[1];
            QDate fileDate = QDate::fromString(dateStr, "dd.MM.yyyy");
            dataMap[fileDate] = valueStr;
        }
    }
    file.close();

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            QDate day = firstDayOfMonth.addDays(7 * i + j - col);
            QPushButton *button = dayLabels[i][j];
            QLabel *label = sumLabels[i][j];

            if (!day.isValid() || day.month() != currentDate.month())
            {
                button->setText(QString::number(day.day()));
                button->setStyleSheet("color: #808080;");
                button->setEnabled(false);
                label->setText("");
            }
            else
            {
                button->setText(QString::number(day.day()));
                button->setEnabled(true);
                button->setProperty("date", day);
                button->setStyleSheet("");

                QString valueStr = dataMap.value(day);
                if (!valueStr.isEmpty())
                {
                    label->setText(valueStr);
                    button->setProperty("sum", valueStr);
                    if (valueStr.toDouble() >= 0)
                    {
                        label->setText("+" + label->text());
                        label->setStyleSheet("color: green;");
                    }
                    else
                    {
                        label->setStyleSheet("color: red;");
                    }
                }
                else
                {
                    label->setText("");
                    button->setProperty("sum", "");
                }
            }
        }
    }
}

void CalendarApp::clearData()
{
    QFile file("/home/egor/calendarapp/database.txt");
    file.resize(0);
    file.close();
    updateCalendar();
    updateTable();
}

void CalendarApp::calculateMonthResult()
{
    double resultValue = 0;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            resultValue += sumLabels[i][j]->text().toDouble();
        }
    }
    if (resultValue >= 0)
    {
        a->setText("+" + QString::number(resultValue));
        a->setStyleSheet("color: green;");
    }
    else
    {
        a->setText(QString::number(resultValue));
        a->setStyleSheet("color: red;");
    }
}

void CalendarApp::updateTable()
{
    model->clear();
    QStringList horizontalHeaderLabels;
    horizontalHeaderLabels << "Date" << "Amount";
    model->setHorizontalHeaderLabels(horizontalHeaderLabels);

    int row = 0;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 7; ++j)
        {
            QDate date = dayLabels[i][j]->property("date").toDate();
            QString amountStr = sumLabels[i][j]->text();

            if (!amountStr.isEmpty() && amountStr != "0")
            {
                QStandardItem *dateItem = new QStandardItem(date.toString("dd.MM.yyyy"));
                QStandardItem *amountItem = new QStandardItem(amountStr);

                dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
                amountItem->setFlags(amountItem->flags() & ~Qt::ItemIsEditable);

                QFont font("Arial", 10, QFont::Bold);
                dateItem->setFont(font);
                amountItem->setFont(font);

                dateItem->setTextAlignment(Qt::AlignCenter);
                amountItem->setTextAlignment(Qt::AlignCenter);

                model->setItem(row, 0, dateItem);
                model->setItem(row, 1, amountItem);

                row++;
            }
        }
    }
    tableView->setStyleSheet(tableStyle);

    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tableView->setFixedHeight(837);
    tableView->show();
}
