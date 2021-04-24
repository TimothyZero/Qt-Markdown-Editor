#include "htmlviewer.h"

#include <QTextBlock>
#include <QDebug>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>

HTMLViewer::HTMLViewer()
{
    setReadOnly(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setOpenExternalLinks(true);
    this->setOpenLinks(true);
    this->document()->setIndentWidth(LIST_INDENT_WIDTH);
    this->setStyleSheet("background-color:white;");
    this->document()->setDocumentMargin(DOCUMENT_MARGIN);
}

void HTMLViewer::paintEvent(QPaintEvent *event){
//    qDebug()<<num<<verticalScrollBar()->value()<<verticalScrollBar()->maximum();
//    qDebug()<<contentsRect()<<viewport()->rect()<<event->rect();
//    if(num > 2) return;
    QTextBrowser::paintEvent(event);
    QPainter p;
    p.begin(this->viewport());
    p.setRenderHint(QPainter::Antialiasing);

//    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
//    painter.setCompositionMode(QPainter::CompositionMode_DestinationAtop);



//    qDebug()<<"blockCount"<<document()->blockCount();

    int maxWidth = 0,maxHeight = 0,count = 0;
    QTextBlock textBlock = document()->begin();
    while(textBlock.isValid()){
        p.setPen(QPen(QColor(0 , 0 , 255), 1, Qt::SolidLine, Qt::FlatCap,Qt::MiterJoin));
//        qDebug()<<textBlock.text()<<textBlock.fragmentIndex();;
//        QTextBlock::iterator it;
//        for (it = textBlock.begin(); !(it.atEnd()); ++it) {
//            QTextFragment currentFragment = it.fragment();
//            if (currentFragment.isValid()){
//                qDebug()<<currentFragment.text();
//            }
//        }

        QRect temp(textBlock.layout()->boundingRect().toRect().x()+4,
                   textBlock.layout()->position().y(),
                   textBlock.layout()->boundingRect().toRect().width(),
                   textBlock.layout()->boundingRect().toRect().height());

//        qDebug()<<textBlock.text()
//                <<textBlock.layout()->position() //(x,y) margin = 4
//                <<textBlock.layout()->boundingRect().toRect() // (indent_x,y,w,h)
//                <<temp;

        count = temp.bottomRight().x() > maxWidth ? 0 : count + ( maxWidth == temp.bottomRight().x() );
        maxWidth = qMax(maxWidth,temp.bottomRight().x());
        maxHeight = qMax(maxHeight,temp.bottomRight().y());
//        qDebug()<<textBlock.text();

        temp.translate(-horizontalScrollBar()->value(), -verticalScrollBar()->value());


        QPoint tl = textBlock.layout()->boundingRect().toRect().topLeft();
        QPoint br = textBlock.layout()->boundingRect().toRect().bottomRight();
        if(tl.x() == CODE_X){
//            qDebug("code block");
            QRect rect(DOCUMENT_MARGIN,temp.y(),CODE_X - CODE_MARGIN,temp.height());
            p.fillRect(rect,QBrush(QColor(CODE_PRE_BG)));
            QRect rect2(CODE_X - CODE_MARGIN + DOCUMENT_MARGIN,temp.y(),CODE_MARGIN,temp.height());
            p.fillRect(rect2,QBrush(QColor(CODE_BG)));

//            QPainterPath path;
//            path.addRoundedRect(temp, 6, 6);
//            p.setPen(QPen(QColor(CODE_PRE_BG), 1));
//            p.drawPath(path);
        }else if(tl.x()>0){
//            qDebug()<<"INDENT_WIDTH";
            if(br.x() < maxWidth - LIST_INDENT_WIDTH){
//                qDebug()<<"quote";
                for(int i = 0;i < tl.x()/LIST_INDENT_WIDTH;i++){
                    QRect rect(DOCUMENT_MARGIN + LIST_INDENT_WIDTH*i,temp.y() - LINE_MARGIN/2, QUOTE_PRE_WIDTH ,temp.height() + LINE_MARGIN);
                    p.fillRect(rect,QBrush(QColor(QUOTE_PRE_BG)));
                }
            }else{
//                qDebug()<<"list";
            }
        }else{
//            p.drawRect(temp);
        }


        textBlock = textBlock.next();
    }

    p.end();
    if(count < document()->blockCount()*0.5 ){
        qDebug()<<"+=+=+=+=";
        maxWidth += 40;
        setMinimumWidth(maxWidth);
    }
//    qDebug()<<count<<maxWidth<<maxHeight;
    setMinimumHeight(maxHeight + 60);
    num++;
    return;
}

void HTMLViewer::sourceChanged(const QUrl &src){
    qDebug()<<src;
    return sourceChanged(src);
}

void HTMLViewer::setSource(const QUrl &name){
    qDebug()<<name;
    return;
//    return setSource(name);
}
