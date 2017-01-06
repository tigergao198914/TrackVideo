#ifndef OBJECTLISTITEMDELEGATE_H
#define OBJECTLISTITEMDELEGATE_H
#include <QStyledItemDelegate>

enum ItemColumn
{
    ID,
    PIC,
    STYLE,
    OFFSET_X,
    OFFSET_Y,
    ADD,
    REMOVE
};

class ObjectListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ObjectListItemDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;

    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    void paint(QPainter * painter,
                               const QStyleOptionViewItem & option,
                               const QModelIndex & index) const;
};
//! [0]
#endif // OBJECTLISTITEMDELEGATE_H
