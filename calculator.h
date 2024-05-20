#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "calendarapp.h"
#include <QLayout>
#include <QSlider>
#include <QCheckBox>

class QLabel;
class QPushButton;

class calculator : public QWidget {
    Q_OBJECT
public:
    explicit calculator (QWidget *parent = nullptr);
    ~calculator();
private:
    QPushButton *calc;
    QGridLayout *layout;
    QLineEdit *sum;
    QSlider *percentSlider;
    QLineEdit *addings;
    QLineEdit *period;
    QLabel *sumL;
    QLabel *percentValue;
    QLabel *percentL;
    QLabel *addingsL;
    QLabel *periodL;
    QLabel *result;
    QCheckBox *state;

    QString ButtonStyle = "QPushButton {font-size: 16px; color: #ffffff; background-color: #ff5733; border: none; padding: 8px 16px;border-radius: 3px;} QPushButton:hover {background-color: #be3e23; }";
private slots:
    void calculation();
    void updatePercentValue(int value);

signals:
    void Close();

};

#endif // CALCULATOR_H
