#include "objectinfolist.h"
#include "objectlistitemdelegate.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableView>
#include "stylecombobox.h"
#include <iostream>

MyTableView::MyTableView(ObjectInfoList* parent)
{
    pListPanel = parent;
}

void MyTableView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    if( this->selectedIndexes().size()<=0 )
        return;
    int rowIndex = this->selectedIndexes()[0].row();
    pListPanel->selectionChanged(rowIndex);
}

ObjectInfoList::ObjectInfoList(ObjectMarker *m)
{
    _marker = m;

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    setLayout(vboxLayout);

    pModel = new QStandardItemModel(0,5);
    QStringList headerList;
    headerList += "id";
    headerList += "object";
    headerList += "style";
    headerList += "offset_x";
    headerList += "offset_y";
    pModel->setHorizontalHeaderLabels(headerList);

    tableView = new MyTableView(this);
    tableView->verticalHeader()->hide();
    tableView->setModel(pModel);
    tableView->setColumnWidth(0, 50);
    tableView->setColumnWidth(1, 100);
    tableView->setColumnWidth(2, 50);
    tableView->setColumnWidth(3, 50);
    tableView->setColumnWidth(4, 50);
    tableView->verticalHeader()->setDefaultSectionSize(100);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    delegate = new ObjectListItemDelegate;
    tableView->setItemDelegate(delegate);

    addBtn = new QToolButton(this);
    addBtn->setIcon(QIcon(QString::fromLatin1(":/images/plus.png")));
    removeBtn = new QToolButton(this);
    removeBtn->setIcon(QIcon(QString::fromLatin1(":/images/minus.png")));
    hboxLayout->addWidget(addBtn);
    hboxLayout->addWidget(removeBtn);
    hboxLayout->setAlignment(Qt::AlignLeft);

    hboxLayout->setSpacing(0);
    vboxLayout->setSpacing(0);
    vboxLayout->setMargin(0);
    vboxLayout->addWidget(tableView);
    vboxLayout->addLayout(hboxLayout);

    connect(addBtn, SIGNAL(clicked()), SLOT(addObjClick()));
    connect(removeBtn, SIGNAL(clicked()),SLOT(removeObjClick()));
}

void ObjectInfoList::addObjClick()
{
    if(_marker){
      _marker->mark(0);
    }
}

void ObjectInfoList::removeObjClick()
{
    if(tableView->selectionModel()->selectedIndexes().size()>0)
    {
        int nRow = tableView->selectionModel()->selectedIndexes()[0].row();
        _marker->deleteMark(nRow);
    }
}

void ObjectInfoList::selectionChanged(int rowIndex)
{
  ObjectInfo *obj = _marker->getData( rowIndex );
    if(obj)
        callback->setObject(obj);
}

void ObjectInfoList::onUpdateObject(ObjectInfo *pObj)
{
    if(pObj)
        callback->setObject(pObj);
}

void ObjectInfoList::onClear()
{
  pModel->removeRows(0,pModel->rowCount());
  callback->setObject(NULL);
}

void ObjectInfoList::onAddObject( ObjectInfo *pObj )
{
    if(pObj==NULL)
        return;
    
    pModel->appendRow(new QStandardItem());
    tableView->selectRow(pModel->rowCount()-1);

    for (int column = 0; column < 3; ++column)
    {
        QModelIndex index = pModel->index(pModel->rowCount()-1, column, QModelIndex());
        if( column==0 )
        {
            pModel->setData(index, pObj->id );
        }
        else if(column == 1)
        {
	  pModel->setData(index, QVariant(QPixmap::fromImage(pObj->segs[0]->img) ));
        }
        else if( column==2)
        {
            pModel->setData(index, QVariant( pObj->renderParam.s ) );
        }
	else if( column==3)
	{
	  pModel->setData(index, QVariant(pObj->renderParam.offset2center.x ) );
        }
	else if( column==4)
	{
	  pModel->setData(index, QVariant(pObj->renderParam.offset2center.y) );
	}
    }
}


void ObjectInfoList::setCallback( CurrentObjectChangedCallback *cb)
{
    callback = cb;
}

void ObjectInfoList::onRemoveObject( int nRow )
{
   pModel->removeRow(nRow);
   std::cout<<"remove row"<< std::endl;
   callback->setObject(NULL);
}

/*
int ObjectInfoList::mapFromSeg2Id(int segNum)
{
    int i = 0;
    for( std::vector<ObjectInfo*>::iterator iter=_objs.begin();iter!=_objs.end();iter++)
    {
        ObjectInfo *tmp = (*iter);
        for( std::vector<ObjectInfoSeg>::iterator segIter=tmp->_info.begin();
              segIter!=tmp->_info.end(); segIter++)
        {
            i++;
            if( segNum==i )
            {
                return tmp->getObjectId();
            }

        }
    }
}
*/

