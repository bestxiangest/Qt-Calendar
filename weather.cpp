/************************************************
 *
 * 模块名称：weather.cpp
 * 功能描述：天气预报功能具体实现
 *
 *
 */
#include "weather.h"
#include "ui_weather.h"
#include <qmetaobject.h>
#include <QDateTime>
#include <QMessageBox>
#include <QEvent>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
//https://geoapi.qweather.com/v2/city/lookup?location=beij&key=3f71fd1e04a045f2a1996eb7c1f4302c
//http://t.weather.itboy.net/api/weather/city/[citycode]
Weather::Weather(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Weather)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);//窗口无边框
    setFixedSize(width(),height());//固定窗口大小

    //构建右键菜单
    mExitMenu = new QMenu(this);
    mExitAct = new QAction(this);

    mExitAct->setText("退出");
    mExitMenu->addAction(mExitAct);

    connect(mExitAct,SIGNAL(triggered()),this,SLOT(close()));
    mNetAccessManager = new QNetworkAccessManager;
    mNetAccessManager_getcitycode = new QNetworkAccessManager;
    connect(mNetAccessManager_getcitycode,&QNetworkAccessManager::finished,this,&Weather::New_onReplied_getCityCode);
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&Weather::New_onReplied);
    New_getCityCode("nanchang");

}

Weather::~Weather()
{
    delete ui;
}

//获取城市编码
void Weather::New_getCityCode(QString cityname){
    QUrl url(QString("https://geoapi.qweather.com/v2/city/lookup?location=%1&key=3f71fd1e04a045f2a1996eb7c1f4302c").arg(cityname));
    mNetAccessManager_getcitycode->get(QNetworkRequest(url));
}
//获取城市天气信息
void Weather::New_getWeatherInfo(QString cityCode)
{
    QUrl url("http://t.weather.itboy.net/api/weather/city/"+cityCode);
    mNetAccessManager->get(QNetworkRequest(url));
}

void Weather::on_pushButton_search_clicked()
{
    QString cityname = ui->lineEdit_searchCity->text();
    if(cityname != ""){
        New_getCityCode(cityname);
        ui->lineEdit_searchCity->setText("");
    }
}


void Weather::contextMenuEvent(QContextMenuEvent *event)
{
    //弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    event->accept();
}

void Weather::mousePressEvent(QMouseEvent *event)
{
    mOffset = event->globalPos()-this->pos();
}

void Weather::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-mOffset);
}

void Weather::New_onReplied(QNetworkReply *reply)//获取网络数据(天气）
{
    qDebug() << "New_onReplied success";

    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "operation:" << reply->operation();
    qDebug() << "status code:" << status_code;
    qDebug() << "url:" << reply->url();
    qDebug() << "raw header:" << reply->rawHeaderList();

    if(reply->error() != QNetworkReply::NoError || status_code != 200){
        //qDebug() << reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"天气","请求数据失败",QMessageBox::Ok);
    }else{
        QByteArray byteArray = reply->readAll();
        //qDebug() <<"read all:"<< byteArray.data();
        New_parseJson(byteArray);
    }
    reply->deleteLater();
}
void Weather::New_onReplied_getCityCode(QNetworkReply *reply)//获取网络数据（城市编码）
{
    qDebug() << "New_onReplied_getCityCode success";

    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "operation:" << reply->operation();
    qDebug() << "status code:" << status_code;
    qDebug() << "url:" << reply->url();
    qDebug() << "raw header:" << reply->rawHeaderList();

    if(reply->error() != QNetworkReply::NoError || status_code != 200){
        qDebug() << reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"城市编码","请求数据失败",QMessageBox::Ok);
    }else{
        QByteArray byteArray = reply->readAll();
        qDebug() <<"read all:"<< byteArray.data();
        New_parseJson_getCitycode(byteArray);
    }
    reply->deleteLater();
}
//处理json信息（城市编码）
void Weather::New_parseJson_getCitycode(QByteArray &byteArray){
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&err);
    if(err.error != QJsonParseError::NoError){
        return;
    }
    QJsonObject rootObj = doc.object();
    QJsonArray locationArray = rootObj.value("location").toArray();
    QString citycode = locationArray[0].toObject().value("id").toString();
    New_getWeatherInfo(citycode);
}
//处理json信息（天气）

void Weather::New_parseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&err);
    if(err.error != QJsonParseError::NoError){
        return;
    }

    QJsonObject rootObj = doc.object();
    //qDebug() << rootObj.value("message").toString();

    //解析日期和城市
    mToday.date = rootObj.value("date").toString();
    mToday.city = rootObj.value("cityInfo").toObject().value("city").toString();

    //解析yesterday
    QJsonObject objData = rootObj.value("data").toObject();
    QJsonObject objYesterday = objData.value("yesterday").toObject();
    mDay[0].week = objYesterday.value("week").toString();
    mDay[0].date = objYesterday.value("ymd").toString();
    mDay[0].type = objYesterday.value("type").toString();

    QString s = objYesterday.value("high").toString().split(" ").at(1); //18°
    s = s.left(s.length()-1);//取到高温
    mDay[0].high = s.toInt();

    QString l = objYesterday.value("low").toString().split(" ").at(1); //18°
    l = l.left(l.length()-1);//取到低温
    mDay[0].low = l.toInt();

    //风向风力
    mDay[0].fx = objYesterday.value("fx").toString();
    mDay[0].fl = objYesterday.value("fl").toString();
    //污染指数
    mDay[0].aqi = objYesterday.value("api").toDouble();

    //解析forcast中五天的数据
    QJsonArray forecastArr = objData.value("forecast").toArray();
    for(int i = 0;i < 8;i++){
        QJsonObject objForecast = forecastArr[i].toObject();
        mDay[i+1].week = objForecast.value("week").toString();
        mDay[i+1].date = objForecast.value("ymd").toString();
        mDay[i+1].type = objForecast.value("type").toString();
        //高温低温
        QString s = objForecast.value("high").toString().split(" ").at(1); //18°
        QString high = s.left(s.length()-1);//取到高温
        mDay[i+1].high = high.toInt();
        QString l = objForecast.value("low").toString().split(" ").at(1); //18°
        QString low = l.left(l.length()-1);//取到低温
        mDay[i+1].low = low.toInt();
        //风向风力
        mDay[i+1].fx = objForecast.value("fx").toString();
        mDay[i+1].fl = objForecast.value("fl").toString();
        //污染指数
        mDay[i+1].aqi = objForecast.value("api").toDouble();
    }
    //解析今天的数据
    mToday.ganmao = objData.value("ganmao").toString();
    mToday.wendu = objData.value("wendu").toInt();
    mToday.shidu = objData.value("shidu").toString();
    mToday.pm25 = objData.value("pm25").toInt();
    mToday.quality = objData.value("quality").toString();

    mToday.type = mDay[1].type;
    mToday.fx = mDay[1].fx;
    mToday.fl = mDay[1].fl;
    mToday.high = mDay[1].high;

    mToday.low = mDay[1].low;

    //更新UI
    ui->label_cityName->setText(QDate::currentDate().toString("MM-dd")+"在"+mToday.city+"的天气为");
    ui->label_temperature->setText(mToday.type+"   "+QString::number(mToday.low)+"-"+QString::number(mToday.high)+"℃");
    ui->label_date_1->setText(mDay[0].date.right(5));
    ui->label_tomTem_1->setText(QString::number(mDay[0].low)+"-"+QString::number(mDay[0].high)+"℃");
    ui->label_tomWeather_1->setText(mDay[0].type);
    for(int i = 2;i<=7;i++){
        QLabel * dateLabel = ui->groupBox->findChild<QLabel *>("label_date_"+QString::number(i));//查找第一天之后的datelabel
        QLabel * temperatureLabel = ui->groupBox->findChild<QLabel *>("label_tomTem_"+QString::number(i));//查找第一天之后的datelabel
        QLabel * weatherLabel = ui->groupBox->findChild<QLabel *>("label_tomWeather_"+QString::number(i));
        dateLabel->setText(mDay[i].date.right(5));
        temperatureLabel->setText(QString::number(mDay[i].low)+"-"+QString::number(mDay[i].high)+"℃");
        weatherLabel->setText(mDay[i].type);
    }
}


//获取当天的天气预报数据
void Weather::New_Get_CurrentWeather(QNetworkReply * qReply){

}
//获取三天内的天气预报数据
void Weather::New_Get_ThreeWearher(QNetworkReply * qReply){

}

