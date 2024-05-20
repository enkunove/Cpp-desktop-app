#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QRadioButton>
#include <QString>
#include <QFile>

class QLabel;
class QPushButton;

class DayWidget : public QWidget {
    Q_OBJECT
public:
    explicit DayWidget(const QDate &date, const QString &summ, QWidget *parent = nullptr);
    QString style = "QLabel {"
                    "    font-size: 16px;"
                    "    color: #333333;"
                    "}"
                    "QRadioButton {"
                    "    font-size: 14px;"
                    "    color: #6d6e71;"
                    "}"
                    "QPushButton {"
                    "    font-size: 16px;"
                    "    color: #ffffff;"
                    "    background-color: #ff5733;"
                    "    border: none;"
                    "    padding: 8px 16px;"
                    "    border-radius: 3px; "
                    "}"
                    "QPushButton:hover {"
                    "    background-color: #be3e23;"
                    "}"
                    "QLineEdit {"
                    "    font-size: 16px;"
                    "    color: #333333;"
                    "    background-color: #f8f9fa;"
                    "    border: 1px solid #ced4da;"
                    "    padding: 8px;"
                    "}"
                    "DayWidget {"
                    "    background-color: #FDF7E4;"
                    "}";


private:
    QLabel *dateLabel;
    QLabel *totalLab;
    QRadioButton *inBut;
    QRadioButton *outBut;
    QPushButton *Add;
    QPushButton *save;
    QLineEdit *sum;
    QLabel *totalValue;
    void updateColor();

signals:
    void Save();

private slots:
    void addTransaction();
    void saveTransaction(const QDate &date);
};

#endif // DAYWIDGET_H
