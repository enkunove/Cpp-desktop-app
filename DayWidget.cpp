#include "DayWidget.h"
#include <QLayout>

DayWidget::DayWidget(const QDate &date, const QString &summ, QWidget *parent)
    : QWidget(parent)
{
    totalLab = new QLabel("Total:", this);
    inBut = new QRadioButton("Income", this);
    outBut = new QRadioButton("Outcome", this);
    Add = new QPushButton("Add", this);
    save = new QPushButton("Save", this);
    sum = new QLineEdit(this);

    totalValue = summ==""? new QLabel("0", this) : new QLabel(summ, this);
    updateColor();

    QHBoxLayout *totalLayout = new QHBoxLayout();
    totalLayout->addWidget(totalLab);
    totalLayout->addWidget(totalValue);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGridLayout *inputLayout = new QGridLayout();

    inputLayout->addWidget(inBut, 0, 0);
    inputLayout->addWidget(outBut, 0, 1);
    inputLayout->addWidget(sum, 1, 0, 1, 2);
    inputLayout->addWidget(save,2, 1);
    inputLayout->addWidget(Add, 2, 0, 1, 1);

    mainLayout->addLayout(totalLayout);
    mainLayout->addLayout(inputLayout);

    inputLayout->setColumnStretch(0, 1);
    inputLayout->setColumnStretch(1, 1);

    inputLayout->setRowStretch(0, 1);
    inputLayout->setRowStretch(1, 1);
    inputLayout->setRowStretch(2, 1);

    connect(Add, SIGNAL(clicked()), this, SLOT(addTransaction()));
    connect(save, &QPushButton::clicked, this, [this, date]() {
        saveTransaction(date);
    });

    setStyleSheet(style);
    setFixedSize(400, 200);
    setWindowTitle(date.toString("dd.MM.yyyy"));
}


void DayWidget::addTransaction()
{
    double currentAmount = totalValue->text().toDouble();
    double amount = sum->text().toDouble();
    double newAmount;

    if (inBut->isChecked())
    {
        newAmount = currentAmount + amount;
    }
    else if (outBut->isChecked())
    {
        newAmount = currentAmount - amount;
    }
    else return;
    totalValue->setText(QString::number(newAmount));
    sum->setText("");
    updateColor();
}

void DayWidget::saveTransaction(const QDate &date)
{
    double currVal = totalValue->text().toDouble();
    QString currDatStr = date.toString("dd.MM.yyyy");
    QString currValStr = QString::number(currVal);

    QFile file("/home/egor/calendarapp/database.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading and writing.";
        return;
    }

    QTextStream in(&file);
    QStringList updatedContents;
    bool dateFound = false;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(" ");
        if (parts.size() >= 2)
        {
            QString existingDateStr = parts[0];
            QString existingValueStr = parts[1];

            if (existingDateStr == currDatStr)
            {
                double newValue =  currVal;
                updatedContents.append(currDatStr + " " + QString::number(newValue));
                dateFound = true;
            }
            else
            {
                updatedContents.append(line);
            }
        }
    }
    if (!dateFound)
    {
        updatedContents.append(currDatStr + " " + currValStr);
    }
    file.resize(0);
    QTextStream out(&file);
    for (const QString &line : updatedContents)
    {
        out << line << "\n";
    }

    file.close();
    sum->setText("");
    close();
    emit Save();
}

void DayWidget::updateColor()
{
    if (totalValue->text().toDouble() >= 0)
    {
        totalValue->setText("+" + totalValue->text());
        totalValue->setStyleSheet("font-size: 24px; color: #00aa00;");
    }
    else
    {
        totalValue->setStyleSheet("font-size: 24px; color: red;");
    }
}

