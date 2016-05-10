#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
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

public slots:
    void onRedrawSignal(double speed, double time);

signals:
    void timelineEndSignal();

protected slots:
    void onOpenPressed();
    virtual void onHidePressed() = 0;
    virtual void onClosePressed() = 0;

protected:
    void setupUi();
    virtual void readFile();
    void openFileFailed();
    void redraw(bool isForward);
    virtual void startPreparationToDraw() = 0;
    virtual void updateViewer(bool isForward) = 0;

    // UI elements
    QPushButton *openButton;
    QPushButton *closeButton;
    QPushButton *hideButton;
    QLabel      *filenameLabel;
    QLabel      *timeLabel;
    QVBoxLayout *buttonLayout;
    QVBoxLayout *viewerLayout;

    // file vars
    QString filename;
    QFile   *file;

    // info vars
    QStringList labels;
    quint64     columnNumber; // = labels.size() - 1, w/o time line

    // data vars
    bool USE_CUSTOM_DATA;
    bool REDRAW_STOPPED;
    double speed;
    double currentTime;
    qint64 currentRecord;
    qint64 recordCounter;
    QVector <double> timeData;
    QHash   <double, double>   timeKeys;
    QList   <QVector <double>> viewerData;
};

#endif // DATAVIEWER_H
