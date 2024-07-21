#include "serialwidget.h"
#include <QDateTime>
#include <QChar>

SerialWidget::SerialWidget(const char* portName,MsgDB* db,QSerialPort* sport,QWidget *parent)
    : QWidget(parent),port(sport),pDB(db)
{
    connect(port, SIGNAL(readyRead()), this, SLOT(serialReadyRead()));
    mainLayout=new QVBoxLayout(this);
    setLayout(mainLayout);
    msgList=new QListWidget(this);

    lineSpin=new QSpinBox(this);
    lineSpin->setRange(0,1000);
    lineSpin->setValue(100);
    cfgLayout=new QHBoxLayout(this);

    dateCheck=new QCheckBox("time",this);
    cfgLayout->addWidget(dateCheck);
    cfgLayout->addWidget(msgList);
    cfgLayout->addWidget(lineSpin);
    mainLayout->addLayout(cfgLayout);
    mainLayout->addWidget(msgList);
    strcpy(this->portName,portName);

    dispCheck=new  QCheckBox("display",this);
    cfgLayout->addWidget(dispCheck);

    recordCheck=new  QCheckBox("record",this);

    enterCheck=new  QCheckBox("rx tx enter",this);
    cfgLayout->addWidget(recordCheck);
    pDB->addTable(portName);
    recordCheck->setChecked(true);
    dateCheck->setChecked(true);
    dispCheck->setChecked(true);

    cfgLayout->addWidget(enterCheck);
    outputLayout=new QHBoxLayout(this);

    outputLinEdit=new QLineEdit(this);
    sendBtn=new QPushButton("send",this);
    outputLayout->addWidget(outputLinEdit);
    outputLayout->addWidget(sendBtn);
    mainLayout->addLayout(outputLayout);
    connect(sendBtn,&QPushButton::clicked,this,&SerialWidget::onSendData);
}

SerialWidget::~SerialWidget()
{
    if(port)
    {
        port->close();
        delete port;
    }
}
void SerialWidget::onSendData(bool)
{
    QByteArray arr=outputLinEdit->text().toUtf8();
    if(enterCheck->isChecked())
        arr.append('\n');
    port->write(arr);
}
void SerialWidget::serialReadyRead()
{
    QByteArray data=port->readAll();

    if(recordCheck->isChecked())
        pDB->addMsg(portName,data.data(),data.size());
    if(dispCheck->isChecked())
    {
        QString msg=data;
        for(int i=0;i<msg.size();i++)
        {
            if(msg.at(i)<QChar(0x20))
                msg[i]=QChar(0x20);
        }

        if(dateCheck->isChecked())
        {
            QDateTime date=QDateTime::currentDateTime();
            QString datetime=date.toString("yyyy-MM-dd HH:mm:ss:");
            if(enterCheck->isChecked())
            {
                QStringList strList=msg.split('\n');
                int count=strList.size();
                for(int i=0;i<count;i++)
                {
                    msgList->addItem(datetime.append(strList.at(i)));
                }
            }
            else
            {
                msgList->addItem(datetime.append(msg));
            }
        }
        else
        {
            if(enterCheck->isChecked())
            {
                QStringList strList=msg.split('\n');
                msgList->addItems(strList);
            }
            else
            {
                msgList->addItem(msg);
            }
        }


        //msgList->addItem(msg.append("too long"));
        //else


        //msg=msg.replace(0x1b,'|');

        while(msgList->count()>lineSpin->value())
        {
            QListWidgetItem* item=msgList->takeItem(0);
            msgList->removeItemWidget(item);
            delete item;
        }
    }

}
