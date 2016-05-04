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

void MapViewer::startPreparationToDraw()
{
    //add start marker?
}

void MapViewer::updateViewer(bool isForward)
{
    if ( isForward)
        nextRecord != 0 ? addLine(nextRecord) : removeAllLines();
    else
        nextRecord != recordCounter - 1 ? removeLine(nextRecord) : addAllLines();
}

void MapViewer::addLine(qint64 id){
    QString jsParam = QString::number(viewerData[0][id-1]) + "," + // current lat
                      QString::number(viewerData[1][id-1]) + "," + // current lng
                      QString::number(viewerData[0][id])   + "," + // next lat
                      QString::number(viewerData[1][id]) ;         // next lng
    QString str = "addLine(" + jsParam + ");"; // run func from google_maps.html
    map->page()->runJavaScript(str);
}

void MapViewer::addAllLines(){
    for (qint64 i = 1; i < recordCounter; ++i)
        addLine(i); // add line from ( i - 1 ) to ( i )
}

void MapViewer::removeLine(qint64 id){
    QString jsParam = QString::number(id-1);
    QString str = "removeLine(" + jsParam + ");"; // run func from google_maps.html
    map->page()->runJavaScript(str);
}

void MapViewer::removeAllLines(){
    map->page()->runJavaScript("removeAllLines()");
}

void MapViewer::onHidePressed()
{
    map->setVisible(map->isVisible() ? false : true);
    filenameLabel->setVisible(map->isVisible());
}
void MapViewer::onClosePressed() {}

