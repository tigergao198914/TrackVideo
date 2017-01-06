#ifndef __OBJECT_MARKER_H__
#define __OBJECT_MARKER_H__
#include <vector>
#include "ui/snapshoter.h"
#include "basic.h"
#include <QImage>

enum Style
{
  Style1,
  Style2,
  Style3
};

struct Location
{
  int x;
  int y;
};

struct MarkInfo
{
  int id;
  bool bEnd;
  Position p;
};

struct ObjectSeg
{
  Position p;
  Duration d;
  QImage img;
};


struct RenderParam
{
  Style s;
  Location offset2center;
  typedef void (*DRAWFUN)( RenderParam *param, QImage *img );
  DRAWFUN  drawFun;
};

struct ObjectInfo
{
  int id;
  RenderParam renderParam;
  std::vector<ObjectSeg*> segs;
};

class ObjectViewer
{
 public:
  virtual void onAddObject(ObjectInfo *obj) {};
  virtual void onRemoveObject(int index) {};
  virtual void onUpdateObject( ObjectInfo *obj) {};
  virtual void onClear() {};
};

class ObjectMarker : public SnapShoterCallback 
{
 public:
  ObjectMarker();
  ~ObjectMarker();
  virtual void onReceiveSnapShot( SnapShot *s, void *handle );
  virtual void registeSnapShoter( SnapShoter *snapshoter );

  void addObjectViewer(ObjectViewer *viewer);
  ObjectInfo * getData(int row);

  void getMarkList( int frameNum, MarkInfo *list, int *count );
  
  void mark( ObjectInfo *obj=NULL );
  void deleteMark(int index);
  void endMark();
  
  void removeSeg();
  void addSeg();
  
  void clear();
 private:
  SnapShoter *_snapshoter;
  std::vector<ObjectInfo*> _objs;
  std::vector<ObjectViewer*> _objViewerList;
  int _id;
};
#endif
