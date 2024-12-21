#include "mc.h"
#include <QTimer>
#include <QPainter>
#include <QPixmap>
#include <QMap>
#include <QApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QKeyEvent>


QPoint MC::s_movePt = QPoint(0, 0);
QPoint MC::s_lpPt = QPoint(0, 0);
QPoint MC::s_rpPt = QPoint(0, 0);


MC::MC(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground,true);

    QFontMetrics fm = this->fontMetrics();
    fmRect = fm.boundingRect(QString("0000,0000"));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [timer,this](){
        this->update();
        if(this->pixmap.isNull())
            this->pixmap = QApplication::primaryScreen()->grabWindow(0);
    });
    timer->start(50);
}

MC::~MC() {}

void MC::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //painter.drawPixmap(0, 0, pixmap);
    painter.fillRect(0, 0, width(), height(), QColor(0,0,0,10));

    QString coord = QString("%1,%2").arg(mousePostion.x()).arg(mousePostion.y());
    lastPos = coord;
    
    QPoint pos = mapFromGlobal(mousePostion) - QPoint(fmRect.width()/2,fmRect.height());
    QRect rect = QRect(pos.x()-5, pos.y()-10, fmRect.width()+10, fmRect.height()+10);
    painter.setBrush(QColor(0,0,0,100));
    painter.drawRect(rect);
    painter.setPen(Qt::white);
    painter.drawText(pos, coord);

    for(auto it = ptMap.begin(); it != ptMap.end(); ++it){
        painter.drawRect(it.value().rect);
        painter.drawText(it.value().pos, it.key());
    }

    painter.setPen(Qt::red);
    painter.drawLine(0,mousePostion.y(),width(),mousePostion.y());
    painter.drawLine(mousePostion.x(),0,mousePostion.x(),height());
}

void MC::mousePressEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        TextRect    tr;
        tr.name = lastPos;
        tr.pos = mapFromGlobal(event->globalPos()) - QPoint(fmRect.width()/2,fmRect.height());
        tr.rect = QRect(tr.pos.x()-5, tr.pos.y()-10, fmRect.width()+10, fmRect.height()+10);
        ptMap[lastPos] = tr;
    }
    else if(event->button() == Qt::RightButton) {
        
    }
}

void MC::mouseMoveEvent(QMouseEvent *event) {
    mousePostion = event->globalPos();
}

void MC::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Escape)
        qApp->quit();
}
