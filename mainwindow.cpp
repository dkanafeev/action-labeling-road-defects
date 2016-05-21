#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup timers
    savedTime = 0;
    speed = 0;
    dataTimer = new QTimer();
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(onTimerSignal()));

    // connect buttons
    connect(ui->openButton,          SIGNAL(pressed()), this, SLOT(onOpenClicked()));
    connect(ui->playButton,          SIGNAL(pressed()), this, SLOT(onPlayClicked()));
    connect(ui->stopButton,          SIGNAL(pressed()), this, SLOT(onStopClicked()));
    connect(ui->speedUpButton,       SIGNAL(pressed()), this, SLOT(onSpeedUpClicked()));
    connect(ui->speedDownButton,     SIGNAL(pressed()), this, SLOT(onSpeedDownClicked()));
    connect(ui->gotoTimeButton,      SIGNAL(pressed()), this, SLOT(onGotoTimeClicked()));
    connect(ui->addActionButton,     SIGNAL(pressed()), this, SLOT(onAddActionClicked()));
    connect(ui->loadActionsButton,   SIGNAL(pressed()), this, SLOT(onLoadActionsClicked()));
    connect(ui->saveActionsButton,   SIGNAL(pressed()), this, SLOT(onSaveActionsClicked()));
    connect(ui->actionStartedButton, SIGNAL(pressed()), this, SLOT(onActionStartedClicked()));
    connect(ui->actionEndedButton,   SIGNAL(pressed()), this, SLOT(onActionEndedClicked()));

    // setup default viewers
    mapViewer = new MapViewer();
    videoViewer = new VideoViewer();

    // connect to redraw
    connect (this, SIGNAL(redrawSignal(double, double, int)), mapViewer,   SLOT(onRedrawSignal(double, double, int)));
    //connect (this, SIGNAL(redrawSignal(double, double, int)), videoViewer, SLOT(onRedrawSignal(double, double, int)));
    connect(ui->playButton, SIGNAL(pressed()), videoViewer, SLOT(play()));
    connect(ui->stopButton, SIGNAL(pressed()), videoViewer, SLOT(play()));
    connect(this, SIGNAL(onSpeedChanged(double)), videoViewer, SLOT(onSpeedChangedSignal(double)));
    // connect to timeline end
    connect (mapViewer,   SIGNAL(timelineEndSignal()), this, SLOT(onTimelineEndSignal()));
    //connect (videoViewer, SIGNAL(timelineEndSignal()), this, SLOT(onTimelineEndSignal()));

    // setup ui
    ui->mapLayout->addLayout(mapViewer);
    ui->videoLayout->addWidget(videoViewer);
    ui->stopButton->setEnabled(false);
    ui->speedUpButton->setEnabled(false);
    ui->speedDownButton->setEnabled(false);
    ui->timeEdit->setValidator( new QIntValidator() );
    currentAction = 0;
    ui->cmbox_actionType->addItem("no_action");
    actionCodes.insert("no_action", currentAction);
}

void MainWindow::onOpenClicked()
{
    QString fname = "MainWindow::onOpenClicked";
    qDebug () << fname + " started" ;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open data"), QDir::homePath(),tr("File (*.output)"));
    if (fileName == "")
    {
        qDebug() << fname + " empty filename! ended";
        return;
    }
    qDebug() << fname + " filename: " + fileName;
    ChartViewer* test = new ChartViewer();
    chartList.append(test);
    test->openFile(fileName);
    connect (this, SIGNAL(redrawSignal(double, double, int)), test, SLOT(onRedrawSignal(double, double, int)));
    ui->chartsLayout->insertLayout(chartList.size() - 1, test);
    qDebug () << fname + " ended" ;
}

void MainWindow::onPlayClicked()
{
    // set default speed
    if (!speed) speed = 1;

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
    emit onSpeedChanged(speed);
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
    emit onSpeedChanged(speed);
}

void MainWindow::onStopClicked()
{
    // clean speed value
//    speed = 0;

    // stop timers
    dataTimer->stop();

    // save elasped time
    savedTime +=  speed * timer.elapsed();

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
    emit redrawSignal(speed, savedTime + speed * timer.elapsed(), currentAction);
}

void MainWindow::onAddActionClicked()
{
    QString actionName = ui->addActionEdit->text();
    ui->cmbox_actionType->addItem(actionName);
    actionCodes.insert(actionName, actionCodes.size());
}

void MainWindow::onActionStartedClicked()
{
    QString actionName = ui->cmbox_actionType->itemText(ui->cmbox_actionType->currentIndex());
    currentAction = actionCodes[actionName];
}

void MainWindow::onActionEndedClicked()
{
    currentAction = actionCodes["no_action"];
}

void MainWindow::onSaveActionsClicked()
{
    // generate report, lables at the first line
    QString actionsReport = "action_id,action_name\n";

    // add data
    for (int i = 0; i < ui->cmbox_actionType->count(); ++i)
        actionsReport.append( QString::number(i) + "," + ui->cmbox_actionType->itemText(i) + "\n");

    // save actionsReport
    QString actionsReportName = QFileDialog::getSaveFileName();
    QFile file( actionsReportName );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );
        stream << actionsReport << endl;
    }
    file.close();
}
void MainWindow::onLoadActionsClicked()
{
    qDebug() << "onLoadActionsClicked currently unavailable!";
}

void MainWindow::onTimelineEndSignal()
{
    // NOT DONE YET! DON'T USE IT!
    return;

    ++endCounter;
    if ( endCounter == chartList.size() + 2) // + 2 are MapViewer and VideoViewer
    {
        // stop playing if all viewers emitted signal that timeline is end
        emit onStopClicked();
        savedTime = 0;
        endCounter = 0;
        emit redrawSignal(0, 0, currentAction); // mean that all viewer should be returned to the initial state
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
