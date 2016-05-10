#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup timers
    savedTime = 0;
    dataTimer = new QTimer();
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(onTimerSignal()));

    // connect buttons
    connect(ui->openButton,      SIGNAL(pressed()), this, SLOT(onOpenClicked()));
    connect(ui->playButton,      SIGNAL(pressed()), this, SLOT(onPlayClicked()));
    connect(ui->stopButton,      SIGNAL(pressed()), this, SLOT(onStopClicked()));
    connect(ui->speedUpButton,   SIGNAL(pressed()), this, SLOT(onSpeedUpClicked()));
    connect(ui->speedDownButton, SIGNAL(pressed()), this, SLOT(onSpeedDownClicked()));
    connect(ui->gotoTimeButton,  SIGNAL(pressed()), this, SLOT(onGotoTimeClicked()));

    // setup default viewers
    mapViewer = new MapViewer();
    videoViewer = new VideoViewer();

    // connect to redraw
    connect (this, SIGNAL(redrawSignal(double, double)), mapViewer,   SLOT(onRedrawSignal(double, double)));
    connect (this, SIGNAL(redrawSignal(double, double)), videoViewer, SLOT(onRedrawSignal(double, double)));

    // connect to timeline end
    connect (mapViewer,   SIGNAL(timelineEndSignal()), this, SLOT(onTimelineEndSignal()));
    connect (videoViewer, SIGNAL(timelineEndSignal()), this, SLOT(onTimelineEndSignal()));

    // setup ui
    ui->mapLayout->addLayout(mapViewer);
    ui->videoLayout->addLayout(videoViewer);
    ui->stopButton->setEnabled(false);
    ui->speedUpButton->setEnabled(false);
    ui->speedDownButton->setEnabled(false);
    ui->timeEdit->setValidator( new QIntValidator() );
}

void MainWindow::onOpenClicked()
{
    QString fname = "MainWindow::onOpenClicked";
    qDebug () << fname + " started" ;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".");
    if (fileName == "")
    {
        qDebug() << fname + " empty filename! ended";
        return;
    }
    qDebug() << fname + " filename: " + fileName;
    ChartViewer* test = new ChartViewer();
    chartList.append(test);
    test->openFile(fileName);
    connect (this, SIGNAL(redrawSignal(double, double)), test, SLOT(onRedrawSignal(double, double)));
    ui->chartsLayout->insertLayout(chartList.size() - 1, test);
    qDebug () << fname + " ended" ;
}

void MainWindow::onPlayClicked()
{
    // set default speed
    speed = 1;

    // start timers
    dataTimer->start(timerDelay);
    timer.restart();

    // update ui
    ui->playButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->speedUpButton->setEnabled(true);
    ui->speedDownButton->setEnabled(true);
    ui->speedLabel->setText("Speed: " + QString::number(speed));
}

void MainWindow::onSpeedUpClicked()
{
    // save elasped time and restart timer
    savedTime +=  speed * timer.elapsed();
    timer.restart();

    // update speed
    speed = speed >= minSpeed ? speed * 2 : speed < -minSpeed ? speed / 2 : minSpeed;

    // update ui
    ui->speedLabel->setText("Speed: " + QString::number(speed));
}

void MainWindow::onSpeedDownClicked()
{
    // save elasped time and restart timer
    savedTime +=  speed * timer.elapsed();
    timer.restart();

    // update speed
    speed = speed <= -minSpeed ? speed * 2 : speed > minSpeed ? speed / 2 : -minSpeed;

    // update ui
    ui->speedLabel->setText("Speed: " + QString::number(speed));
}

void MainWindow::onStopClicked()
{
    // clean speed value
    speed = 0;

    // stop timers
    dataTimer->stop();

    // save elasped time
    savedTime +=  timer.elapsed();

    // update ui
    ui->speedLabel->setText("Speed: " + QString::number(speed));
    ui->playButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->speedUpButton->setEnabled(false);
    ui->speedDownButton->setEnabled(false);

}

void MainWindow::onGotoTimeClicked()
{
    savedTime = ui->timeEdit->text().toInt();
    timer.restart();
    qDebug () << "onGotoTimeClicked() " << QString::number(savedTime);
}

void MainWindow::onTimerSignal()
{
    emit redrawSignal(speed, savedTime + speed * timer.elapsed());
}

void MainWindow::onTimelineEndSignal()
{
    ++endCounter;
    if ( endCounter == chartList.size() + 2) // + 2 are MapViewer and VideoViewer
    {
        // stop playing if all viewers emitted signal that timeline is end
        emit onStopClicked();
        savedTime = 0;
        endCounter = 0;
        emit redrawSignal(0, 0); // mean that all viewer should be returned to the initial state
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
