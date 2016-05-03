#include "videoviewer.h"

VideoViewer::VideoViewer (QWidget *parent) : DataViewer(parent)
{
    map = new QWebEngineView();
    viewerLayout->addWidget(map);
    map->setUrl(QUrl("qrc:/html/google_maps.html"));
    map->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    map->setMinimumSize(300, 200);
    map->show();
}

void VideoViewer::startPreparationToDraw(){}
void VideoViewer::updateViewer(){}

void VideoViewer::onHidePressed() {}
void VideoViewer::onClosePressed() {}

