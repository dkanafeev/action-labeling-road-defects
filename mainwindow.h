#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QElapsedTimer>
#include <QTimer>
#include "chartviewer.h"
#include "mapviewer.h"
#include "videoviewer.h"

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
    void redrawSignal(double speed, double timer);
    void onSpeedChanged(double speed);
    void positionChanged(qint64 position);

private slots:

    void onPlayClicked();
    void onStopClicked();
    void onSpeedUpClicked();
    void onSpeedDownClicked();
    void onOpenClicked();
    void onGotoTimeClicked();

    void onTimerSignal();       // signal from timer for redraw
    void onTimelineEndSignal(); // signal from viewer that timeline of viewer is end

private:
    double speed;
    qint64 endCounter;
    const double minSpeed = 0.5;
    const qint64 timerDelay = 0;
    Ui::MainWindow *ui;

    // timers
    QTimer  *dataTimer; // for emit redraw signals
    qint64   savedTime;           // saved time when stop
    QElapsedTimer timer;  // for count elapsed time

    // viewers
    MapViewer* mapViewer;
    VideoViewer* videoViewer;
    QList <ChartViewer*> chartList;
};

#endif // MAINWINDOW_H
