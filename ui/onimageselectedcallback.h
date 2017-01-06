#ifndef ONIMAGESELECTEDCALLBACK_H
#define ONIMAGESELECTEDCALLBACK_H

class OnImageSelectCallback
{
public:
   virtual void OnImageSelect(QImage &image, QPoint &startPoint, QPoint &endPoint) = 0;
};


#endif // ONIMAGESELECTEDCALLBACK_H
