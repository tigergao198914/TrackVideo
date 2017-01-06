#include "video_track.h"
#include <QPainter>
#include "matlab/libTrack.h"
#include "image_draw.h"

bool VideoTrack::_bInit = false;

VideoTrack::VideoTrack( ObjectMarker *marker )
  :_curObjectCount(0)
  ,_objMarker( marker )
  ,_buf(NULL)
{
  if( !_bInit )
    {
      libTrackInitialize();
      _bInit = true;
    }
}

VideoTrack::~VideoTrack()
{
  if( _bInit )
    {
      libTrackTerminate();
      _bInit = false;
    }

  if( _buf )
    {
      delete [] _buf;
      _buf = NULL;
    }
}

int VideoTrack::onReceiveFrame( VideoFrame &frame)
{
  std::cout<< "VideoTrack receiver a frame:"<<frame._frameNum<<std::endl;
  feed( frame._frameNum, frame.img, frame._scale );
}

void VideoTrack::mark(  int id,  int leftUp_x,  int leftUp_y,  int width,  int height)
{
  double tmpId = id;
  double tmpx = leftUp_x;
  double tmpy = leftUp_y;
  double tmpw = width;
  double tmph = height;

  if( width <=0 || height <= 0 )
    {
      return;
    }

  std::cout<<"<mark> id:"<<id;
  std::cout<<"  leftUp_x:"<<leftUp_x<<", leftUp_y:"<<leftUp_y;
  std::cout<<"  width:"<<width<<" ,height:"<<height<<std::endl;
  
  mwArray mwid(1, 1, mxDOUBLE_CLASS);
  mwid.SetData(&tmpId, 1);
  mwArray mwleftUp_x(1, 1, mxDOUBLE_CLASS);
  mwleftUp_x.SetData(&tmpx, 1);
  mwArray mwleftUp_y(1, 1, mxDOUBLE_CLASS);
  mwleftUp_y.SetData(&tmpy, 1);
  mwArray mwwidth(1, 1, mxDOUBLE_CLASS);
  mwwidth.SetData(&tmpw, 1);
  mwArray mwheight(1, 1, mxDOUBLE_CLASS);
  mwheight.SetData(&tmph, 1);
  
  markObject( mwid, mwleftUp_x, mwleftUp_y, mwwidth, mwheight );
  _curObjectCount++;
}

void VideoTrack::endMark(  int id)
{
  std::cout<<"end mark id:"<< id<< std::endl;
  double tmpId = id;
  mwArray mwid(1, 1, mxDOUBLE_CLASS);
  mwid.SetData(&tmpId, 1);
  endMarkObject( mwid );
  _curObjectCount--;
}

void VideoTrack::feed(  int number, QImage *img, double scale )
{
  int num = 128;
  int count = 0;
  MarkInfo tmpMarkInfo[num];
  
  mwSize dims[3] = {0,0,3};
  dims[0] = img->height();
  dims[1] = img->width();
  memset( tmpMarkInfo, 0x0, sizeof(tmpMarkInfo));
  
  double tmpNum = number;
  mwArray mwNum(1,1,mxDOUBLE_CLASS);
  mwNum.SetData( &tmpNum, 1);
  
  mwArray mwPosition(1024, 5, mxDOUBLE_CLASS);
  mwArray mwBuffer((mwSize)3, dims,  mxUINT8_CLASS);


  /* convert QImage to matlab required format */
  int size = img->width()*img->height();

  if( _width != img->width() || _height != img->height() )
  {
    if( _buf != NULL )
    {
      delete [] _buf;
      _buf = NULL;
    }
  }
  
  if(_buf==NULL)
  {
    _buf =  new unsigned char[size*3];
  }
  
  _width = img->width();
  _height = img->height();
  memset( _buf , 0x0, size*3 );
  
  for( int i=0; i<img->height(); i++ )
    {
      for(int j=0; j<img->width(); j++ )
	{
	   QColor c = img->pixel(j, i);
	  _buf[ (j*img->height()+i)+size*0 ] = c.red();
	  _buf[ (j*img->height()+i)+size*1 ] = c.green();
	  _buf[ (j*img->height()+i)+size*2 ] = c.blue();
	}
    }
  
  mwBuffer.SetData( _buf, size*3 );

  if( number > 0 )
    _objMarker->getMarkList( number-1, tmpMarkInfo, &count );

  for( int index=0; index<count; index++ )
  {
    int tmpId = tmpMarkInfo[index].id;
    if( tmpMarkInfo[index].bEnd )
    {
      endMark(tmpId);
    }
    else
    {
      Position *tmpP =  &tmpMarkInfo[index].p;
      mark(tmpId, tmpP->leftUp_x, tmpP->leftUp_y, tmpP->width, tmpP->height  );
    }
  }

  /* put frame data into matlab */
  feedFrame(1, mwPosition , mwNum, mwBuffer );
  //FH_show2( mwBuffer, mwPosition);


  /* get track result from matlab */
  FrameTrackRes *frameRes = new FrameTrackRes;
  int leftup_x, leftup_y, width, height;
  size_t rowNum = mwPosition.NumberOfElements()/5 ;
  for( int i=1; i<=rowNum; i++ )
    {
       int id = mwPosition.Get( 1, i, 1);
       leftup_x = mwPosition.Get( 1, i, 2);
       leftup_y = mwPosition.Get( 1, i, 3);
       width = mwPosition.Get(1, i, 4);
       height = mwPosition.Get(1, i, 5);
       
       std::cout<< "<result> id:"<< id;
       std::cout<< "  leftup:"<< leftup_x<<","<<leftup_y;
       std::cout<< "  size:"<< width <<","<< height <<std::endl;

       TrackRes *trackRes = new TrackRes;
       trackRes->id = id;
       trackRes->p.height = height;
       trackRes->p.width  = width;
       trackRes->p.leftUp_x = leftup_x;
       trackRes->p.leftUp_y = leftup_y;
       
       _imgDraw.drawCircle(*img, &trackRes->p);
       
       trackRes->p.height = height*scale;
       trackRes->p.width  = width*scale;
       trackRes->p.leftUp_x = leftup_x*scale;
       trackRes->p.leftUp_y = leftup_y*scale;
       frameRes->res.push_back( std::shared_ptr<TrackRes>(trackRes));
    }

  /* if seek video to previous frame, remove all infos behind */
  if(number<=_videoTrackRes.size())
  {
    _videoTrackRes.erase( _videoTrackRes.begin()+number-1, _videoTrackRes.end() );
  }
  
   std::shared_ptr<FrameTrackRes> frameResPtr = std::shared_ptr<FrameTrackRes>(frameRes);
  _videoTrackRes.push_back( frameResPtr );
}

std::shared_ptr<FrameTrackRes> VideoTrack::getFrameTrackRes( int frameNum )
{
  if( _videoTrackRes.size() < frameNum )
    return nullptr;
  return _videoTrackRes[ frameNum-1 ];
}

void VideoTrack::save2File(std::string fileName)
{
  FILE *file = fopen( fileName.c_str(), "wr" );

  int frameNum = 0;
  for( auto iter = _videoTrackRes.begin(); iter!= _videoTrackRes.end(); iter++ )
  {
    frameNum++;
    for( auto trackptr = (*iter)->res.begin(); trackptr!=(*iter)->res.end(); trackptr++ )
    {
      int id = (*trackptr)->id;
      int x = (*trackptr)->p.leftUp_x;
      int y = (*trackptr)->p.leftUp_y;
      int w = (*trackptr)->p.width;
      int h = (*trackptr)->p.height;
      fprintf( file, "%d %d %d %d %d %d\n", frameNum, id, x, y, w, h);
    }
  }
  
  fflush(file);
  fclose(file);
}
