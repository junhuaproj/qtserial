#ifndef SERIALWIDGET_H
#define SERIALWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QSpinBox>
#include <qserialport.h>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include "msgdb.h"

class SerialWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerialWidget(const char* portName,MsgDB* db,QSerialPort* port,QWidget *parent = nullptr);
    virtual ~SerialWidget();
protected slots:
    void serialReadyRead();
    void onSendData(bool);
protected:
    QSerialPort* port;
    MsgDB* pDB;
    char portName[32];

    QVBoxLayout* mainLayout;
    QListWidget* msgList;

    QHBoxLayout* cfgLayout;
    QSpinBox* lineSpin;
    QCheckBox* dateCheck;
    QCheckBox* dispCheck;
    QCheckBox* recordCheck;
    QCheckBox* enterCheck;


    QHBoxLayout* outputLayout;

    QLineEdit* outputLinEdit;
    QPushButton* sendBtn;
signals:
};

#endif // SERIALWIDGET_H
