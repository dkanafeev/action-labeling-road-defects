#include "chartviewer.h"

ChartViewer::ChartViewer(QWidget *parent) : DataViewer(parent)
{
    //setup plot
    chartPlot = new QCustomPlot();
    chartPlot->setMinimumHeight(plotHeight);
    chartPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    // setup plot axes
    chartPlot->xAxis->setLabel("timeline");
    chartPlot->xAxis->setNumberFormat("f");
    chartPlot->xAxis->setNumberPrecision(0);
    chartPlot->yAxis->setLabel("values");
    chartPlot->yAxis->setRange(-rangeYSize, rangeYSize);
    viewerLayout->addWidget(chartPlot);

    this->openButton->hide();
}

void ChartViewer::updateViewer()
{
    // update each line
    for (quint64 i = 0 ; i < lineNumber; ++i)
    {
        chartPlot->graph(i)->rescaleKeyAxis();
        chartPlot->xAxis->setRange(timeData[nextRecord], rangeXSize, Qt::AlignCenter);
    }
    chartPlot->replot();
}

void ChartViewer::startPreparationToDraw()
{
    // preload all data to chart
    for (quint64 i = 0; i < lineNumber; i++)
    {
        chartPlot->addGraph(chartPlot->xAxis, chartPlot->yAxis);
        chartPlot->graph(i)->rescaleKeyAxis();
        chartPlot->graph(i)->setPen(getColor(i));
        chartPlot->graph(i)->addData(timeData, viewerData[i]);
    }

    // vertical line which mean current
    QCPItemLine *verticalLine = new QCPItemLine(chartPlot);
    chartPlot->addItem(verticalLine);
    verticalLine->start->setType( QCPItemPosition::PositionType::ptAxisRectRatio );
    verticalLine->start->setCoords(0.5, 0);
    verticalLine->end->setType( QCPItemPosition::PositionType::ptAxisRectRatio );
    verticalLine->end->setCoords(0.5, 1);
    verticalLine->setPen(QPen(QBrush(Qt::red), 3));

    chartPlot->xAxis->setRange(timeData.first(), rangeXSize, Qt::AlignCenter);
    chartPlot->replot();
}

QColor ChartViewer::getColor(int idColor)
{
    double frequency = 2*3.14157/6;
    quint8 red   = qSin(frequency * idColor + 0) * 127 + 128;
    quint8 green = qSin(frequency * idColor + 2) * 127 + 128;
    quint8 blue  = qSin(frequency * idColor + 4) * 127 + 128;
    return QColor (red, green, blue);
}

void ChartViewer::onHidePressed()
{
    chartPlot->setVisible(chartPlot->isVisible() ? false : true);
}

void ChartViewer::onClosePressed()
{
    QString fname = "ChartViewer::onClosePressed() => ";
    qDebug() << fname + "started";
    // send signal to MainWindow that this chartViewer is removed
    // there should be destructor call, which hasn't write yet
    qDebug() << fname + "end";
}
