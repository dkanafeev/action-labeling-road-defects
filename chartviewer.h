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
    double  getLastTime  () {return lastTime;}
    double  getNextTime  () {return nextTime;}

public slots:
    void onHideClicked();
    void onCloseClicked();

private:
    void setupUi();
    void openFileFailed();
    void readFile();
    void startPreparationToDraw();
    void updatePlot(bool isForward = true);
    QColor getColor(int idColor);
    qint64 msecFromQTime(QTime time);

    // UI elements
    QPushButton *closeButton;
    QPushButton *hideButton;
    QLabel      *filenameLabel;
    QCustomPlot *chartPlot;
    QHBoxLayout *buttonLayout;

    // chart vars
    const double rangeXSize = 1000.0;
    const double rangeYSize = 20.0;
    const int plotHeight = 200;
    QList <QPen> lineColors;

    // file vars
    QString filename;
    QFile   *file;

    // info vars
    QStringList   labels;
    quint64       lineNumber; // = labels.size() - 1, w/o time line

    // time vars
    QElapsedTimer timer;
    double lastTime;
    double nextTime;

    // data vars
    qint64 nextRecord;
    qint64 recordCounter;
    QVector <double> timeData;
    QList <QVector <double>> chartData;
};

#endif // CHARTVIEWER_H
