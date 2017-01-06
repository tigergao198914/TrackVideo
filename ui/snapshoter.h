#ifndef SNAPSHOTER_H
#define SNAPSHOTER_H
#include "../object_marker.h"
#include <QImage>

struct SnapShot{
  int leftUp_x;
  int leftUp_y;
  int width;
  int height;
  QImage img;
  int frameNum;
};

class SnapShoterCallback{
 public:
  virtual void onReceiveSnapShot( SnapShot *s, void *handle ) = 0;
};

class SnapShoter{
public:
  virtual void takeSnapShot( SnapShoterCallback *cb, void *handle )=0;
};


#endif // SNAPSHOTER_H
