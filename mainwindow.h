#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QTimer>
#include "chartviewer.h"
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onPlayClicked();
    void onStopClicked();
    void onSpeedUpClicked();
    void onSpeedDownClicked();
    void onOpenClicked();
    void onTimerSignal();

private:
    qint64 speed;
    const qint64 timerDelay = 5;
    Ui::MainWindow *ui;
    QTimer  *dataTimer;
    QList <ChartViewer*> chartList;
};

#endif // MAINWINDOW_H
