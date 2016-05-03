#ifndef CHARTVIEWER_H
#define CHARTVIEWER_H

#include "dataviewer.h"
#include "qcustomplot.h"

class ChartViewer : public DataViewer
{
    Q_OBJECT
public:
    explicit ChartViewer(QWidget *parent = 0);

public slots:
    void onHidePressed();
    void onClosePressed();

private:
    void startPreparationToDraw();
    void updateViewer();

    QColor getColor(int idColor);
    qint64 msecFromQTime(QTime time);

    // chart vars
    const double rangeXSize = 1000.0;
    const double rangeYSize = 20.0;
    const int plotHeight = 200;
    QCustomPlot *chartPlot;
    QList <QPen> lineColors;
};

#endif // CHARTVIEWER_H
