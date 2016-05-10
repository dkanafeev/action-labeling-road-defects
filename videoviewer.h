#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWebEngineView>
#include <dataviewer.h>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>

class VideoViewer : public DataViewer
{
   Q_OBJECT
public:
   explicit VideoViewer (QWidget *parent = 0);

public slots:
    virtual void onHidePressed();
    virtual void onClosePressed();

protected:
    void readFile();
    void startPreparationToDraw();
    void updateViewer(bool isForward);
    //QWebEngineView* map;
    QVideoWidget* videowidget;
    QMediaPlaylist* playlist;
    QMediaPlayer mediaPlayer;

};

#endif // VIDEOVIEWER_H
