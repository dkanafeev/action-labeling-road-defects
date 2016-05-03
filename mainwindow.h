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

signals:
    void redrawSignal(double speed);

private slots:
    void onPlayClicked();
    void onStopClicked();
    void onSpeedUpClicked();
    void onSpeedDownClicked();
    void onOpenClicked();
    void onTimerSignal();

private:
    double speed;
    const double minSpeed = 0.5;
    const qint64 timerDelay = 2;
    Ui::MainWindow *ui;
    QTimer  *dataTimer;
    QList <ChartViewer*> chartList;
};

#endif // MAINWINDOW_H
