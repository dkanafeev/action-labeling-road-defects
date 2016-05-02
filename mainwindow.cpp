#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dataTimer = new QTimer();
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(onTimerSignal()));

    connect(ui->openButton,      SIGNAL(pressed()), this, SLOT(onOpenClicked()));
    connect(ui->playButton,      SIGNAL(pressed()), this, SLOT(onPlayClicked()));
    connect(ui->stopButton,      SIGNAL(pressed()), this, SLOT(onStopClicked()));
    connect(ui->speedUpButton,   SIGNAL(pressed()), this, SLOT(onSpeedUpClicked()));
    connect(ui->speedDownButton, SIGNAL(pressed()), this, SLOT(onSpeedDownClicked()));
}

void MainWindow::onOpenClicked()
{
    QString fname = "MainWindow::onOpenClicked";
    qDebug () << fname + " started" ;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".");
    ChartViewer* test = new ChartViewer();
    chartList.append(test);
    test->openFile(fileName);
    ui->chartsLayout->addLayout(test);
    qDebug () << fname + " ended" ;
}

void MainWindow::onPlayClicked()
{
    QString fname = "MainWindow::onPlayClicked";
    qDebug () << fname + " started" ;
    speed = 1;
    ui->speedLabel->setText("Speed: " + QString::number(speed));
    ui->playButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    dataTimer->start(timerDelay);
    qDebug () << fname + " ended" ;
}

void MainWindow::onSpeedUpClicked()
{
    QString fname = "MainWindow::onSpeedUpClicked";
    qDebug () << fname + " started" ;
    speed = speed >= 1 ? speed * 2 : speed < -1 ? speed / 2 : 1;
    ui->speedLabel->setText("Speed: " + QString::number(speed));
    qDebug () << fname + " ended" ;
}

void MainWindow::onSpeedDownClicked()
{
    QString fname = "MainWindow::onSpeedDownClicked";
    qDebug () << fname + " started" ;
    speed = speed <= -1 ? speed * 2 : speed > 1 ? speed / 2 : -1;
    ui->speedLabel->setText("Speed: " + QString::number(speed));
    qDebug () << fname + " ended" ;
}

void MainWindow::onStopClicked()
{
    QString fname = "MainWindow::onStopClicked";
    qDebug () << fname + " started" ;
    speed = 0;
    ui->speedLabel->setText("Speed: " + QString::number(speed));
    ui->playButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    dataTimer->stop();
    qDebug () << fname + " ended" ;
}

void MainWindow::onTimerSignal()
{
    QString fname = "MainWindow::onTimerSignal";
    qDebug () << fname + " started" ;
    foreach (ChartViewer *chart, chartList)
    {
        chart->redraw(speed);
    }
    qDebug () << fname + " ended" ;
}

MainWindow::~MainWindow()
{
    delete ui;
}
