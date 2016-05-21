#ifndef MAPVIEWER_H
#define MAPVIEWER_H

#include <QWebEngineView>
#include "dataviewer.h"

class MapViewer : public DataViewer
{
   Q_OBJECT
public:
   explicit MapViewer (QWidget *parent = 0);

public slots:
    virtual void onHidePressed();
    virtual void onClosePressed();
    void onCleanMapPressed();

private:


    QPushButton* cleanMap;
    void startPreparationToDraw();
    void updateViewer(bool isForward);

    void addLine(qint64 id);
    void addMarker(qint64 id);
    void addAllLines();
    void removeLine(qint64 id);
    void removeMarker(qint64 id);
    void removeAllLines();
    void removeAllMarkers();

    QWebEngineView* map;

};

#endif // MAPVIEWER_H
