#include "objectinfodetailpanel.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QToolButton>
#include <iostream>


ObjectInfoDetailPanel::ObjectInfoDetailPanel(ObjectMarker  *m)
    :pObjectInfo(0)
{
  _marker = m;
  //setAutoFillBackground(true);
  //setStyleSheet("background-color:white ; border:1px solid grey");
  pVBoxLayout = new QVBoxLayout;
  pHboxLayout = new QHBoxLayout;
  detailTable = new QTableView(this);
  QHeaderView *header = new QHeaderView(Qt::Horizontal);
  detailTable->setHorizontalHeader (header);
  model = new QDetailTableModel(this);
  detailTable->setModel(model);
  detailTable->setColumnWidth(0, 50);
  detailTable->setColumnWidth(1, 50);
  detailTable->setColumnWidth(2, 50);
  detailTable->setColumnWidth(3, 50);
  detailTable->setColumnWidth(4, 80);
  detailTable->setColumnWidth(5, 80);
  detailTable->verticalHeader()->hide();
  pVBoxLayout->addWidget(detailTable);

  addBtn = new QToolButton(this);
  addBtn->setIcon(QIcon(QString::fromLatin1(":/images/plus.png")));
  removeBtn = new QToolButton(this);
  removeBtn->setIcon(QIcon(QString::fromLatin1(":/images/minus.png")));
  pHboxLayout->addWidget(addBtn);
  pHboxLayout->addWidget(removeBtn);
  pHboxLayout->setAlignment(Qt::AlignLeft);
  pVBoxLayout->addLayout(pHboxLayout);

  pVBoxLayout->setSpacing(0);
  pVBoxLayout->setMargin(0);

  connect(addBtn, SIGNAL(clicked()), SLOT(addSegClick()));
  connect(removeBtn, SIGNAL(clicked()), SLOT(removeSegClick()));
  setLayout(pVBoxLayout);
}

void ObjectInfoDetailPanel::addSegClick()
{
    if(_marker){
        _marker->mark(pObjectInfo);
    }
}

void ObjectInfoDetailPanel::removeSegClick()
{
    if(detailTable->selectionModel()->selectedIndexes().size()>0)
    {
        int nRow = detailTable->selectionModel()->selectedIndexes()[0].row();
        int i = 0;
	
        for( auto iter=pObjectInfo->segs.begin(); iter!= pObjectInfo->segs.end(); iter++)
        {
            if(i==nRow)
            {
                pObjectInfo->segs.erase(iter);
                break;
            }
            i++;
        }
	
        //model->removeRow(nRow);
        model->updateData(pObjectInfo);
        //callback->setObject(NULL);
    }
}

void clearLayout(QLayout *layout)
{
    while (layout->count()>0)
    {
        QLayoutItem* item = layout->takeAt(0);
        if (item->widget())
        {
            delete item->widget();
        }
        if (item->layout())
        {
            clearLayout(item->layout());
            delete item->layout();
        }
    }
}

void ObjectInfoDetailPanel::setObject( ObjectInfo *obj )
{
    //clearLayout(pVBoxLayout);
    pObjectInfo = obj;
    model->updateData(obj);
    /*
    QToolButton *addButton = new QToolButton(this);
    addButton->setIcon(QIcon(QString::fromLatin1(":/images/plus.png")));
    std::vector<ObjectInfoSeg>::iterator iter;
    for( iter = obj->_info.begin(); iter!=obj->_info.end(); iter++ )
    {
        addSeg(*iter);
    }

    QHBoxLayout *pHBoxLayoutAdd = new QHBoxLayout();
    pHBoxLayoutAdd->addStretch();
    pHBoxLayoutAdd->addWidget(addButton);
    pVBoxLayout->addLayout(pHBoxLayoutAdd);
    pVBoxLayout->addStretch();
    */

}



