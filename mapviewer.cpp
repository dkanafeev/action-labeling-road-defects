#include "mapviewer.h"

MapViewer::MapViewer (QWidget *parent) : DataViewer(parent)
{
    map = new QWebEngineView();
    viewerLayout->addWidget(map);
    map->setUrl(QUrl("qrc:/html/google_maps.html"));
    map->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    map->setMinimumSize(300, 200);
    map->show();
}

void MapViewer::startPreparationToDraw(){}
void MapViewer::updateViewer(){}

void MapViewer::onHidePressed()
{
    map->setVisible(map->isVisible() ? false : true);
}
void MapViewer::onClosePressed() {}

