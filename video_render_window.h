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

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    std::mutex _imageMutex;
    std::condition_variable _emptyCon;
    std::condition_variable _fullCon;
    QImage *_image;
    bool _bEmpty;
    void drawTextAndEllipse(QPainter *painter, std::string text, int leftX, int leftY,
			    int width, int height);
};

class VideoRenderWindow : public QWidget
{
    Q_OBJECT

public:
    VideoRenderWindow();
    void receive( QImage *image );
private:
    GLWidget *_openGL;
};

#endif
