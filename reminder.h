/************************************************
 *
 * 模块名称：reminder.h
 * 功能描述：日程窗口类的设计
 *
 *
 */
#ifndef REMINDER_H
#define REMINDER_H

#include <QWidget>
#include <QMenu>
#include <qevent.h>
#include <QTime>

namespace Ui {
class Reminder;
}

class Reminder : public QWidget
{
    Q_OBJECT

public:
    explicit Reminder(QWidget *parent = nullptr);
    ~Reminder();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QMenu * mExitMenu;//右键退出菜单
    QAction * mExitAct;//退出行为
    QPoint mOffset;
    QString fileName;
    QTime selectedTime;

private:
    Ui::Reminder *ui;

public slots:
    void receiveMessage(const QString message);
    void openfile();
    void checkAndWriteFile(const QString fileName);
    void timeChanged(QTime &time);

signals:
    void show_succeed();
    void windowClosed();
private slots:
    void on_pushButton_save_clicked();
    void on_timeEdit_selectTime_userTimeChanged(const QTime &time);
};

#endif // REMINDER_H
