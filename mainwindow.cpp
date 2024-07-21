#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_data_bit->addItems({"5","6","7","8"});
    ui->comboBox_stopbit->addItems({"1","1.5","2"});
    ui->comboBox_parity->addItems({"NoParity","EvenParity",
        "OddParity","SpaceParity","MarkParity"});
    ui->comboBox_bandrate->addItems({"1200","2400",
            "4800","9600","19200","38400","57600","115200"});
    refreshSerialPort();
    setLayout(ui->horizontalLayout_main);
    connect(ui->pushButton_add,SIGNAL(clicked(bool)),this,SLOT(onBtnAdd(bool)));
    connect(ui->pushButton_remove,SIGNAL(clicked(bool)),this,SLOT(onBtnRemove(bool)));
    ui->comboBox_data_bit->setCurrentIndex(3);
    ui->comboBox_bandrate->setCurrentIndex(4);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::refreshSerialPort()
{
    QList<QSerialPortInfo> infos= QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::const_iterator it=infos.constBegin(),end=infos.constEnd();
    for(;it!=end;it++)
    {
        ui->comboBox_port->addItem((*it).portName());
        //serialPort.setPortName("COM5");
        //serialPort.setBaudRate(115200);
        //serialPort.setDataBits(QSerialPort::Data8);
        //serialPort.setParity(QSerialPort::NoParity);
        //serialPort.setStopBits(QSerialPort::OneStop);
    }
}

void MainWindow::onBtnAdd(bool)
{
    QSerialPort* port=new QSerialPort();
    QString portName=ui->comboBox_port->currentText();
    port->setPortName(portName);
    port->setBaudRate(ui->comboBox_bandrate->currentText().toInt());
    port->setParity((QSerialPort::Parity)ui->comboBox_parity->currentIndex());
    port->setDataBits((QSerialPort::DataBits)(ui->comboBox_data_bit->currentIndex()+5));
    port->setStopBits((QSerialPort::StopBits)ui->comboBox_stopbit->currentIndex());
    if(port->open(QIODevice::ReadWrite))
    {
        SerialWidget* serial=new SerialWidget(portName.toStdString().c_str(),&db,port,this);
        int size=serialWidgets.size();
        if(size>1)
        {
            ui->gridLayout_serialwidget->addWidget(serial,size%2,size/2);
        }
        else
            ui->gridLayout_serialwidget->addWidget(serial);
        serialWidgets.append(serial);
        QListWidgetItem* item=new QListWidgetItem(portName);
        item->setData(Qt::UserRole+1,(qlonglong)serial);
        ui->listWidget->addItem(item);
    }
    else
    {
        //qDebug()<<port->errorString();
        QMessageBox::about(this,"error",port->errorString());
        delete port;
    }

}
void MainWindow::onBtnRemove(bool)
{
    QList<QListWidgetItem*> items=ui->listWidget->selectedItems();
    QList<QListWidgetItem*>::reverse_iterator it=items.rbegin(),end=items.rend();
    SerialWidget* serial;
    for(;it!=end;it++)
    {
        serial=(SerialWidget*)qvariant_cast<uint64_t>((*it)->data(Qt::UserRole+1));
        serialWidgets.removeOne(serial);
        delete serial;
        ui->listWidget->removeItemWidget(*it);
        delete *it;
    }
}
