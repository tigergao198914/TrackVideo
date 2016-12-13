#include "main_window.h"
#include <QVBoxLayout>
#include <thread>


MainWindow::MainWindow(QWidget *parent)
  :QWidget(parent)
{
  setupUI();
  _videoFileReader = new VideoFileReader;
  _videoFileRecorder = new VideoFileRecorder;

  std::shared_ptr<VideoReceiver> p(_player);
  _videoFileReader->addReceiver(p );
  std::shared_ptr<VideoReceiver> r(_videoFileRecorder);
  _videoFileReader->addReceiver(r);
  
  startEncode();
  startDecode();
}

MainWindow::~MainWindow()
{
  //delete _videoFileReader;
  //delete _videoFileRecorder;
  delete _player;
}

void MainWindow::setupUI()
{
  QVBoxLayout *mainLayout = new QVBoxLayout();
  setLayout(mainLayout);
  _player = new VideoPlayer(this);
  mainLayout->addWidget(_player);
  setWindowState(Qt::WindowMaximized);
}

void MainWindow::startDecode()
{
    _videoFileReader->config(std::string("/home/tiger/Downloads/yangge/杨哥吃螃蟹.mp4"));  
    std::thread decoderThread( &VideoFileReader::start, _videoFileReader );
    decoderThread.detach();
}

void MainWindow::startEncode()
{
  _videoFileRecorder->config("/home/tiger/Downloads/test.mp4", 720, 1080, 25, 25, 300000);
  _videoFileRecorder->start();
}
