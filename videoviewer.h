#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QWebEngineView>
#include <dataviewer.h>

class VideoViewer : public DataViewer
{
   Q_OBJECT
public:
   explicit VideoViewer (QWidget *parent = 0);

public slots:
    virtual void onHidePressed();
    virtual void onClosePressed();

private:
    void startPreparationToDraw();
    void updateViewer();
    QWebEngineView* map;

};

#endif // VIDEOVIEWER_H
