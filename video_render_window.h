#ifndef __VIDEO_RENDER_WINDOW_H__
#define __VIDEO_RENDER_WINDOW_H__

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QOpenGLWidget>
#include <QFrame>
#include <string>
#include <mutex>
#include <condition_variable>

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget( QWidget *parent);

public slots:
  //void animate();
  void receive( QImage *image );
  QImage getImage();
protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    int _width;
    int _height;
    int _leftX;
    int _leftY;
    int _rightX;
    int _rightY;
    std::mutex _imageMutex;
    std::condition_variable _emptyCon;
    std::condition_variable _fullCon;
    QImage *_image;
    QImage _tmpImage;
    QImage *_fullImage;
    bool _bEmpty;
};

class VideoRenderWindow : public QWidget
{
    Q_OBJECT

public:
    VideoRenderWindow();
    void receive( QImage *image );
    QImage getImage();
private:
    GLWidget *_openGL;
};

#endif
