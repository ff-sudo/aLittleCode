#ifndef MC_H
#define MC_H

#include <QWidget>
#include <QPixmap>

struct TextRect
{
    QString name;
    QRect rect;
    QPoint pos;
};
class MC : public QWidget
{
    Q_OBJECT
public:
    MC(QWidget *parent = nullptr);
    ~MC();

    static void SetMovePt(int x, int y){
        s_movePt = QPoint(x, y);
    }

    static void SetLpPt(int x, int y){
        s_lpPt = QPoint(x, y);
    }

    static void SetRpPt(int x, int y){
        s_rpPt = QPoint(x, y);
    }

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


private:
    QPixmap pixmap;
    QPoint mousePostion;
    QMap<QString, TextRect> ptMap;
    QString lastPos;
    QRect fmRect;

    static QPoint s_movePt, s_lpPt, s_rpPt;
};



#endif