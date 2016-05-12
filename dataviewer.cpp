#include "dataviewer.h"

DataViewer::DataViewer(QWidget *parent) : QHBoxLayout(parent)
{
    currentRecord = 0;
    currentTime  = 0;
    recordCounter = 0;
    USE_CUSTOM_DATA = false;
    REDRAW_STOPPED = false;
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

    reportButton = new QPushButton("Report");
    reportButton->setFixedSize(100, 20);
    connect(reportButton, SIGNAL(pressed()), this, SLOT(onReportPressed()));

    //setup labels
    filenameLabel = new QLabel("");
    timeLabel = new QLabel("");
    timeLabel->setFixedSize(100, 25);

    // setup buttonLayout
    buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(timeLabel);
    buttonLayout->addWidget(hideButton);
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(reportButton);

    // setup viewerLayout
    viewerLayout = new QVBoxLayout;
    viewerLayout->addWidget(filenameLabel);

    // add items to this DataViewer
    this->addLayout(buttonLayout);
    this->addLayout(viewerLayout);
}

void DataViewer::onRedrawSignal(double speed, double time, int action)
{
    // don't redraw if used default data sources and this sources is empty
    if (! timeData.size() && ! USE_CUSTOM_DATA)
        return;

    // don't redraw if stopped
    if ( REDRAW_STOPPED )
        return;

    // check data for current time
    for (int i = time - 1000 ; i < time; i++)
    {
        if ( timeKeys.contains(i) )
        {
            // save current speed
            this->speed = speed;
            // save time key
            timeLabel->setText("Time: " + QString::number(i) + " A=" + QString::number(action));
            currentTime = i;
            currentRecord = timeKeys.value(i);
            actionData[currentRecord] = action;
            // start redraw
            redraw(speed > 0);
            break;
        }
    }
}

void DataViewer::redraw(bool isForward)
{
    updateViewer(isForward);
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
    /**
     * Need to think what should be happen if file is not open
     * We can:
     * 1) show MsgBox and remove this DataViewer
     * 2) show OpenFile dialog again and again while user don' choose a file
     * 3) something else?
     */
    qDebug() << fname + "end";
}

void DataViewer::readFile()
{
    qDebug()<<"readFile from DataViewer";

    // read first line which contain column labels
    QTextStream in(file);
    QString line = in.readLine();
    labels = line.split(',');
    columnNumber = labels.size() - 1;

    //clear data
    viewerData.clear();
    timeData.clear();
    timeKeys.clear();
    actionData.clear();

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
        if (_tmpSplitedLine.size() != columnNumber + 1)
            continue;

        // add default action id to actionData
        actionData.append(0);

        // read fisrt value, it should be a time
        _tmpStr = _tmpSplitedLine.at(0);
        timeKeys.insert(_tmpStr.toDouble(), timeData.size());
        timeData.append(_tmpStr.toDouble());

        // read other values, which should be a double
        for (quint64 i = 0; i < columnNumber; ++i) {
            _tmpStr = _tmpSplitedLine.at(i + 1);
            double _tmpData = _tmpStr.toDouble();
            viewerData[i].append(_tmpData);
        }
    }

    recordCounter = timeData.size();
    // add preloaded lines
    startPreparationToDraw();

    timeLabel->setText("Time: 0");
}

void DataViewer::onClosePressed()
{
    QString fname = "DataViewer::onClosePressed() => ";
    qDebug() << fname + "started";
    // send signal to MainWindow that this DataViewer is removed
    // there should be destructor calling, which hasn't write yet
    qDebug() << fname + "end";
}

void DataViewer::onOpenPressed() {
    openFile(QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open File"), "."));
}

void DataViewer::onReportPressed()
{
    // generate report, lables at the first line
    QString report = labels.join(",") + ",action\n";
    QString _tmpData = "";

    // add data
    for (int i = 0; i < timeData.size(); ++i)
    {
        _tmpData = "";
        for (int j = 0; j < labels.size() - 1; ++j)
            _tmpData.append(QString::number(viewerData[j].at(i)) + ",");

        QString tt =  QString::number(timeData.value(i), 'f', 0) + ","
                   + _tmpData
                   + QString::number(actionData.value(i)) + "\n";
        report.append(tt);
    }

    // save report
    QString reportName = QFileDialog::getSaveFileName();
    QFile file( reportName );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );
        stream << report << endl;
    }
    file.close();
}
