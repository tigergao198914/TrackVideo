#include "qdetailtablemodel.h"
#include <QColor>
#include <QSize>

QDetailTableModel::QDetailTableModel(QObject *parent)
{
    pObjectInfo = NULL;
}

QDetailTableModel::~QDetailTableModel()
{

}

// 更新表格数据
void QDetailTableModel::updateData(ObjectInfo *pInfo)
{
    pObjectInfo = pInfo;
    beginResetModel();
    endResetModel();
}

// 行数
int QDetailTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (pObjectInfo!=0)
        return pObjectInfo->segs.size();
    else
        return 0;
}

// 列数
int QDetailTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 6;
}

// 设置表格项数据
bool QDetailTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if( value.toInt() == 0 )
        return false;

    if( pObjectInfo==NULL )
      return false;

    int nColumn = index.column();
    int nRow = index.row();
    ObjectSeg *seg = pObjectInfo->segs.at(nRow);
    if( seg == NULL )
      return false;
    switch (role)
    {
    case Qt::EditRole:
    {
        if (nColumn == UPLEFT_X)
        {
           seg->p.leftUp_x = value.toInt();
        }
        else if (nColumn == UPLEFT_Y)
        {
	  seg->p.leftUp_y = value.toInt();
        }

        else if (nColumn == RIGHTDOWN_X)
        {
	   seg->p.width = value.toInt();
        }

        else if (nColumn == RIGHTDOWN_Y)
        {
	  seg->p.height = value.toInt();
        }

        else if (nColumn == STARTFRAME)
        {
          seg->d.startFrame = value.toInt();
        }
        else if (nColumn == ENDFRAME)
        {
          seg->d.endFrame = value.toInt();
        }
	 pObjectInfo->segs[nRow] = seg;

        emit dataChanged(index, index);
        return true;
    }
    default:
        return false;
    }
    return false;
}

// 表格项数据
QVariant QDetailTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (pObjectInfo==NULL)
      return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    ObjectSeg *seg = pObjectInfo->segs.at(nRow);
    if( seg == NULL )
      return QVariant();
    switch (role)
    {
    case Qt::TextColorRole:
        return QColor(Qt::black);
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (nColumn == UPLEFT_X)
	  return seg->p.leftUp_x;

        else if (nColumn == UPLEFT_Y)
	  return seg->p.leftUp_y;

        else if (nColumn == RIGHTDOWN_X)
	  return seg->p.width;

        else if (nColumn == RIGHTDOWN_Y)
	  return seg->p.height;

        else if (nColumn == STARTFRAME)
	  return seg->d.startFrame;

        else if (nColumn == ENDFRAME)
	  return seg->d.endFrame;

        return "";
    }
    default:
        return QVariant();
    }

    return QVariant();
}

// 表头数据
QVariant QDetailTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == UPLEFT_X)
                return QStringLiteral("x1");

            else if (section == UPLEFT_Y)
                return QStringLiteral("y1");

            else if (section == RIGHTDOWN_X)
                return QStringLiteral("width");

            else if (section == RIGHTDOWN_Y)
                return QStringLiteral("height");

            else if (section == STARTFRAME)
                return QStringLiteral("start frame");

            else if (section == ENDFRAME)
                return QStringLiteral("end frame");
        }
    }
    default:
        return QVariant();
    }

    return QVariant();
}

// 表格可选中
Qt::ItemFlags QDetailTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags
            = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable ;

    return flags;
}
