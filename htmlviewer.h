#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

#include <QPainter>
#include <QTextBrowser>
#include "dict.h"


class HTMLViewer : public QTextBrowser
{
public:
    HTMLViewer();

private:
    int num = 0;




protected:
    void paintEvent(QPaintEvent *event);
    void sourceChanged(const QUrl &src);
    void setSource(const QUrl &name);
};

#endif // HTMLVIEWER_H
