#include "dataviewer.h"

DataViewer::DataViewer(QWidget *parent) : QHBoxLayout(parent)
{
    this->setupUi();
}

void DataViewer::setupUi()
{
    //setup buttons
    hideButton = new QPushButton("Hide");
    hideButton->setFixedSize(100, 20);
    connect(hideButton, SIGNAL(pressed()), this, SLOT(onHidePressed()));

    closeButton = new QPushButton("Close");
    closeButton->setFixedSize(100, 25);
    connect(closeButton, SIGNAL(pressed()), this, SLOT(onClosePressed()));

    openButton = new QPushButton("Open");
    openButton->setFixedSize(100, 25);
    connect(openButton, SIGNAL(pressed()), this, SLOT(onOpenPressed()));

    nextButton = new QPushButton(">");
    nextButton->setFixedSize(100, 25);
    connect(nextButton, SIGNAL(pressed()), this, SLOT(onNextPressed()));

    prevButton = new QPushButton("<");
    prevButton->setFixedSize(100, 25);
    connect(prevButton, SIGNAL(pressed()), this, SLOT(onPrevPressed()));

    //setup labels
    filenameLabel = new QLabel("");
    timeLabel = new QLabel("");
    timeLabel->setFixedSize(100, 25);

    // setup buttonLayout
    buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(timeLabel);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(hideButton);
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(closeButton);

    // setup viewerLayout
    viewerLayout = new QVBoxLayout;
    viewerLayout->addWidget(filenameLabel);

    // add items to this DataViewer
    this->addLayout(buttonLayout);
    this->addLayout(viewerLayout);
}

void DataViewer::onRedrawSignal(double speed)
{
    if (! timeData.size())
        return;

    qint64 _tmpTimeElapsed = speed * timer.elapsed();
    // check timers and file exist
    // note: multiplication by speed is necessary for changing direction
    if ( speed * ( lastTime + _tmpTimeElapsed ) >= speed * nextTime && timeData.size())
    {
        redraw(speed > 0 ? true : false);
        timer.restart();
    }
}

void DataViewer::openFile(QString filename)
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

void DataViewer::openFileFailed()
{
    QString fname = "DataViewer::openFileFailed() => ";
    qDebug() << fname + "started";
    /** Need to think what should be happen if file is not open
     * We can:
     * 1) show MsgBox and remove this DataViewer
     * 2) show OpenFile dialog again and again while user don' choose a file
     * 3) something else?
     */
    qDebug() << fname + "end";
}

void DataViewer::readFile()
{
    // next record shouldn't be a zero at the beginning
    // some issues could be if file contain only one line.
    nextRecord = 1;
    recordCounter = 0;

    // read first line which contain column labels
    QTextStream in(file);
    QString line = in.readLine();
    labels = line.split(',');
    lineNumber = labels.size() - 1;

    //clear data
    viewerData.clear();
    timeData.clear();

    for (int i = 0; i < labels.size(); ++i)
    {
        QString str = labels[i];

        // create data lists
        if ( str != "time" ) // note: alredy created lists for time (timeDataInteger)
        {
            QVector <double> newList;
            viewerData.append(newList);
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
            viewerData[i].append(_tmpData);
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

void DataViewer::redraw(bool isForward)
{
    if (! timeData.size())
        return;
    // copypaste from redraw - separate func?
    updateViewer(isForward);

    // get next record id
    isForward ? ++nextRecord : --nextRecord;
    // check if first/last record
    nextRecord = nextRecord >= recordCounter ? 0 : nextRecord < 0 ? recordCounter - 1 : nextRecord;

    // update times and restart timer
    lastTime = nextTime;
    timeLabel->setText("Time: " + QString::number(lastTime, 'f', 0));
    nextTime = timeData[nextRecord];
}

qint64 DataViewer::msecFromQTime(QTime time)
{
    qint64 runtime = 0;
    runtime += time.hour()   * 1000 * 60 * 60;
    runtime += time.minute() * 1000 * 60;
    runtime += time.second() * 1000;
    runtime += time.msec();
    return runtime;
}

void DataViewer::onClosePressed()
{
    QString fname = "DataViewer::onClosePressed() => ";
    qDebug() << fname + "started";
    // send signal to MainWindow that this DataViewer is removed
    // there should be destructor calling, which hasn't write yet
    qDebug() << fname + "end";
}
void DataViewer::onNextPressed() { redraw(true );}
void DataViewer::onPrevPressed() { redraw(false);}
void DataViewer::onOpenPressed() { openFile(QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open File"), "."));}
