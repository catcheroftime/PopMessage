#include "messageshow.h"
#include "ui_messageshow.h"

MessageShow::MessageShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageShow)
  , m_takeurl(false)
  , m_first_show(true)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::SubWindow);

    this->setFixedSize(240,120);

    setUiStyle();

    timer_show = new QTimer(this);
    timer_stay = new QTimer(this);
    timer_close = new QTimer(this);
    time_count = 0;
    transparent = 1.0;
    desktop_height = QApplication::desktop()->height();

    notify_sum = 0;

    connect(timer_show, SIGNAL(timeout()),  this, SLOT(MsgMove()));
    connect(timer_stay, SIGNAL(timeout()),  this, SLOT(MsgStay()));
    connect(timer_close, SIGNAL(timeout()), this, SLOT(MsgClose()));
}

MessageShow::~MessageShow()
{
    deleteTimer(timer_show);
    deleteTimer(timer_stay);
    deleteTimer(timer_close);
    delete ui;
}



void MessageShow::setInfomation(QString titleInfo, QString msg, bool hidetitle)
{
    if(hidetitle)
    {
        ui->widget_msgtitle->hide();
    }else
    {
        ui->widget_msgtitle->show();
    }
    ui->label_titleInfo->setText(titleInfo);


    ui->label_msg->setText(msg);
    ui->label_msg->setToolTip(msg);


    this->showMessage();
}


void MessageShow::setInfomation(QString titleInfo, QString msg, QString filepath, bool takeurl)
{
    ui->widget_msgtitle->show();
    if(takeurl)
    {
        ui->label_titleInfo->setText(titleInfo);
        ui->label_msg->setText(QString("<a style='color: gray;'href=\"")+filepath+QString("\">")+msg);
        connect(ui->label_msg,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    }else
    {
        ui->label_titleInfo->setText(titleInfo);
        ui->label_msg->setText(msg);
    }
    ui->label_msg->setToolTip(msg);
}

void MessageShow::openUrl(QString filepath)
{
    emit sigClickUrl(filepath);
}

void MessageShow::setUiStyle()
{
    ui->widget_msgtitle->setStyleSheet("border:0px solid rgb(10,10,10);"
                                       "color:#fff;"
                                       "background-color:rgb(10,10,10);"
                                       "font-size: 13px;"
                                       "font-family:\"Microsoft YaHei\";");

    ui->pushButton_close->setStyleSheet("QPushButton{image: url(./resource/images/close_n);}"
                                         "QPushButton:hover{image: url(./resource/images/close_h);}"
                                         "QPushButton:pressed{image: url(./resource/images/close_p);}");

    ui->label_msg->setStyleSheet("border:0px solid rgb(15,15,15);"
                                 "background-color:rgb(15,15,15);"
                                 "color:#fff;"
                                 "font-size: 12px;"
                                 "font-family:\"Microsoft YaHei\";");
}

void MessageShow::deleteTimer(QTimer *timer)
{
    if(timer->isActive())
        timer->stop();

    delete timer;
    timer = NULL;
}




void MessageShow::on_pushButton_close_clicked()
{
    timer_show->stop();
    timer_stay->stop();
    timer_close->stop();
    emit sigClose(this);
}

void MessageShow::showMessage()
{
    time_count = 0;
    transparent = 1.0;
    desktop_height = QApplication::desktop()->height();
    timer_show->stop();
    timer_stay->stop();
    timer_close->stop();
    setWindowOpacity(1);
    QDesktopWidget *desktop = QApplication::desktop();
    QRect desk_rect = desktop->availableGeometry();
    normal_point.setX(desk_rect.width() - rect().width());
    normal_point.setY(desk_rect.height() - rect().height());
    move(normal_point.x(), normal_point.y());
    showNormal();
    timer_show->start(3);
}

void MessageShow::MsgMove()
{
    desktop_height--;
    move(normal_point.x(), desktop_height);
    if(desktop_height <= ( normal_point.y() - notify_sum*(this->height()+3) ) )
    {
        timer_show->stop();
        timer_stay->start(800);
    }
}

void MessageShow::MsgStay()
{
    time_count++;
    if(time_count >= 3)
    {
        timer_stay->stop();
        timer_close->start(100);
    }
}

void MessageShow::MsgClose()
{
    transparent -= 0.2;
    if(transparent <= 0.0)
    {
        timer_close->stop();
        emit sigClose(this);
    }
    else
    {
        setWindowOpacity(transparent);
    }
}

void MessageShow::updatePosition()
{
    if(m_first_show){
        this->showMessage();
        m_first_show = false;
    }else{
        notify_sum += 1;
        desktop_height = desktop_height - this->height()-3;
        move(normal_point.x(), desktop_height);
    }
}
