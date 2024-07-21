#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialwidget.h"
#include "msgdb.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refreshSerialPort();
protected slots:
    void onBtnAdd(bool);
    void onBtnRemove(bool);
private:
    QList<SerialWidget*> serialWidgets;
    MsgDB db;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
