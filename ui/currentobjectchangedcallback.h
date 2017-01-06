#ifndef CURRENTOBJECTCHANGEDCALLBACK_H
#define CURRENTOBJECTCHANGEDCALLBACK_H

class CurrentObjectChangedCallback
{
public:
    virtual void setObject( ObjectInfo *obj ) = 0;
};

#endif // CURRENTOBJECTCHANGEDCALLBACK_H
