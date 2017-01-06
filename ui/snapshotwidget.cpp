#include "snapshotwidget.h"
#include <QMouseEvent>
#include <QPushButton>

SnapshotWidget::SnapshotWidget()
{
    imageCallback = 0;
}

void SnapshotWidget::setimage(QImage& image)
{
    //读取背景图，等比例缩放RGB值，使背景变暗，进入截图模式
    int red, green, blue;
    originImage = image;
    tmpImage = image;
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            red = qRed(image.pixel(x, y)) * 0.5;
            green = qGreen(image.pixel(x, y)) * 0.5;
            blue = qBlue(image.pixel(x, y)) * 0.5;
            tmpImage.setPixel(x, y, qRgb(red, green, blue));
        }
    }
    this->setPixmap(QPixmap::fromImage(tmpImage));
}


void SnapshotWidget::mousePressEvent(QMouseEvent *e)
{
    origin = e->pos();
}

void SnapshotWidget::mouseMoveEvent(QMouseEvent *e)
{
    {
        QImage t = tmpImage;
        // 记录终点
        end = e->pos();
        int red, green, blue;
        //调整要显示的区域
        int w = abs(end.x() - origin.x());
        int h = abs(end.y() - origin.y());
        int x = origin.x() < end.x() ? origin.x() : end.x();
        int y = origin.y() < end.y() ? origin.y() : end.y();

        for( int i=0; i<w; i++ )
        {
            for( int j=0; j<h; j++ )
            {
                red = qRed(originImage.pixel(i+x, j+y));
                green = qGreen(originImage.pixel(i+x, j+y));
                blue = qBlue(originImage.pixel(i+x, j+y));
                t.setPixel(i+x,j+y,qRgb(red, green, blue));
            }
        }
        this->setPixmap(QPixmap::fromImage(t));
    }
}

void SnapshotWidget::mouseReleaseEvent(QMouseEvent *e)
{
    {
        // 记录终点
        end = e->pos();

        int wid = abs(origin.x() - end.x());
        int height = abs(origin.y() - end.y());
        int x = origin.x() < end.x() ? origin.x() : end.x();
        int y = origin.y() < end.y() ? origin.y() : end.y();
        QImage selectImage = originImage.copy(x, y, wid, height);
        onImageSelected(selectImage, origin, end);
    }
}

void SnapshotWidget::onImageSelected(QImage &image, QPoint &startPoint, QPoint &endPoint )
{
    imageCallback->OnImageSelect(image, startPoint, endPoint );
}
