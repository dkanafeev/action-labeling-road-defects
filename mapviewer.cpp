#include "mapviewer.h"

MapViewer::MapViewer (QWidget *parent) : DataViewer(parent)
{
    map = new QWebEngineView();
    viewerLayout->addWidget(map);
    map->setUrl(QUrl("qrc:/html/google_maps.html"));
    map->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    map->setMinimumSize(300, 200);
    map->show();

    cleanMap = new QPushButton("Clean");
    cleanMap->setFixedSize(100, 25);
    connect(cleanMap, SIGNAL(pressed()), this, SLOT(onCleanMapPressed()));
    buttonLayout->addWidget(cleanMap);
}

void MapViewer::startPreparationToDraw()
{
    //add start marker?
}

void MapViewer::updateViewer(bool isForward)
{
    if ( isForward)
        currentRecord != 0 ? addLine(currentRecord) : removeAllLines();
    else
        currentRecord != recordCounter - 1 ? removeLine(currentRecord) : addAllLines();
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

void MapViewer::onCleanMapPressed()
{
    removeAllLines();
}

void MapViewer::onHidePressed()
{
    map->setVisible(map->isVisible() ? false : true);
    filenameLabel->setVisible(map->isVisible());
}
void MapViewer::onClosePressed() {}


