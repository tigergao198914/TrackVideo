#ifndef SNAPSHOTWIDGET_H
#define SNAPSHOTWIDGET_H
#include <QLabel>
#include <QWidget>
#include "onimageselectedcallback.h"

class SnapshotWidget : public QLabel
{
public:
    SnapshotWidget();
    void setimage(QImage &image);
    OnImageSelectCallback *imageCallback;
protected:
    void mouseMoveEvent(QMouseEvent *e)   Q_DECL_OVERRIDE; // 处理鼠标移动
    void mousePressEvent(QMouseEvent *e)   Q_DECL_OVERRIDE;  // 处理鼠标按下
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE; // 处理鼠标释放
    //void paintEvent(QPaintEvent *e);
private:
    void onImageSelected(QImage &image,QPoint &startPoint, QPoint &endPoint);
    QPoint origin;                            // 记录鼠标按下坐标
    QPoint end;                               // 记录鼠标释放坐标
    QImage originImage;
    QImage tmpImage;                          //curent Image
};

#endif // SNAPSHOTWIDGET_H
