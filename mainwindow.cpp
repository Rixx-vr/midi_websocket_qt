#include "mainwindow.h"
#include "qdebug.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDeviceList(QMap<QString, QString> devices)
{
    foreach (QString device, devices.keys()) {
        ui->devices->addItem(devices[device], device);
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString device = ui->devices->currentData().toString();
    if (this->connected) {
        emit this->onDisconnect();
        this->ui->pushButton->setText("connect");
        this->connected = false;
    } else {
        emit this->onDeviceSelected(device);
        emit this->onPortSelected((quint16) ui->port->value());
        this->connected = true;
        this->ui->pushButton->setText("disconnect");
    }
}

