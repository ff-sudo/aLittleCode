#include "mainview.h"
#include <QRandomGenerator>
#include <QPainter>
#include <QImage>
#include <QtMath>
#include <QDateTime>
#include <QFont>
#include <QTimer>
#include <QDebug>
#include <QFontMetrics>
#include <QMouseEvent>

#define CIRCLE_NUM      1000
#define CIRCLE_RADIUS   3
#define CIRCLE_MOVETIME 400
#define CIRCLE_RATE     8

MainView::MainView(QWidget *parent)
    : QWidget(parent)
{
    QFont font = this->font();
    font.setPixelSize(128);
    font.setFamily("Consolas");
    setFont(font);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setMouseTracking(true);

    resize(800,400);
    createCircles();

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();

    connect(timer, &QTimer::timeout, this, [this]{
        QImage img = QImage(size(),QImage::Format_RGB888);
        QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss");    //![1] 获取时间文本

        QPainter painter(&img);                                                     //![2] 时间文本转成图片
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setBrush(Qt::black);
        painter.setFont(this->font());
        painter.fillRect(img.rect(),Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, currentTime);

        m_tagPts.clear();                                                           //![3] 图片灰度处理，提取出文字坐标
        for(int x=0; x<img.width(); x+=4){
            for(int y=0; y<img.height(); y+=4){
                if(qGray(img.pixel(x,y)) < 127)
                    m_tagPts.append(QPoint(x,y));
            }
        }
        this->updateGraphics();                                                     //![4] 刷新时钟
    });
}

MainView::~MainView()
{
}

void MainView::createCircles()
{   //![1] 初始化圆圈阵列
    int radius = qMin(size().width(),size().height())/2 - 10;
    QPoint opt = QPoint(size().width()/2, size().height()/2);
    qfloat16 angle = 0;
    for(int i=0; i<CIRCLE_NUM; ++i){
        QPoint cp;
        angle = (qfloat16)QRandomGenerator::global()->bounded(0,360)*3.1415926/180.0;
        cp.setX(opt.x() + qCos( angle) * radius);
        cp.setY(opt.y() + qSin( angle) * radius);

        m_circles.append(new Circle(cp, CIRCLE_RADIUS));
    }
}

void MainView::updateGraphics()
{
    //![1] 动画帧率控制
    static quint8 s_updateTimes = 0;
    s_updateTimes++;
    if(s_updateTimes == CIRCLE_RATE){
        s_updateTimes = 0;
        return;
    }

    //![2] 取出所需要绘制的点
    m_bUpdateGraphics = true;
    int count = qMin(m_circles.count(), m_tagPts.count());

    //![3] 更新当前帧点的位置，按移动距离*（当前帧/总帧）来算位置
    for(int i=0; i<count; ++i){
        if(m_circles[i]->endPt() == m_tagPts[i])
            continue;
        qfloat16 dx = m_circles[i]->startPt().x() - m_tagPts[i].x();
        qfloat16 dy = m_circles[i]->startPt().y() - m_tagPts[i].y();
        qfloat16 d  = qfloat16(s_updateTimes) / qfloat16(CIRCLE_RATE);

        m_circles[i]->setEndPt(QPoint(m_circles[i]->startPt().x() - dx*d,
                                      m_circles[i]->startPt().y() - dy*d));
        m_circles[i]->setStartPt(QPoint(m_circles[i]->startPt().x() - dx*d,
                                      m_circles[i]->startPt().y() - dy*d));

    }

    //![4] 当此次刷新点比上次多的时候，多出点就按圆圈动画聚拢
    if( m_circles.count() > count){
        int radius = this->fontMetrics().height();
        QPoint opt = QPoint(m_tagPts.last());
        qfloat16 angle = 0;
        for(int i=count; i<m_circles.count(); ++i){
            if(i > count+40)
                break;
            QPoint cp;
            angle = (qfloat16)QRandomGenerator::global()->bounded(0,360)*3.1415926/180.0;
            cp.setX(opt.x() + qCos( angle) * radius);
            cp.setY(opt.y() + qSin( angle) * radius);
            m_circles[i]->setStartPt(cp);
        }
    }

    //![5] 更新界面，并且触发下一帧
    update();
    QTimer::singleShot(CIRCLE_MOVETIME/CIRCLE_RATE, this, SLOT(updateGraphics()));
    m_bUpdateGraphics = false;
}

void MainView::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    //![1] 渐变色准备
#if 0
    QConicalGradient gradient(width()/2, height()/2, m_angle);
    gradient.setColorAt(0, Qt::red);
    gradient.setColorAt(0.33, Qt::green);
    gradient.setColorAt(0.66, Qt::yellow);
    gradient.setColorAt(1, Qt::red);
#else
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(QString("#79F1A4")));
    gradient.setColorAt(1, QColor(QString("#0E5CAD")));
#endif
    painter.setBrush(gradient);

    //![2] 透明背景
    painter.fillRect(rect(), QColor(0,0,0,0));

    //![3] 绘制更新图像
    int count = qMin(m_circles.count(), m_tagPts.count());
    count = count == 0 ? m_circles.count() : count;
    for(int i=0; i<count; ++i){
        painter.drawEllipse(m_circles[i]->endPt(),m_circles[i]->radius(),m_circles[i]->radius());
    }
}
//![] 窗口拖动实现
void MainView::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        m_pressPt = e->pos();
        m_bPressed = true;
    }
}

void MainView::mouseMoveEvent(QMouseEvent *e)
{
    if(m_bPressed){
        auto curPt = e->pos() - m_pressPt;
        move(frameGeometry().topLeft() + curPt);
    }
}

void MainView::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    m_bPressed = false;
}
//![]


QPoint Circle::startPt() const
{
    return m_startPt;
}

void Circle::setStartPt(QPoint newStartPt)
{
    m_startPt = newStartPt;
}

QPoint Circle::endPt() const
{
    return m_endPt;
}

void Circle::setEndPt(QPoint newEndPt)
{
    m_endPt = newEndPt;
}

int Circle::radius() const
{
    return m_radius;
}

void Circle::setRadius(int newRadius)
{
    m_radius = newRadius;
}
