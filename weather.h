/************************************************
 *
 * 模块名称：weather.h
 * 功能描述：天气窗口类的设计
 *
 *
 */
#ifndef WEATHER_H
#define WEATHER_H

#include <QMainWindow>
#include <QMenu>
#include <qevent.h>
#include <weatherdata.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QTimer>
#include <QTime>
#include <QThread>
#include <QDebug>

namespace Ui {
class Weather;
}

class Weather : public QMainWindow
{
    Q_OBJECT

public:
    explicit Weather(QWidget *parent = nullptr);
    ~Weather();

private:
    Ui::Weather *ui;
    QNetworkAccessManager * qNwkHttp;
    QNetworkAccessManager * qNwkHttp_getcitycode;
    QNetworkAccessManager *mNetAccessManager;
    QNetworkAccessManager *mNetAccessManager_getcitycode;
    Today mToday;
    Day mDay[9];

public slots:
    void New_onReplied(QNetworkReply * reply);
    void New_onReplied_getCityCode(QNetworkReply * reply);

public:
    //获取当天的天气预报数据
    void New_Get_CurrentWeather(QNetworkReply * qReply);
    //获取三天内的天气预报数据
    void New_Get_ThreeWearher(QNetworkReply * qReply);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void New_getWeatherInfo(QString cityCode);
    void New_parseJson(QByteArray & byteArray);
    void New_getCityCode(QString cityname);
    void New_parseJson_getCitycode(QByteArray & byteArray);

private slots:
    void on_pushButton_search_clicked();

private:
    QMenu * mExitMenu;//右键退出菜单
    QAction * mExitAct;//退出行为
    QPoint mOffset;
    QString cityCode;
};

#endif // WEATHER_H
