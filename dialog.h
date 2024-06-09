/************************************************
 *
 * 模块名称：dialog.h
 * 功能描述：详细日程信息窗口类的设计
 *
 *
 */
#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
    QString objectName;
    QString date;
    int eventNum,whoClicked;
signals:
    void windowClosed();

public slots:
    void handleReminderData(const QStringList data);
    void handleWhoClicked(const QString &name);
private slots:
    void on_pushButton_change_clicked();
    void on_pushButton_delete_clicked();
};

#endif // DIALOG_H
