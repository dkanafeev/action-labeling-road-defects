#ifndef CHARTVIEWER_H
#define CHARTVIEWER_H

#include "dataviewer.h"
#include "qcustomplot.h"

class ChartViewer : public DataViewer
{
    Q_OBJECT
public:
    explicit ChartViewer(QWidget *parent = 0);

protected slots:
    void onHidePressed();
    void onClosePressed();
    void onIncreaseRangeYPressed();
    void onDecreaseRangeYPressed();

protected:
    QPushButton *decreaseRangeYButton;
    QPushButton *increaseRangeYButton;
    void startPreparationToDraw();
    void updateViewer(bool isForward);

    QColor getColor(int idColor);
    qint64 msecFromQTime(QTime time);

    // chart vars
    quint16 rangeXSize = 1000.0;
    quint16 minRangeYSize = 1.0;
    quint16 maxRangeYSize = 1024.0;
    quint16 rangeYSize = 32.0;
    int plotHeight = 200;
    QCustomPlot *chartPlot;
    QList <QPen> lineColors;

};

#endif // CHARTVIEWER_H
