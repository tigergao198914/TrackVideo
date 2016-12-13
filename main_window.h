#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <memory>
#include <QWidget>
#include "video_player.h"
#include "video_file_source.h"
#include "video_file_recorder.h"

namespace Ui{
  class MainWindow;
}

class MainWindow :  public QWidget
{
    Q_OBJECT
 public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
 private:
    VideoPlayer *_player;
    VideoFileReader* _videoFileReader;
    VideoFileRecorder* _videoFileRecorder;
    void setupUI();
    void startDecode();
    void startEncode();
};

#endif
