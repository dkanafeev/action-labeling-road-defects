#ifndef CHARTVIEWER_H
#define CHARTVIEWER_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "qcustomplot.h"

class ChartViewer : public QVBoxLayout
{
    Q_OBJECT
    QDOC_PROPERTY(QString fileName READ filename WRITE openFile)
public:
    explicit ChartViewer(QWidget *parent = 0);

    void redraw(qint64 speed);
    void openFile(QString filename);

    QString getFileName  () {return filename;}
    QTime   getLastTime  () {return lastTime;}
    QTime   getNextTime  () {return nextTime;}

public slots:
    void onHideClicked();
    void onCloseClicked();

private:
    void setupUi();
    void openFileFailed();
    void readFile();
    void readFirstLine();
    qint64 msecFromQTime(QTime time);

    // UI elements
    QPushButton *closeButton;
    QPushButton *hideButton;
    QLabel      *filenameLabel;
    QCustomPlot *chartPlot;
    QHBoxLayout *buttonLayout;

    // file vars
    QString filename;
    QFile   *file;

    // time vars
    QElapsedTimer timer;
    QTime lastTime;
    QTime nextTime;

    // data vars
    qint64 currentRecord;
    qint64 recordCounter;
    const qint64 rangeSize = 1000;
    QStringList   labels;
    QList <QTime> times;
    QList <QList <double>> chartData;
    QList <QPen> lineColors;    //colors of line which will be added by playing
    QList <QPen> oldLineColors; //colors of line which will be preloaded
};

#endif // CHARTVIEWER_H
