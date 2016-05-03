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

private:
    void startPreparationToDraw();
    void updateViewer();
    QWebEngineView* map;

};

#endif // MAPVIEWER_H
