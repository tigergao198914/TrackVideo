#ifndef OBJECTINFODETAILPANEL_H
#define OBJECTINFODETAILPANEL_H
#include <QWidget>
#include <QPoint>
#include <QVBoxLayout>
#include "qdetailtablemodel.h"
#include "currentobjectchangedcallback.h"
#include <QToolButton>
#include <QTableView>
#include "object_marker.h"

class ObjectInfoDetailPanel: public QWidget,public CurrentObjectChangedCallback
{
    Q_OBJECT
public:
    ObjectInfoDetailPanel(ObjectMarker *m );
    void setObject( ObjectInfo *obj );
public slots:
    void addSegClick();
    void removeSegClick();
private:
    QTableView *detailTable;
    ObjectInfo *objectInfo;
    QDetailTableModel *model;
    QVBoxLayout *pVBoxLayout;
    QHBoxLayout *pHboxLayout;
    QToolButton *addBtn;
    QToolButton *removeBtn;
    //void addSeg(ObjectInfoSeg &obj);
    ObjectInfo* pObjectInfo;
    ObjectMarker *_marker;
    // SnapShoter *_snapShoter;
};

#endif // OBJECTINFODETAILPANEL_H
