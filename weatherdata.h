/************************************************
 *
 * 模块名称：weatherdata.h
 * 功能描述：天气预报信息类的设计
 *
 *
 */
#ifndef WEATHERDATA_H
#define WEATHERDATA_H
#include <QString>
class Today{
    public:
        QString date;
        QString week;
        QString city;
        QString ganmao;
        int wendu;
        QString shidu;
        int pm25;
        QString quality;
        QString type;
        QString fx;
        QString fl;
        int high;
        int low;
        int aqi;
        Today(){
            date ="2022-10-20";
            week = "0";
            city ="广州";

            ganmao ="感冒指数";

            wendu =0;
            shidu ="0%";
            pm25 = 0;
            quality ="无数据";
            type ="多云";

            fl ="2级";
            fx ="南风";

            high = 30;
            low = 18;

            aqi = 0;
        }
};

class Day{
public:
    QString date;
    QString week;
    QString type;
    int high;
    int low;
    QString fx;
    QString fl;
    int aqi;
    Day(){
        date ="2022-10-20";
        week ="周五";
        type ="多云";
        high = 0;
        low =0;
        fx ="南风";
        fl ="2级";
        aqi = 0;
    }
};

#endif // WEATHERDATA_H
