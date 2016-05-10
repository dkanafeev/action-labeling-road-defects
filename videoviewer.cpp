#include "videoviewer.h"

VideoViewer::VideoViewer (QWidget *parent) : DataViewer(parent)
  , mediaPlayer(0, QMediaPlayer::VideoSurface)
{
    videowidget = new QVideoWidget;
    viewerLayout->addWidget(videowidget);
    videowidget->setMinimumSize(300, 200);
    videowidget->show();
}

void VideoViewer::startPreparationToDraw(){}

void VideoViewer::updateViewer(bool isForward){
    qDebug() << "VideoViewer::updateViewer(bool isForward)" << isForward;
    mediaPlayer.play();
    mediaPlayer.setPlaybackRate(1);
}

void VideoViewer::onHidePressed() {}
void VideoViewer::onClosePressed() {}

void VideoViewer::readFile()
{
    timeData.append(0);
    mediaPlayer.setVideoOutput(videowidget);
    mediaPlayer.setMedia(QUrl::fromLocalFile(filename));
    qDebug()<<"readfile from videoviewer";
    mediaPlayer.setVolume(50);
}
