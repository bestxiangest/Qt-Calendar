/************************************************
 *
 * 模块名称：mainwindow.cpp
 * 功能描述：主窗口功能具体实现
 *
 *
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qmetaobject.h>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QStatusBar>
#include <QApplication>
#include <QPropertyAnimation>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <thewholeyear.h>

static int isVisi = 0;


QString weeks[] = {"MONDAY","TUESDAY","WEDNESDAY","THURSDAY","FRIDAY","SATURDAY","SUNDAY"};
QString months[] = {"JANUARY","FEBRUARY","MARCH","APRIL","MAY","JUNE","JULY","AUGUST","SEPTEMBER","OCTOBER","NOVEMBER","NOVEMBER","DECEMBER"};
QDate today = QDate::currentDate();
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowTitle("日历");
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);//窗口无边框
    setFixedSize(width(),height());//固定窗口大小
    this->setStatusBar(nullptr);
    //定时器部分
    QTimer * qtimer;
    qtimer = new QTimer();
    qtimer->setInterval(1000);
    qtimer->start();
    connect(qtimer,SIGNAL(timeout()),this,SLOT(OnTimerOutFunc()));
    //隐藏“查看”
    ui->groupBox_reminderLeft->setVisible(isVisi);
    //widget = new QWidget();// 创建内部QWidget
    layout = new QVBoxLayout(ui->scrollAreaWidgetContents);//在“查看”的QWidget中创建一个布局管理器
    //基本ui初始化
    iniUI();


    //右键窗口退出
    mExitMenu = new QMenu;
    mExitAct = new QAction;
    mExitMenu->addAction(mExitAct);
    mExitAct->setText("退出");
    connect(mExitAct,SIGNAL(triggered()),this,SLOT(close()));
    //网络管理器
    mNetAccessManager = new QNetworkAccessManager;
    connect(mNetAccessManager,&QNetworkAccessManager::finished,this,&MainWindow::onReplied);
    //获取天气数据
    getWeatherInfo("101240101");//南昌的cityCode。直接在构造中请求天气数据
    selectedDate = QDate::currentDate();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::iniUI(){
    ui->dateEdit->setVisible(false);
    QDateTime nowdatetime = QDateTime::currentDateTime();
    QString str_nowdatetime = nowdatetime.date().toString("yyyy-MM-dd")+"  "+nowdatetime.time().toString("hh:mm:ss");
    ui->dateEdit->setDate(nowdatetime.date());
    ui->label_nowDateTime->setText(str_nowdatetime);

    QDate nowdate = nowdatetime.date();
    ui->label_Week->setText(weeks[nowdate.dayOfWeek()-1]);
    ui->label_Month_Day->setText(months[nowdate.month()-1]+"   "+QString::number(nowdate.day()));
    ui->pushButton_Year->setText(QString::number(nowdate.year())+"/"+QString::number(nowdate.month()));
    int day = nowdate.day();
    QDate firstdayofmonth = nowdate.addDays(-day+1);//获取本月第一天
    int firstday_week = firstdayofmonth.dayOfWeek();//获取本月第一天是周几
}



//画出日历
void MainWindow::drawCalendar(QDate nowdate,int firstday_week){
    nowdate = nowdate.addMonths(-1);//上一个月的按钮把他隐藏
    for(int i = 1;i < firstday_week;i++){
        QPushButton * daybutton = ui->groupBox_Buttons->findChild<QPushButton *>("pushButton_"+QString::number(i));//查找上一个月的多余项
        daybutton->setStyleSheet("QPushButton{background:white;color: grey;border:none;}");
        daybutton->setText(QString(" "));//删去
    }

    nowdate = nowdate.addMonths(1);     //回到本月
    //qDebug()<<nowdate.toString("yyyy-MM-dd");
    selectedDate = nowdate;             //更新类中的selectedData

    qDebug()<<nowdate.toString()<<"huachurili";
    for(int i = firstday_week;i<=42;i++){//找到第一天
        QPushButton * daybutton = ui->groupBox_Buttons->findChild<QPushButton *>("pushButton_"+QString::number(i));//查找第一天后面的label
        if(i<=nowdate.daysInMonth()+firstday_week-1){//所有在本月的按钮控件
            daybutton->setText(QString::number(i-firstday_week+1));
            QObject::connect(daybutton,SIGNAL(clicked()),this,SLOT(daybuttonclicked()));
        }
        else if(i>nowdate.daysInMonth()+firstday_week-1){//后面多余的删去
            daybutton->setStyleSheet("QPushButton{background:white;color: grey;border:none;}");
            daybutton->setText(QString(" "));
        }
        if(nowdate.year() == selectedDate.year() && nowdate.month() == selectedDate.month() && (i-firstday_week+1) == selectedDate.day()){
            daybutton->setStyleSheet("QPushButton{background:rgb(85, 170, 127);color:black;border-radius:10px;}");
        }else{
            daybutton->setStyleSheet("QPushButton{background:white;color: grey;border:none;}");
        }

    }
    ui->pushButton_Year->setText(QString::number(nowdate.year())+"/"+QString::number(nowdate.month()));
    findReminder(selectedDate.toString("yyyy-MM-dd"));//更新日程
}

void MainWindow::findReminder(QString fileName)//更新日程
{
    ui->label_weekLeft->setText(fileName+"\n"+weeks[selectedDate.dayOfWeek()-1]);

    QFile jsonFile(QString(".\\scheduleFile\\%1.json").arg(fileName));
    clearLayout(layout);
    if (jsonFile.exists()) {
        // 文件存在，以只读模式打开
        if (jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray data = jsonFile.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if(!doc.isObject())return;
            QJsonArray eventArray = doc.object().value("events").toArray();
            int length = eventArray.size();//获取日程数量
            if(length == 0){
                jsonFile.remove();
                qDebug()<<"日程项为零，移除文件";
                ui->label_reminderTitle->setText("美好的一天~");
                ui->groupBox_reminderLeft->setVisible(0);
                jsonFile.close();
                return;
            }
            QString titleSum;//设置总标题内容，之后把它放在主窗口
            QString editSum;
            QString title[length],content[length],time[length];
            reminderData.clear();
            reminderData<<QString::number(length);
            for(int i = 0;i<length;i++){
                QJsonObject eventObject = eventArray[i].toObject();
                title[i] = eventObject.value("title").toString();
                reminderData<<title[i];
                content[i] = eventObject.value("content").toString();
                reminderData<<content[i];
                time[i] = eventObject.value("time").toString("HH:mm");
                reminderData<<time[i];
                if(i<4)titleSum = titleSum + title[i] + "\n";
                QPushButton *reminder_button = new QPushButton(QString(title[i]),ui->scrollAreaWidgetContents);
                reminder_button->setObjectName(QString::number(i+1)+" "+fileName);
                layout->addWidget(reminder_button);
                qDebug()<<reminder_button->objectName();
                connect(reminder_button,&QPushButton::clicked,[=]{
                    outputName(reminder_button);
                });
            }

            ui->label_reminderTitle->setText(titleSum);
            // 关闭文件
            jsonFile.close();
        }
        else {
            // 打开文件失败的处理
            qDebug() << "无法打开文件：" << jsonFile.errorString();
        }
    }else{
        ui->label_reminderTitle->setText("美好的一天~");
    }
}

//按下对应日期，日期背景变成绿色
void MainWindow::daybuttonclicked(){
    for(int i = 1;i<=42;i++){
        QPushButton * daybutton = ui->groupBox_Buttons->findChild<QPushButton *>("pushButton_"+QString::number(i));
        daybutton->setStyleSheet("QPushButton{background:white;color: grey;border:none;}");
    }
    QPushButton *daybutton = qobject_cast<QPushButton *>(sender());
    int selectedButtonDay = daybutton->text().toInt();
    //qDebug()<<selectedButtonDay;
    selectedDate = selectedDate.addDays(selectedButtonDay - selectedDate.day());
    //qDebug()<<selectedDate.day();
    if(daybutton->text()!=" "){
        daybutton->setStyleSheet("QPushButton{background:rgb(85, 170, 127);color:black;border-radius:10px;}");
        findReminder(selectedDate.toString("yyyy-MM-dd"));//更新日程
    }
}
//显示日期和时间
void MainWindow::OnTimerOutFunc(){
    QDateTime nowdatetime = QDateTime::currentDateTime();
    QString str_nowdatetime = nowdatetime.date().toString("yyyy-MM-dd")+"  "+nowdatetime.time().toString("hh:mm:ss");
    ui->label_nowDateTime->setText(str_nowdatetime);
}

void MainWindow::onReplied(QNetworkReply *reply)//获取网络数据
{
    qDebug() << "onReplied success";

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
        parseJson(byteArray);
    }
    reply->deleteLater();
}

//获取当天的天气预报数据
void MainWindow::Get_CurrentWeather(QNetworkReply * qReply){

}

//移动窗口
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    mExitMenu->exec(QCursor::pos());
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mOffset = event->globalPos()-this->pos();//鼠标相对于窗口左上角的距离

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-mOffset);
}
//获取城市天气信息
void MainWindow::getWeatherInfo(QString cityCode)
{
    QUrl url("http://t.weather.itboy.net/api/weather/city/"+cityCode);
    mNetAccessManager->get(QNetworkRequest(url));
}
//处理json信息
void MainWindow::parseJson(QByteArray &byteArray)
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

    QJsonObject objData = rootObj.value("data").toObject();

    //解析forcast中五天的数据
    QJsonArray forecastArr = objData.value("forecast").toArray();

    QJsonObject objToday = forecastArr[0].toObject();
    mToday.week = objToday.value("week").toString();
    mToday.date = objToday.value("ymd").toString();
    mToday.type = objToday.value("type").toString();
    //高温低温
    QString s = objToday.value("high").toString().split(" ").at(1); //18°
    QString high = s.left(s.length()-1);//取到高温
    mToday.high = high.toInt();
    QString l = objToday.value("low").toString().split(" ").at(1); //18°
    QString low = l.left(l.length()-1);//取到低温
    mToday.low = low.toInt();
    //风向风力
    mToday.fx = objToday.value("fx").toString();
    mToday.fl = objToday.value("fl").toString();
    //污染指数
    mToday.aqi = objToday.value("api").toDouble();

    //解析今天的数据
    mToday.ganmao = objData.value("ganmao").toString();
    mToday.wendu = objData.value("wendu").toInt();
    mToday.shidu = objData.value("shidu").toString();
    mToday.pm25 = objData.value("pm25").toInt();
    mToday.quality = objData.value("quality").toString();

    //更新UI
    ui->lineEdit_city->setText(mToday.city);
    ui->label_cityWeather->setText(mToday.type+"   "+QString::number(mToday.low)+"-"+QString::number(mToday.high)+"℃");
}


void MainWindow::clearLayout(QLayout *layout){//清除布局管理器
    if(layout == nullptr) return;//布局是否存在？
    while(QLayoutItem *item = layout->takeAt(0)){
        if(QWidget *widget = item->widget()){
            widget->setParent(nullptr);//将该控件的父对象设置为空
            delete widget;//删除控件
        }else if (QLayout *childLayout = item->layout()){//如果布局中还存在布局
            clearLayout(childLayout);
            delete childLayout;
        }
        delete item;
    }
}

//选择的日期改变时
void MainWindow::on_dateEdit_dateChanged(const QDate &changedDate)
{
    int firstday_week = changedDate.addDays(-1*changedDate.day()+1).dayOfWeek();//获取跳转月份的第一天是周几
    drawCalendar(changedDate,firstday_week);
}


void MainWindow::on_pushButton_clicked()
{
    ui->dateEdit->setDate(QDate::currentDate());
}

//上一个月
void MainWindow::on_pushButton_left_clicked()
{
    ui->dateEdit->setDate(ui->dateEdit->date().addMonths(-1));

}

//下一个月
void MainWindow::on_pushButton_right_clicked()
{
    ui->dateEdit->setDate(ui->dateEdit->date().addMonths(1));
}

//选择年月
void MainWindow::on_pushButton_Year_clicked()
{
    ui->dateEdit->setVisible(true);
}

//选择完毕，不显示文本框
void MainWindow::on_dateEdit_editingFinished()
{
    ui->dateEdit->setVisible(false);
    qDebug("finish");
}
void MainWindow::on_pushButton_help_clicked()
{
    if(!isVisi){
        ui->groupBox_reminderLeft->setVisible(isVisi);
        isVisi = 1;
    }
    if(isVisi){
        ui->groupBox_reminderLeft->setVisible(isVisi);
        isVisi = 0;
    }
    qDebug()<<"已加载";
}
//弹出日程查看窗口
void MainWindow::outputName(QPushButton *button)
{
    qDebug()<<"Button clicked:"<<button->objectName();
    Dialog * dialog = new Dialog;
    QObject::connect(this,&MainWindow::sendWhoClicked,dialog,&Dialog::handleWhoClicked);
    emit sendWhoClicked(button->objectName());
    QObject::connect(this,&MainWindow::sendReminderData,dialog,&Dialog::handleReminderData);
    emit sendReminderData(reminderData);
    dialog->show();
    qDebug()<<"日程信息已发送";
    connect(dialog,&Dialog::windowClosed,this,&MainWindow::onWindowClosed);
}

void MainWindow::onWindowClosed()
{
    int firstday_week = selectedDate.addDays(-1*selectedDate.day()+1).dayOfWeek();//获取跳转月份的第一天是周几
    drawCalendar(selectedDate,firstday_week);
    qDebug()<<"关闭窗口后，当前日期为："<<selectedDate.toString("yyyy-MM-dd");
}
//弹出天气窗口
void MainWindow::on_pushButton_weather_clicked()
{
    Weather * configWindow = new Weather;
    configWindow->show();
}

//弹出日程窗口
void MainWindow::on_pushButton_reminders_clicked()
{
    Reminder * reminder = new Reminder;
    QObject::connect(this,&MainWindow::sendMessage,reminder,&Reminder::receiveMessage);
    reminder->show();
    emit sendMessage(selectedDate.toString("yyyy-MM-dd"));
    qDebug()<<"信息已发送："<<selectedDate.toString("yyyy-MM-dd");
    QObject::connect(reminder,&Reminder::windowClosed,this,&MainWindow::onWindowClosed);
}

void MainWindow::on_pushButton_openTheWholeYear_clicked()
{
    TheWholeYear *thewholeyear = new TheWholeYear;
    connect(this,&MainWindow::sendMDate,thewholeyear,&TheWholeYear::receiveData);
    emit sendMDate(selectedDate);
    thewholeyear->show();

}

