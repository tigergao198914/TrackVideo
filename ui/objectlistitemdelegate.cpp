#include "objectlistitemdelegate.h"
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPainter>
#include <QTextEdit>
#include "stylecombobox.h"

ObjectListItemDelegate::ObjectListItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *ObjectListItemDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index) const
{
    int column = index.column();
    switch (column)
    {
        case ID:
            return new QLabel(parent);
        case PIC:
            return new QLabel(parent);
        case STYLE:
            return new StyleCombobox(parent);
        case OFFSET_X:
	  return new QTextEdit(parent);
        case OFFSET_Y:
	  return new QTextEdit(parent);
    }

}

void ObjectListItemDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QLabel *textlabel;
    StyleCombobox *style;
    QTextEdit *offset;
    QString value;
    int combIndex;
    int column = index.column();
    switch (column)
    {
        case ID:
            value = index.model()->data(index, Qt::EditRole).toString();
            textlabel = static_cast<QLabel*>(editor);
            textlabel->setText(value);
            break;
        case PIC:
            //value = index.model()->data(index, Qt::EditRole).toString() ;
            //textlabel= static_cast<QLabel*>(editor);
            //textlabel->setPixmap( QPixmap(":/images/test.jpg") );
            break;
        case STYLE:
            combIndex = index.model()->data(index, Qt::EditRole).toInt();
            style = static_cast<StyleCombobox*>(editor);
            style->setCurrentIndex(combIndex);
            break;
        case OFFSET_X:
	    offset = static_cast<QTextEdit*>(editor);
	    value = index.model()->data(index, Qt::EditRole).toString();
	    offset->setText(value);
	    break;
        case OFFSET_Y:
	    offset = static_cast<QTextEdit*>(editor);
	    value = index.model()->data(index, Qt::EditRole).toString();
	    offset->setText(value);
	    break;
    }
;
}

void ObjectListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if( index.column()==1 ){
        QLabel *image = static_cast<QLabel*>(editor);
        QPixmap value = index.model()->data(index, Qt::EditRole).value<QPixmap>();
        image->setPixmap(value);
        model->setData(index, value, Qt::EditRole);
    }
    else if( index.column()==0 )
    {
        QLabel *text = static_cast<QLabel*>(editor);
        QString value = index.model()->data(index, Qt::EditRole).toString();
        text->setText(value);
        model->setData(index, value, Qt::EditRole);
    }
    else if( index.column()==2  )
    {
        StyleCombobox *style = static_cast<StyleCombobox*>(editor);
        int value = style->currentIndex()+1;
        style->setCurrentIndex(value);
        model->setData(index, value, Qt::EditRole);
    }
    else if( index.column()==3 )
    {
        QTextEdit *offset_x = static_cast<QTextEdit*>(editor);
        int  value =  index.model()->data(index, Qt::EditRole).toInt();
        offset_x->setText( QString::number(value) );
        model->setData(index, value, Qt::EditRole);
    }
    else if( index.column() == 4 )
    {
        QTextEdit *offset_y = static_cast<QTextEdit*>(editor);
        int  value =  index.model()->data(index, Qt::EditRole).toInt();
        offset_y->setText( QString::number(value) );
        model->setData(index, value, Qt::EditRole);
    }
}

void ObjectListItemDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}

void ObjectListItemDelegate::paint(QPainter * painter,
                           const QStyleOptionViewItem & option,
                           const QModelIndex & index) const
{
    if( index.column()==1 )
    {
        const QAbstractItemModel * model=index.model();
        QPixmap value = index.model()->data(index, Qt::EditRole).value<QPixmap>();
        //painter->drawPixmap( 50 ,(index.row())*100,value);
        painter->drawImage(option.rect,value.toImage());
    }
    /*
    else if( index.column()==2 )
    {
        int styleIndex = index.model()->data(index, Qt::EditRole).toInt();
        QString s;
        switch( styleIndex )
        {
            case 1:
                 s = "play.png";
                 break;
            case 2:
                 s = "plus.png";
                 break;
            case 3:
                s = "minus.png";
                break;
             default:
                s = "play.png";
                break;
        }
        QPixmap value = QPixmap(":/images/"+s);
        //painter->drawPixmap( 200 ,(index.row())*100+30,value);
        painter->drawImage(option.rect,value.toImage());
        
    }
    */
    else
    {
        QStyledItemDelegate::paint(painter,option,index);
        return;
    }
}
