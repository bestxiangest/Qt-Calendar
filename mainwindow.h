/************************************************
 *
 * 模块名称：mainwindow.h
 * 功能描述：主窗口类的设计
 *
 *
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <weather.h>
#include <weatherdata.h>
#include <reminder.h>
#include <dialog.h>
#include <QVariantMap>
#include <QTimer>
#include <QTime>
#include <QThread>
#include <QFile>
#include <QVBoxLayout>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_dateEdit_dateChanged(const QDate &date);
    void on_pushButton_clicked();
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
    void on_pushButton_Year_clicked();
    void on_dateEdit_editingFinished();
    void daybuttonclicked();
    void on_pushButton_weather_clicked();
    void on_pushButton_reminders_clicked();
    void on_pushButton_help_clicked();
    //void readReminders();
    void outputName(QPushButton *button);
    void onWindowClosed();

    void on_pushButton_openTheWholeYear_clicked();

public slots:
    void OnTimerOutFunc();
    void onReplied(QNetworkReply * reply);

private:
    Ui::MainWindow *ui;
    void iniUI();
    void drawCalendar(QDate nowdate,int firstday_week);
    void findReminder(QString fileName);
    QNetworkAccessManager * qNwkHttp;
    QTimer qTimer;
    QMenu * mExitMenu;
    QAction * mExitAct;
    QPoint mOffset;//窗口移动时鼠标与窗口左上角的偏移量

    QNetworkAccessManager *mNetAccessManager;

    Today mToday;
    QDate selectedDate;
    QFile file;
    QWidget *widget;
    QVBoxLayout *layout;
    QStringList reminderData;


public:
    //获取当天的天气预报数据
    void Get_CurrentWeather(QNetworkReply * qReply);
    //获取三天内的天气预报数据
    void Get_ThreeWearher(QNetworkReply * qReply);
    //检测文件是否存在
    void file_isExist(QFile * file);
    QDate seleDate(){
        return selectedDate;
    }
    void clearLayout(QLayout *layout);
    QString getSelectedDate(){
        return selectedDate.toString("yyyy-MM-dd");
    }

signals:
    void sendMessage(const QString message);
    void sendReminderData(const QStringList);
    void sendWhoClicked(const QString &name);
    void sendMDate(const QDate date);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void getWeatherInfo(QString cityCode);
    void parseJson(QByteArray & byteArray);

};
#endif // MAINWINDOW_H
