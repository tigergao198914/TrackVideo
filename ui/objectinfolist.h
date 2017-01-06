#ifndef OBJECTINFOLIST_H
#define OBJECTINFOLIST_H
#include <QWidget>
#include "../object_marker.h"
#include <QStandardItemModel>
#include <QTableView>
#include "objectlistitemdelegate.h"
#include "currentobjectchangedcallback.h"
#include <vector>
#include <QToolButton>
#include "snapshoter.h"
class ObjectInfoList;
class MyTableView : public QTableView
{
    Q_OBJECT
public:
    MyTableView(ObjectInfoList* parent);
protected slots:
    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected) Q_DECL_OVERRIDE;
private:
    ObjectInfoList *pListPanel;
};

class ObjectInfoList : public QWidget, public ObjectViewer
{
    Q_OBJECT
public:
    ObjectInfoList(ObjectMarker*);
    virtual void onAddObject( ObjectInfo *pObj );
    virtual void onRemoveObject( int id );
    virtual void onUpdateObject(ObjectInfo *pObj);
    virtual void onClear();

    std::vector<ObjectInfo *> getObjects();
    void setCallback( CurrentObjectChangedCallback *cb);
    void selectionChanged(int rowIndex);
    // int mapFromSeg2Id(int segNum);
public slots:
    void addObjClick();
    void removeObjClick();
    //void currentItemShow(QItemSelection*,QItemSelection*);
    //void on_tableViewTriggerSelectionModel_currentRowChanged(QModelIndex,QModelIndex);

private:
    QStandardItemModel *pModel;
    MyTableView *tableView;
    ObjectListItemDelegate *delegate;
    CurrentObjectChangedCallback *callback;
    // std::vector<ObjectInfo *> _objs;
    QToolButton *addBtn;
    QToolButton *removeBtn;
    ObjectMarker *_marker;
};

#endif // OBJECTINFOLIST_H
