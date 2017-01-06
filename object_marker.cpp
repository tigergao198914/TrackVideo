#include "object_marker.h"
#include <iostream>

ObjectMarker::ObjectMarker( )
  :_id(1)
{

}

ObjectMarker::~ObjectMarker()
{
  clear();
}

ObjectInfo *ObjectMarker::getData( int row )
{
  if( row >= _objs.size() )
    return NULL;
  return _objs[row];
}

void ObjectMarker::addObjectViewer(ObjectViewer *view)
{
  _objViewerList.push_back( view );
}

void ObjectMarker::clear()
{
  for( auto iter=_objs.begin(); iter!=_objs.end();  )
  {
    for( auto seg= (*iter)->segs.begin(); seg != (*iter)->segs.end(); )
    {
      auto tmp = (*iter)->segs.erase( seg );
      delete *seg;
      seg = tmp;
    }
    auto tmpIter =  _objs.erase( iter );
    delete *iter;
    iter = tmpIter;
  }

  for( auto viewer = _objViewerList.begin(); viewer !=_objViewerList.end(); viewer++)
  {
    (*viewer)->onClear();
  }
}

void ObjectMarker::getMarkList(int frameNum, MarkInfo *buf, int *count )
{
  int curIndex = 0;
  for( auto iter=_objs.begin(); iter!=_objs.end(); iter++ )
  {
    for( auto seg= (*iter)->segs.begin(); seg!=(*iter)->segs.end(); seg++ )
    {
      if( (*seg)->d.startFrame==frameNum ){
        MarkInfo *marker = &buf[curIndex];
	marker->id = (*iter)->id;
	marker->bEnd = false;
        memcpy( &marker->p, &(*seg)->p, sizeof(Position) );
	curIndex++;
      }
      
      if( (*seg)->d.endFrame==frameNum ){
        MarkInfo *marker = &buf[curIndex];
	marker->id = (*iter)->id;
	marker->bEnd = true;
	curIndex++;
      }
    }
  }
  *count = curIndex;
}

void ObjectMarker::mark(ObjectInfo *obj)
{
  _snapshoter->takeSnapShot(this,  obj );
}

void ObjectMarker::deleteMark(int index)
{
  //notify viewer
  for( auto viewer = _objViewerList.begin(); viewer !=_objViewerList.end(); viewer++)
  {
    (*viewer)->onRemoveObject( index );
  }

  int i = 0;
  for( auto iter = _objs.begin(); iter!=_objs.end();)
  {
    if( i == index )
    {
      auto tmp =  _objs.erase( iter );
      ObjectInfo *info = _objs[index];
      //todo
      // for( auto seg = info->segs.begin(); seg!=info->segs.end(); seg++ )
      // {
	// info->segs.erase( seg );
	// delete *seg;
      // }
      delete *iter;
      iter = tmp;
      break;
    }
  }
}

void ObjectMarker::endMark()
{
  
}

void ObjectMarker::onReceiveSnapShot( SnapShot *s, void *handle )
{
  ObjectInfo *obj = NULL;
  ObjectSeg *objSeg = new ObjectSeg;   
  objSeg->p.width = s->width;
  objSeg->p.height = s->height;
  objSeg->p.leftUp_x = s->leftUp_x;
  objSeg->p.leftUp_y = s->leftUp_y;
  objSeg->d.startFrame = s->frameNum ;
  objSeg->d.endFrame = -1;
  objSeg->img = s->img;

  if(handle==NULL)
  {
    //need to new object
    obj = new ObjectInfo();
    obj->id = _id;
    std::cout<< _id << std::endl;
    _id++;
    _objs.push_back( obj );
    obj->segs.push_back(objSeg);
  }
  else
  {
    obj = (ObjectInfo *)handle;
    obj->segs.push_back(objSeg);
  }

  //notify viewer
  for( auto viewer = _objViewerList.begin(); viewer !=_objViewerList.end(); viewer++)
  {
    if( handle==NULL )
      (*viewer)->onAddObject(obj);
    else
      (*viewer)->onUpdateObject( obj );
  }
}

void ObjectMarker::registeSnapShoter( SnapShoter *snapshoter )
{
  _snapshoter = snapshoter;
}
