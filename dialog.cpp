/************************************************
 *
 * 模块名称：dialog.cpp
 * 功能描述：详细日程信息窗口功能具体实现
 *
 *
 */
#include "dialog.h"
#include "ui_dialog.h"
#include <mainwindow.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("编辑日程");

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::handleReminderData(QStringList reminderData)//第一个元素为length，第二个元素[1]为title，[2]为content，以此类推
{
    eventNum = reminderData.at(0).toInt();//共有多少日程
    whoClicked = objectName.toInt();//获取被点击的是第几个日程
    QString title = reminderData.at(whoClicked*3-2);
    QString content = reminderData.at(whoClicked*3-1);
    QString time = reminderData.at(whoClicked*3);
    ui->textEdit_title->setText(title);
    ui->textEdit_content->setText(content);
    ui->timeEdit->setTime(QTime::fromString(time));
}

void Dialog::handleWhoClicked(const QString &name)
{
    objectName = name.split(" ").at(0);
    date = name.split(" ").at(1);
}

void Dialog::on_pushButton_change_clicked()
{
    QString fileName = date;
    QFile jsonFile(QString(".\\scheduleFile\\%1.json").arg(fileName));

    if (jsonFile.exists()) {
        // 文件存在，以读写模式打开
        if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"无法打开文件：文件存在，以只读方式不能打开";
            return;
        }
        QByteArray jsonData = jsonFile.readAll();
        jsonFile.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if(!jsonDoc.isObject())qDebug()<<"内容形式不正确，无法更改日程";
        //获取文件中的对象和数组
        QJsonObject jsonObject = jsonDoc.object();
        QJsonArray eventArray = jsonObject.value("events").toArray();
        //修改日程*****************************
        QJsonObject eventObject;
        eventObject["time"] = ui->timeEdit->time().toString("HH:mm");
        eventObject["title"] = ui->textEdit_title->toPlainText();
        eventObject["content"] = ui->textEdit_content->toPlainText();

        //修改元素：
        int indexToModify = whoClicked - 1;
        if(indexToModify >= 0 && indexToModify < eventArray.size()){
            eventArray[indexToModify] = eventObject;
        }
        jsonObject["events"] = eventArray;//更新对象中的数组
        jsonDoc.setObject(jsonObject);//更新文档中的对象


        if(jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            // 将JSON文档写入文件
            jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
            jsonFile.close();
            qDebug()<<"日程已经在文件中删除。";
        } else {
            // 打开文件失败的处理
            qDebug() << "无法打开文件：" << jsonFile.errorString();
        }
    }
    this->close();
    emit windowClosed();
}


void Dialog::on_pushButton_delete_clicked()
{
    QString fileName = date;
    QFile jsonFile(QString(".\\scheduleFile\\%1.json").arg(fileName));

    if (jsonFile.exists()) {
        // 文件存在，以读写模式打开
        if (!jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<"无法打开文件：文件存在，以只读方式不能打开";
            return;
        }
        QByteArray jsonData = jsonFile.readAll();
        jsonFile.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if(!jsonDoc.isObject())qDebug()<<"内容形式不正确，无法更改日程";
        //获取文件中的对象和数组
        QJsonObject jsonObject = jsonDoc.object();
        QJsonArray eventArray = jsonObject.value("events").toArray();

        //修改元素：
        int indexToModify = whoClicked - 1;
        if(indexToModify >= 0 && indexToModify < eventArray.size()){
            eventArray.removeAt(indexToModify);
        }
        jsonObject["events"] = eventArray;//更新对象中的数组
        jsonDoc.setObject(jsonObject);//更新文档中的对象


        if(jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)){
            // 将JSON文档写入文件
            jsonFile.write(jsonDoc.toJson(QJsonDocument::Indented));
            jsonFile.close();
            qDebug()<<"日程已经在文件中修改。";
        } else {
            // 打开文件失败的处理
            qDebug() << "无法打开文件：" << jsonFile.errorString();
        }
    }
    this->close();
    emit windowClosed();
}
