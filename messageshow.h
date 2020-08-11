#ifndef MESSAGESHOW_H
#define MESSAGESHOW_H

#include <QDialog>
#include <QTimer>
#include <QDesktopWidget>
#include <QPoint>
#include <QDebug>

namespace Ui {
class MessageShow;
}

class MessageShow : public QDialog
{
    Q_OBJECT

public:
    MessageShow(QWidget *parent = 0);
    ~MessageShow();

    void setInfomation(QString titleInfo, QString msg, bool hidetitle = false);
    void setInfomation(QString titleInfo, QString msg, QString filepath, bool takeurl);

    void updatePosition();

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;


signals:
    void sigClickUrl(QString);
    void sigClose(MessageShow *);

private slots:
    void on_pushButton_close_clicked();
    void MsgMove();
    void MsgStay();
    void MsgClose();
    void openUrl(QString filepath);
private:
    void showMessage();
    void setUiStyle();
    inline void deleteTimer(QTimer * timer);
private:
    Ui::MessageShow *ui;

    QTimer* timer_show;
    QTimer* timer_stay;
    QTimer* timer_close;

    int time_count;
    double transparent;
    int desktop_height;
    bool m_takeurl;

    QPoint normal_point;
    bool m_first_show;

    bool m_enterEvent;

    int notify_sum;

};

#endif // MESSAGESHOW_H
