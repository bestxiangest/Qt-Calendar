/************************************************
 *
 * 模块名称：thewholeyear.cpp
 * 功能描述：查看全年日历功能具体实现
 *
 *
 */
#include "thewholeyear.h"
#include "ui_thewholeyear.h"
#include <QDate>

TheWholeYear::TheWholeYear(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TheWholeYear)
{
    ui->setupUi(this);
}

TheWholeYear::~TheWholeYear()
{
    delete ui;
}

int isLeapYear(int year);//判断是否为闰年
int daysOfMonth(int year, int month);//获取某年某月有多少天


//判断是否为闰年
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//获取某年某月有多少天
int daysOfMonth(int year, int month) {
    switch (month) {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    }
    return 0;
}

//画出日历
QString TheWholeYear::drawCalendar(int nowMonth,int uiNum) {
    qDebug()<<"全年日历"<<nowMonth;
    QString content;
    content = QString::number(nowMonth)+"月"+'\n';
    QString week[7] = {"一","二"," 三"," 四"," 五"," 六"," 日"};
    for (int i = 0; i < 7; i++)
        content=content+week[i]+"   ";
    content += "\n";
    QDate nowdate = m_date.addMonths(-m_date.month()+nowMonth);
    int day = nowdate.day();
    QDate firstdayofmonth = nowdate.addDays(-day+1);//获取本月第一天
    int firstday_week = firstdayofmonth.dayOfWeek();//获取本月第一天是周几
    qDebug()<<"firstday_week:"<<firstday_week;
    for(int i = 0;i<firstday_week-1;i++){
        switch(firstday_week){
        case 1:
            content+="";
            break;
        case 2:
            content+="      ";
            break;
        case 3:
            content+="       ";
            break;
        case 4:
            content+="       ";
            break;
        case 5:
            content+="       ";
            break;
        case 6:
            content+="       ";
            break;
        case 7:
            content+="       ";
            break;
        }


    }
    int lastloopday = 7 - firstday_week + 1;
    for(int i = 1;i<=daysOfMonth(nowdate.year(),nowdate.month());i++){//找到第一天
        if(i<=9)content = content + QString::number(i)+"     ";
        else{
            content = content + QString::number(i)+"   ";
        }
        lastloopday--;
        if(lastloopday == 0){
            content+='\n';
            lastloopday = 7;
        }

    }
    return content;
}


void TheWholeYear::receiveData(QDate date)
{
    m_date = date;

    for(int i = 1;i<=12;i++){
        QLabel * label = ui->groupBox->findChild<QLabel *>("label_"+QString::number(i));
        QDate s_date = m_date.addMonths(-m_date.month()+i);
        label->setText(drawCalendar(s_date.month(),i));
    }

}

