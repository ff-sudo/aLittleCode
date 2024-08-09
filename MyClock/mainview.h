#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QWidget>


class Circle{
public:
    Circle(const QPoint &st, const int radius): m_startPt(st),m_endPt(st), m_radius(radius){}


    QPoint startPt() const;
    void setStartPt(QPoint newStartPt);

    QPoint endPt() const;
    void setEndPt(QPoint newEndPt);

    int radius() const;
    void setRadius(int newRadius);

private:
    QPoint  m_startPt;
    QPoint  m_endPt;
    int m_radius;
};


class MainView : public QWidget
{
    Q_OBJECT

public:
    MainView(QWidget *parent = nullptr);
    ~MainView();

    void createCircles();
    Q_INVOKABLE void updateGraphics();

    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QVector<QPoint>     m_tagPts;
    QVector<Circle*>    m_circles;
    QPoint  m_pressPt;
    quint16 m_angle{0};
    bool m_bPressed{false};
    bool m_bUpdateGraphics{false};

};
#endif // MAINVIEW_H
