#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "popupmanage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ptn_show_clicked()
{
    PopupManage::getInstance()->setInfomation("错误", "saas as ", false);
}
