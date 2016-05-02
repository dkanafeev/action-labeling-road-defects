#include "chartviewer.h"

ChartViewer::ChartViewer(QWidget *parent) : QVBoxLayout(parent)
{
    this->setupUi();
}

void ChartViewer::setupUi()
{
    QString fname = "ChartViewer::setupUi() => ";
    qDebug() << fname + "started";
    //setup buttons
    hideButton = new QPushButton("Hide");
    connect(hideButton, SIGNAL(pressed()), this, SLOT(onHideClicked()));
    closeButton = new QPushButton("Close");
    connect(closeButton, SIGNAL(pressed()), this, SLOT(onCloseClicked()));

    //setup label
    filenameLabel = new QLabel("");

    //setup plot
    chartPlot = new QCustomPlot();
    chartPlot->setMinimumHeight(200);
    chartPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    // setup the axes
    chartPlot->xAxis->setLabel("timeline");
    chartPlot->xAxis->setNumberFormat("f");
    chartPlot->xAxis->setNumberPrecision(0);
    //chartPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    //chartPlot->xAxis->setDateTimeFormat("hh:mm:ss:zz");

    chartPlot->yAxis->setLabel("values");
    chartPlot->yAxis->setRange(-20.0, 20.0);

    // setup buttonLayot
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(hideButton);
    buttonLayout->addWidget(filenameLabel);
    buttonLayout->addWidget(closeButton);

    // add items to this chartViewer
    this->addLayout(buttonLayout);
    this->addWidget(chartPlot);
    qDebug() << fname + "end";
}

void ChartViewer::redraw(qint64 speed)
{
    QString fname = "ChartViewer::redraw() => ";
    qDebug() << fname + "started";

    // check timers
    // note: speed is unused!
    //if (lastTime + timer.elapsed() < nextTime)
    //{
    //    qDebug() << fname + "your time hasn't come yet! ended";
    //    return;
    //}

    //if it's time to redraw, start it!
    qint64 msecCurrentTime = msecFromQTime(times[currentRecord]);
    qDebug() << fname + "time is " + QString::number(msecCurrentTime);

    for (int i = 0 ; i < labels.size()-1; i++)
    {
        chartPlot->graph(i)->rescaleKeyAxis();
        chartPlot->xAxis->setRange(msecCurrentTime, rangeSize, Qt::AlignCenter);
        chartPlot->graph(i)->setPen(lineColors[i]);
        double toChart = chartData[i][currentRecord];
        qDebug() << fname + " add data " + QString::number(toChart) +
                            " to graph " + QString::number(i);
        chartPlot->graph(i)->addData(msecCurrentTime, toChart);
    }
    chartPlot->replot();

    ++currentRecord;
    if (currentRecord >= recordCounter)
        currentRecord = 0;

    lastTime = nextTime;
    nextTime = times[currentRecord];
    timer.restart();

    qDebug() << fname + "end";
}

void ChartViewer::openFile(QString filename)
{
    QString fname = "ChartViewer::openFile() => ";
    qDebug() << fname + "started";

    file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << fname + "there is no file: " + filename;
        openFileFailed();
    }
    else
    {
        this->filename = filename;
        filenameLabel->setText("Path: " + filename);
        readFile();
    }

    qDebug() << fname + "end";
}

void ChartViewer::openFileFailed()
{
    QString fname = "ChartViewer::openFileFailed() => ";
    qDebug() << fname + "started";
    qDebug() << fname + "end";
}

void ChartViewer::readFile()
{
    QString fname = "ChartViewer::readFile() => ";
    qDebug() << fname + "started";
    int _tmpLabelLineLenght = 0;

    currentRecord = 0;
    recordCounter = 0;

    // read first line which contain column labels
    QTextStream in(file);
    QString line = in.readLine();
    labels = line.split(',');
    foreach (const QString &str, labels) {
        qDebug() << fname + "found label: " + str;
        // create graph
        chartPlot->addGraph(chartPlot->xAxis, chartPlot->yAxis);
        double frequency = 2*3.14157/6;
        quint8 red   = qSin(frequency * _tmpLabelLineLenght + 0) * 127 + 128;
        quint8 green = qSin(frequency * _tmpLabelLineLenght + 2) * 127 + 128;
        quint8 blue  = qSin(frequency * _tmpLabelLineLenght + 4) * 127 + 128;
        lineColors.append(QPen(QBrush(QColor(red, green, blue)), 3));
        oldLineColors.append(QPen(QColor(red, green, blue, 128)));
        // create data lists
        if ( str != "time" ) //for time data: QList <QTime> times
        {
            QList <double> newList;
            chartData.append(newList);
        }

        //
        ++_tmpLabelLineLenght;
    }

    // read data
    while ( !( line = in.readLine() ).isNull())
    {
        QStringList _tmpSplitedLine = line.split(',');
        int _tmpDataLineLenght = _tmpSplitedLine.size();
        QString _tmpStr;

        if (_tmpDataLineLenght != _tmpLabelLineLenght)
            continue;

        ++recordCounter;
        for (int i = 0; i < _tmpDataLineLenght; ++i) {
            _tmpStr = _tmpSplitedLine.at(i);

            //check if time format
            //QRegExp regex("\\d+:\\d+:\\d+:\\d+");
            //if (regex.exactMatch(_tmpStr))
            if ( i == 0 )
            {
                // first should be a time
                times.append(QTime::fromString(_tmpStr, "h:m:s:z"));
            }
            else
            {
                // it's another data
                double _tmpData = _tmpStr.toDouble();
                chartData[i - 1].append(_tmpData);
            }
        }
    }
    qDebug() << fname + "readed " + recordCounter + " lines";
    // startPreparationToDraw();
    qint64 first = msecFromQTime(times.first());
    qint64 last  = msecFromQTime(times.last());
    chartPlot->xAxis->setRange(first, rangeSize, Qt::AlignCenter);
    qDebug() << fname + "end";
}

qint64 ChartViewer::msecFromQTime(QTime time)
{
    QString fname = "ChartViewer::msecFromQTime() => ";
    qDebug() << fname + "started, time is " + time.toString("h:m:s:z");
    qint64 runtime = 0;
    runtime += time.hour()   * 1000 * 60 * 60;
    runtime += time.minute() * 1000 * 60;
    runtime += time.second() * 1000;
    runtime += time.msec();
    qDebug() << fname + "end, return " + QString::number(runtime);
    return runtime;
}

void ChartViewer::onHideClicked()
{
    QString fname = "ChartViewer::onHideClicked() => ";
    qDebug() << fname + "started";
    qDebug() << fname + "end";
}

void ChartViewer::onCloseClicked()
{
    QString fname = "ChartViewer::onCloseClicked() => ";
    qDebug() << fname + "started";
    qDebug() << fname + "end";
}
