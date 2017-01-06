#ifndef QDETAILTABLEMODEL_H
#define QDETAILTABLEMODEL_H
#include<QAbstractTableModel>
#include "../object_marker.h"

enum DetailType
{
    UPLEFT_X,
    UPLEFT_Y,
    RIGHTDOWN_X,
    RIGHTDOWN_Y,
    STARTFRAME,
    ENDFRAME
};

class QDetailTableModel : public QAbstractTableModel
{
public:
   QDetailTableModel(QObject *parent);

   ~QDetailTableModel();

   // 更新表格数据
   void updateData(ObjectInfo *pInfo);

   // 行数
   int rowCount(const QModelIndex &parent) const;

   // 列数
   int columnCount(const QModelIndex &parent) const;

   // 设置表格项数据
   bool setData(const QModelIndex &index, const QVariant &value, int role);


   // 表格项数据
   QVariant data(const QModelIndex &index, int role) const;

   // 表头数据
   QVariant headerData(int section, Qt::Orientation orientation, int role) const;

   // 表格可选中
   Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    ObjectInfo *pObjectInfo;

};

#endif // QDETAILTABLEMODEL_H
