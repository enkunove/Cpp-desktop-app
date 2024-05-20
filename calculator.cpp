#include "calculator.h"
#include <cmath>

calculator::calculator(QWidget *parent) : QWidget(parent)
{
    calc = new QPushButton("Calculate", this);
    layout = new QGridLayout(this);
    sum = new QLineEdit();
    percentSlider = new QSlider(Qt::Horizontal);
    percentSlider->setRange(0, 100);
    addings = new QLineEdit();
    period = new QLineEdit();
    sumL = new QLabel("Invested sum:");
    percentL = new QLabel("Percent (per 1 period): ");
    addingsL = new QLabel("Addings (per 1 period): ");
    periodL = new QLabel("Number of periods: ");
    result = new QLabel("Result: 0.00");
    percentValue = new QLabel("0%");
    state = new QCheckBox("Compound Interest");

    layout->addWidget(sumL, 0, 0);
    layout->addWidget(sum, 1, 0);
    layout->addWidget(percentL, 2, 0);
    layout->addWidget(percentSlider, 3, 0);
    layout->addWidget(percentValue, 3, 1);
    layout->addWidget(addingsL, 4, 0);
    layout->addWidget(addings, 5, 0);
    layout->addWidget(periodL, 6, 0);
    layout->addWidget(period, 7, 0);
    layout->addWidget(result, 10, 0);
    layout->addWidget(calc, 9, 0);
    layout->addWidget(state, 8,0);

    connect(calc, &QPushButton::clicked, this, &calculator::calculation);
    connect(percentSlider, &QSlider::valueChanged, this, &calculator::updatePercentValue);

    setFixedSize(600, 400);
    calc->setStyleSheet(ButtonStyle);
    setWindowTitle("Calculator");
    layout->setColumnMinimumWidth(1, 40);
    sum->setStyleSheet("QLineEdit { padding: 5px; font-size: 14px; }");
    addings->setStyleSheet("QLineEdit { padding: 5px; font-size: 14px; }");
    period->setStyleSheet("QLineEdit { padding: 5px; font-size: 14px; }");
    percentSlider->setStyleSheet("QSlider::groove:horizontal { background: #ff5733; height: 10px; }"
                                 "QSlider::handle:horizontal { background: #be3e23; width: 20px; margin: -5px 0; border-radius: 10px; }");
    percentValue->setStyleSheet("QLabel { font-size: 14px; }");
    sumL->setStyleSheet("QLabel { font-size: 16px; }");
    percentL->setStyleSheet("QLabel { font-size: 16px; }");
    addingsL->setStyleSheet("QLabel { font-size: 16px; }");
    periodL->setStyleSheet("QLabel { font-size: 16px; }");
    result->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; }");
    state->setStyleSheet("QCheckBox { font-size: 14px; }");
}

void calculator::updatePercentValue(int value)
{
    percentValue->setText(QString::number(value) + "%");
}

void calculator::calculation()
{
    double s = sum->text().toDouble();
    double p = percentSlider->value();
    double a = addings->text().toDouble();
    double per = period->text().toDouble();

    double res = s;

    if (per <= 0) return;

    if (state->isChecked())
    {
        for (int i = 0; i < per; ++i)
        {
            res += a;
            res *= (1 + p / 100);
        }
    }
    else
    {
        double totalAddings = a * per;
        double interest = res * p / 100 * per;
        res += totalAddings + interest;
    }

    result->setText(QString("Result: %1").arg(res, 0, 'f', 2));
}

calculator::~calculator()
{
    emit Close();
}
