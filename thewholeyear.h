/************************************************
 *
 * 模块名称：thewholeyear.h
 * 功能描述：查看全年日历类的设计
 *
 *
 */
#ifndef THEWHOLEYEAR_H
#define THEWHOLEYEAR_H

#include <QWidget>
#include <QDate>

namespace Ui {
class TheWholeYear;
}

class TheWholeYear : public QWidget
{
    Q_OBJECT
private:
    QDate m_date;

public:
    explicit TheWholeYear(QWidget *parent = nullptr);
    ~TheWholeYear();
public slots:
    void receiveData(QDate);
    QString drawCalendar(int,int);

private:
    Ui::TheWholeYear *ui;
};

#endif // THEWHOLEYEAR_H
