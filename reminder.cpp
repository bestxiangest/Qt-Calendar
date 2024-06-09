/************************************************
 *
 * 模块名称：reminder.cpp
 * 功能描述：日程功能具体实现
 *
 *
 */
#include "reminder.h"
#include "ui_reminder.h"
#include <QDate>
#include <mainwindow.h>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QTime>

Reminder::Reminder(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reminder)
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

    //connect(ui->timeEdit_selectTime,SIGNAL(timeChanged(QTime)),this,SLOT(timeChanged(QTime &time)));
}

Reminder::~Reminder()
{
    delete ui;
}
void Reminder::contextMenuEvent(QContextMenuEvent *event)
{
    //弹出右键菜单
    mExitMenu->exec(QCursor::pos());
    event->accept();
}

void Reminder::mousePressEvent(QMouseEvent *event)
{
    mOffset = event->globalPos()-this->pos();
}

void Reminder::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos()-mOffset);
}

void Reminder::receiveMessage(const QString message)
{
    qDebug()<<message<<"信息已接收";
    fileName = message;
    //显示日期
    ui->label_Date->setText(message);
    connect(this,&Reminder::show_succeed,this,&Reminder::openfile);
    emit show_succeed();

}

void Reminder::openfile(){
    qDebug()<<"open_file"<<fileName<<".json";
}

void Reminder::checkAndWriteFile(const QString fileName)
{
    QString title = ui->plainTextEdit_Title->toPlainText();
    QString content = ui->plainTextEdit_Content->toPlainText();

    QFile jsonFile(QString(".\\scheduleFile\\%1.json").arg(fileName));

    if (jsonFile.exists()) {
        // 文件存在，以读写模式打开
        if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"无法打开文件：文件存在，以只读方式打开";
            return;
        }
        QByteArray jsonData = jsonFile.readAll();
        jsonFile.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if(!jsonDoc.isObject())qDebug()<<"内容形式不正确，无法追加日程";
        //获取文件中的对象和数组
        QJsonObject jsonObject = jsonDoc.object();
        QJsonArray eventArray = jsonObject.value("events").toArray();
        //追加日程
        QJsonObject eventObject;
        eventObject["time"] = ui->timeEdit_selectTime->time().toString("HH:mm");
        qDebug()<<eventObject["time"].toString()<<"finished";
        eventObject["title"] = title;
        eventObject["content"] = content;

        //将新日程加入array
        eventArray.append(eventObject);
        jsonObject["events"] = eventArray;//更新对象中的数组
        jsonDoc.setObject(jsonObject);//更新文档中的对象

        if(jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            // 将JSON文档写入文件
            jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
            jsonFile.close();
            qDebug()<<"日程已经追加到文件中。";
        } else {
            // 打开文件失败的处理
            qDebug() << "无法打开文件：" << jsonFile.errorString();
        }
    } else {
        // 文件不存在，创建并写入
        if (jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // 同上，创建JSON对象并写入文件
            QJsonObject jsonObject;
            jsonObject["date"] = fileName;

            QJsonObject eventObject;
            eventObject["time"] = ui->timeEdit_selectTime->time().toString("HH:mm");
            eventObject["title"] = title;
            eventObject["content"] = content;

            QJsonArray eventArray;
            eventArray.append(eventObject);

            jsonObject["events"] = eventArray;

            QJsonDocument jsonDoc(jsonObject);
            jsonFile.write(jsonDoc.toJson());

            jsonFile.close();
            qDebug()<<"日程已添加到文件中。";
        } else {
            // 创建或打开文件失败的处理
            qDebug() << "无法创建或打开文件：" << jsonFile.errorString();
        }
    }
}

void Reminder::timeChanged(QTime &time)
{
    selectedTime = time;
}


void Reminder::on_pushButton_save_clicked()
{
    QString title = ui->plainTextEdit_Title->toPlainText();
    QString content = ui->plainTextEdit_Content->toPlainText();
    checkAndWriteFile(fileName);
    this->close();
    emit windowClosed();

}

void Reminder::on_timeEdit_selectTime_userTimeChanged(const QTime &time)
{
    return;
}

