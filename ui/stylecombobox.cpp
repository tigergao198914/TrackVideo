#include "stylecombobox.h"
#include <QIcon>
#include <QSize>

StyleCombobox::StyleCombobox(QWidget *parent):
    QComboBox(parent)
{
  // addItem( QIcon(":/images/play.png"),"1" );
  // addItem( QIcon(":/images/plus.png"), "2" );
  //  addItem( QIcon(":/images/minus.png"),"3" );
  //  setIconSize( QSize(100,100));
  addItem( "style1" );
  addItem( "style2" );
  addItem( "style3" );
  setFixedHeight(50);
}
