#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QTime>
#include <QLabel>
#include <QElapsedTimer>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DataViewer : public QHBoxLayout
{
    Q_OBJECT
    QDOC_PROPERTY(QString fileName READ filename WRITE openFile)
public:
    explicit DataViewer(QWidget *parent = 0);
    void openFile(QString filename);

    QString getFileName  () {return filename;}
    double  getLastTime  () {return lastTime;}
    double  getNextTime  () {return nextTime;}

public slots:
    void onRedrawSignal(double speed);


protected slots:
    void onNextPressed();
    void onPrevPressed();
    void onOpenPressed();
    virtual void onHidePressed() = 0;
    virtual void onClosePressed() = 0;

protected:
    void setupUi();
    void readFile();
    void openFileFailed();
    void redraw(bool isForward);
    qint64 msecFromQTime(QTime time);
    virtual void startPreparationToDraw() = 0;
    virtual void updateViewer(bool isForward) = 0;


    // UI elements
    QPushButton *openButton;
    QPushButton *closeButton;
    QPushButton *hideButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QLabel      *filenameLabel;
    QLabel      *timeLabel;
    QVBoxLayout *buttonLayout;
    QVBoxLayout *viewerLayout;

    // file vars
    QString filename;
    QFile   *file;

    // info vars
    QStringList   labels;
    quint64       lineNumber; // = labels.size() - 1, w/o time line

    // time vars
    QElapsedTimer timer;
    double lastTime;
    double nextTime;

    // data vars
    qint64 nextRecord;
    qint64 recordCounter;
    QVector <double> timeData;
    QList <QVector <double>> viewerData;
};

#endif // DATAVIEWER_H
