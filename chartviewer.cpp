#include "chartviewer.h"

ChartViewer::ChartViewer(QWidget *parent) : QVBoxLayout(parent)
{
    this->setupUi();
}

void ChartViewer::setupUi()
{
    //setup buttons
    hideButton = new QPushButton("Hide");
    connect(hideButton, SIGNAL(pressed()), this, SLOT(onHidePressed()));
    closeButton = new QPushButton("Close");
    connect(closeButton, SIGNAL(pressed()), this, SLOT(onClosePressed()));
    nextButton = new QPushButton(">");
    connect(nextButton, SIGNAL(pressed()), this, SLOT(onNextPressed()));
    prevButton = new QPushButton("<");
    connect(prevButton, SIGNAL(pressed()), this, SLOT(onPrevPressed()));

    //setup labels
    filenameLabel = new QLabel("");
    timeLabel = new QLabel("");

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

    // setup buttonLayout
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(timeLabel);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(hideButton);
    buttonLayout->addWidget(filenameLabel);
    buttonLayout->addWidget(closeButton);


    // add items to this chartViewer
    this->addLayout(buttonLayout);
    this->addWidget(chartPlot);
}

void ChartViewer::redraw(qint64 speed)
{
    qint64 _tmpTimeElapsed = speed * timer.elapsed();
    // check timers
    // note: multiplication by speed is necessary for changing direction
    if (speed * ( lastTime + _tmpTimeElapsed ) >= speed * nextTime)
    {
        updatePlot(speed > 0 ? true : false);
        // update times and restart timer
        lastTime = nextTime;
        timeLabel->setText("Time: " + QString::number(lastTime, 'f', 0));
        //note: updatePlot change nextRecord!
        nextTime = timeData[nextRecord];
        timer.restart();
    }
}

void ChartViewer::updatePlot(bool isForward)
{
    // update each line
    for (quint64 i = 0 ; i < lineNumber; ++i)
    {
        chartPlot->graph(i)->rescaleKeyAxis();
        chartPlot->xAxis->setRange(timeData[nextRecord], rangeXSize, Qt::AlignCenter);
    }
    chartPlot->replot();

    // get next record id
    nextRecord = isForward ? nextRecord + 1 : nextRecord - 1 ;

    // check if first/last record
    nextRecord = nextRecord >= recordCounter ? 0 : nextRecord < 0 ? recordCounter - 1 : nextRecord;
}

void ChartViewer::openFile(QString filename)
{
    file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        openFileFailed();
    }
    else
    {
        this->filename = filename;
        filenameLabel->setText("Path: " + filename);
        readFile();
    }
}

void ChartViewer::openFileFailed()
{
    QString fname = "ChartViewer::openFileFailed() => ";
    qDebug() << fname + "started";
    /** Need to think what should be happen if file is not open
     * We can:
     * 1) show MsgBox and remove this ChartViewer
     * 2) show OpenFile dialog again and again while user don' choose a file
     * 3) something else?
     */
    qDebug() << fname + "end";
}

void ChartViewer::readFile()
{
    nextRecord = 0;
    recordCounter = 0;

    // read first line which contain column labels
    QTextStream in(file);
    QString line = in.readLine();
    labels = line.split(',');
    lineNumber = labels.size() - 1;

    for (int i = 0; i < labels.size(); ++i)
    {
        QString str = labels[i];

        // create graph
        chartPlot->addGraph(chartPlot->xAxis, chartPlot->yAxis);

        //set colors
        QColor color = getColor(i);
        lineColors.append(QPen(QBrush(color), 3));

        // create data lists
        if ( str != "time" ) // note: alredy created lists for time (timeDataInteger)
        {
            QVector <double> newList;
            chartData.append(newList);
        }
    }

    // read data
    while ( !( line = in.readLine() ).isNull())
    {
        QStringList _tmpSplitedLine = line.split(',');
        QString _tmpStr;

        // skip line if number of value is not the same as label number
        if (_tmpSplitedLine.size() != lineNumber + 1)
            continue;

        // read fisrt value, it should be a time
        _tmpStr = _tmpSplitedLine.at(0);
        QTime _tmpTime = QTime::fromString(_tmpStr, "h:m:s:z");
        timeData.append(msecFromQTime(_tmpTime));

        // read another values, which should be a double
        for (quint64 i = 0; i < lineNumber; ++i) {
            _tmpStr = _tmpSplitedLine.at(i + 1);
            double _tmpData = _tmpStr.toDouble();
            chartData[i].append(_tmpData);
        }
        ++recordCounter;
    }

    // add preloaded lines
    startPreparationToDraw();

    // save initial time values and start timer
    nextTime = timeData.first();
    lastTime = timeData.first();
    timeLabel->setText("Time: " + QString::number(lastTime, 'f', 0));
    timer.start();
}

void ChartViewer::startPreparationToDraw()
{
    // preload all data to chart
    for (quint64 i = 0; i < lineNumber; i++)
    {
        chartPlot->addGraph(chartPlot->xAxis, chartPlot->yAxis);
        chartPlot->graph(i)->rescaleKeyAxis();
        chartPlot->graph(i)->setPen(lineColors[i]);
        chartPlot->graph(i)->addData(timeData, chartData[i]);
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

qint64 ChartViewer::msecFromQTime(QTime time)
{
    qint64 runtime = 0;
    runtime += time.hour()   * 1000 * 60 * 60;
    runtime += time.minute() * 1000 * 60;
    runtime += time.second() * 1000;
    runtime += time.msec();
    return runtime;
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

void ChartViewer::onNextPressed()
{
    QString fname = "ChartViewer::onNextPressed() => ";
    qDebug() << fname + "started";
    // copypaste from redraw - separate func?
    updatePlot(true);
    // update times and restart timer
    lastTime = nextTime;
    timeLabel->setText("Time: " + QString::number(lastTime, 'f', 0));
    //note: updatePlot change nextRecord!
    nextTime = timeData[nextRecord];

    qDebug() << fname + "end";
}

void ChartViewer::onPrevPressed()
{
    QString fname = "ChartViewer::onPrevPressed() => ";
    qDebug() << fname + "started";
    // copypaste from redraw - separate func?
    updatePlot(false);
    // update times and restart timer
    lastTime = nextTime;
    timeLabel->setText("Time: " + QString::number(lastTime, 'f', 0));
    //note: updatePlot change nextRecord!
    nextTime = timeData[nextRecord];
    qDebug() << fname + "end";
}
