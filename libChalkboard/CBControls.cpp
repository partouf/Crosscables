
#include "CBControls.h"

#define min(a,b)            (((a) < (b)) ? (a) : (b))
#define max(a,b)            (((a) > (b)) ? (a) : (b))


TCBWindow::TCBWindow( int x, int y, int w, int h ) : TCBCollector( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_WINDOW;
   this->mainMenu = NULL;
}

TCBWindow::~TCBWindow() {
}

TCBMagnet *TCBWindow::getWindow() {
    return this;
}

TCBGroupbox::TCBGroupbox( int x, int y, int w, int h ) : TCBCollector( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_GROUPBOX;
}

TCBGroupbox::~TCBGroupbox() {
}

TCBPanel::TCBPanel( int x, int y, int w, int h ) : TCBCollector( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_PANEL;
}

TCBPanel::~TCBPanel() {
}

TCBLabel::TCBLabel( int x, int y, int w, int h ) : TCBMagnet( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_LABEL;

   alignment.set( CB_ALIGN_LEFT );

   alignment.setNotify( GFCreateNotify( int, TCBLabel, this, &TCBLabel::setAlignment ) );
}

TCBLabel::~TCBLabel() {
}

void TCBLabel::setAlignment( int i ) {
   alignment.internalSet( i );

   BValue *val = new BValue();
   val->setInteger( i );
   setInfo( CB_INFOTYPE_ALIGNMENT, val );
   delete val;
}

//------------------------------------------------------------------------------

TCBMagnetWithContextMenu::TCBMagnetWithContextMenu( int x, int y, int w, int h ) : TCBMagnet( x, y, w, h ) {
   this->defaultmenu.internalSet( NULL );
   this->contextmenu.internalSet( NULL );
   contextmenu.setNotify( GFCreateNotify( TCBMenu *, TCBMagnetWithContextMenu, this, &TCBMagnetWithContextMenu::setContextMenu ) );

   shouldShowDefaultMenu.internalSet( true );
   shouldShowContextMenu.internalSet( false );
}

TCBMagnetWithContextMenu::~TCBMagnetWithContextMenu() {
}

void TCBMagnetWithContextMenu::setContextMenu( TCBMenu *menu ) {
   this->contextmenu.internalSet( menu );

   setInfo( CB_INFOTYPE_SETCONTEXTMENU, NULL );
}

//------------------------------------------------------------------------------

TCBDefaultEditMenu::TCBDefaultEditMenu( TCBTextControl *object ) : TCBMenu() {
   pObject = object;

   createDefaultMenu();
}

TCBDefaultEditMenu::~TCBDefaultEditMenu() {
}

void TCBDefaultEditMenu::createDefaultMenu() {
   TCBMenu *pMenuItem;

   pMenuItem = new TCBMenu( "Cut" );
   pMenuItem->parent.set( this );
   pMenuItem->onClick += GFCreateNotify( TCBMagnet *, TCBDefaultEditMenu, this, &TCBDefaultEditMenu::cut );

   pMenuItem = new TCBMenu( "Copy" );
   pMenuItem->parent.set( this );
   pMenuItem->onClick += GFCreateNotify( TCBMagnet *, TCBDefaultEditMenu, this, &TCBDefaultEditMenu::copy );

   pMenuItem = new TCBMenu( "Paste" );
   pMenuItem->parent.set( this );
   pMenuItem->onClick += GFCreateNotify( TCBMagnet *, TCBDefaultEditMenu, this, &TCBDefaultEditMenu::paste );

   pMenuItem = new TCBMenuSeparator();
   pMenuItem->parent.set( this );

   pMenuItem = new TCBMenu( "Select All" );
   pMenuItem->parent.set( this );
   pMenuItem->onClick += GFCreateNotify( TCBMagnet *, TCBDefaultEditMenu, this, &TCBDefaultEditMenu::selectall );
}

void TCBDefaultEditMenu::cut( TCBMagnet *magnet ) {
   pObject->cut();
}

void TCBDefaultEditMenu::copy( TCBMagnet *magnet ) {
   pObject->copy();
}

void TCBDefaultEditMenu::paste( TCBMagnet *magnet ) {
   pObject->paste();
}

void TCBDefaultEditMenu::selectall( TCBMagnet *magnet ) {
   pObject->selectall();
}

//------------------------------------------------------------------------------

TCBTextControl::TCBTextControl( int x, int y, int w, int h ) : TCBMagnetWithContextMenu( x, y, w, h ) {
   this->defaultmenu.internalSet( new TCBDefaultEditMenu( this ) );

   this->parent.setNotify( GFCreateNotify( TCBCollector *, TCBTextControl, this, &TCBTextControl::setParent ) );
}

TCBTextControl::~TCBTextControl() {
}

void TCBTextControl::setParent( TCBCollector *aParent ) {
   TCBMagnet::setParent( aParent );

   this->defaultmenu.internalGet()->setParent( aParent );
}

void TCBTextControl::cut() {
   setInfo( CB_INFOTYPE_CUTTEXT, NULL );
}
void TCBTextControl::copy() {
   setInfo( CB_INFOTYPE_COPYTEXT, NULL );
}
void TCBTextControl::paste() {
   setInfo( CB_INFOTYPE_PASTETEXT, NULL );
}
void TCBTextControl::selectall() {
   setInfo( CB_INFOTYPE_SELECTALLTEXT, NULL );
}

//------------------------------------------------------------------------------

TCBEditbox::TCBEditbox( int x, int y, int w, int h ) : TCBTextControl( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_EDITBOX;
}

TCBEditbox::~TCBEditbox() {
}

//------------------------------------------------------------------------------

TCBMultiline::TCBMultiline( int x, int y, int w, int h ) : TCBTextControl( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_MULTILINE;
}

TCBMultiline::~TCBMultiline() {
}

//------------------------------------------------------------------------------

TCBListControl::TCBListControl( int x, int y, int w, int h ) : TCBMagnetWithContextMenu( x, y, w, h ) {
   items = new Vector();

   selected.set( -1 );

   selected.setNotify( GFCreateNotify( int, TCBListControl, this, &TCBListControl::setSelectedItem ) );
   selected.setRetreive( GFCreateRetreive( int, TCBListControl, this, &TCBListControl::getSelectedItem ) );
}

TCBListControl::~TCBListControl() {
   clearItems();
   delete items;
}

int TCBListControl::addItem( String *sValue, int iId ) {
   TCBListControlItem *item = new TCBListControlItem();

   item->id = iId;
   item->value.setString( sValue );

   int i = items->addElement( item );
   if ( i != -1 ) {
      BValue *val = new BValue();
      val->setInteger( i );
      setInfo( CB_INFOTYPE_LISTITEMADD, val );
      delete val;
   } else {
      delete item;
   }

   selected.set( selected.internalGet() );

   return i;
}

int TCBListControl::addItem( BValue *sValue, int iId ) {
   TCBListControlItem *item = new TCBListControlItem();

   item->id = iId;
   item->value.setValue( sValue );

   int i = items->addElement( item );
   if ( i != -1 ) {
      BValue *val = new BValue();
      val->setInteger( i );
      setInfo( CB_INFOTYPE_LISTITEMADD, val );
      delete val;
   } else {
      delete item;
   }

   selected.set( selected.internalGet() );

   return i;
}

void TCBListControl::removeItem( int iIndex ) {
   BValue *value = static_cast<BValue *>( items->removeElement( iIndex ) );
   if ( value != NULL ) {
      items->compress();

      BValue *val = new BValue();
      val->setInteger( iIndex );
      setInfo( CB_INFOTYPE_LISTITEMDEL, val );
      delete val;

      delete value;
   }

   selected.set( selected.internalGet() );
}

int TCBListControl::count() {
   return items->size();
}

TCBListControlItem *TCBListControl::getItem( int iIndex ) {
   return static_cast<TCBListControlItem *>( items->elementAt( iIndex ) );
}

void TCBListControl::clearItems() {
   items->clear();

   setInfo( CB_INFOTYPE_LISTITEMCLR, NULL );

   selected.set( selected.internalGet() );
}

void TCBListControl::setSelectedItem( int iIndex ) {
   int i = iIndex;
   int c = items->size() - 1;
   if ( iIndex < 0 ) {
      i = 0;
   } else if ( iIndex > c ) {
      i = c;
   }

   BValue *value = new BValue();
   value->setInteger( i );
   setInfo( CB_INFOTYPE_LISTITEMSEL, value );
   i = value->asInteger();
   delete value;

   selected.internalSet( i );
}

int TCBListControl::getSelectedItem() {
   int i = selected.internalGet();

   BValue *value = new BValue();
   value->setInteger( i );
   getInfo( CB_INFOTYPE_LISTITEMSEL, value );
   i = value->asInteger();
   delete value;

   selected.internalSet( i );

   return i;
}

//------------------------------------------------------------------------------

TCBListbox::TCBListbox( int x, int y, int w, int h ) : TCBListControl( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_LISTBOX;
}

TCBListbox::~TCBListbox() {
}

//------------------------------------------------------------------------------

TCBCombobox::TCBCombobox( int x, int y, int w, int h ) : TCBListControl( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_COMBOBOX;
}

TCBCombobox::~TCBCombobox() {
}

//------------------------------------------------------------------------------

TCBButton::TCBButton( int x, int y, int w, int h ) : TCBMagnet( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_BUTTON;
}

TCBButton::~TCBButton() {
}

//------------------------------------------------------------------------------

TCBStateButton::TCBStateButton( int x, int y, int w, int h ) : TCBMagnet( x, y, w, h ) {
   state.set( false );

   state.setNotify( GFCreateNotify( bool, TCBStateButton, this, &TCBStateButton::setState ) );
   state.setRetreive( GFCreateRetreive( bool, TCBStateButton, this, &TCBStateButton::getState ) );
}

TCBStateButton::~TCBStateButton() {
}

void TCBStateButton::setState( bool bState ) {
   state.internalSet( bState );

   BValue *value = new BValue();
   value->setBoolean( bState );
   setInfo( CB_INFOTYPE_BUTTONSTATE, value );
   delete value;
}

bool TCBStateButton::getState() {
   BValue *value = new BValue();
   value->setBoolean( state.internalGet() );
   getInfo( CB_INFOTYPE_BUTTONSTATE, value );
   bool bState = value->asBoolean();
   state.internalSet( bState );
   delete value;

   return bState;
}

//------------------------------------------------------------------------------

TCBCheckbox::TCBCheckbox( int x, int y, int w, int h ) : TCBStateButton( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_CHECKBOX;
}

TCBCheckbox::~TCBCheckbox() {
}

//------------------------------------------------------------------------------

TCBRadiobutton::TCBRadiobutton( int x, int y, int w, int h ) : TCBStateButton( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_RADIOBUTTON;
}

TCBRadiobutton::~TCBRadiobutton() {
}

//------------------------------------------------------------------------------

TCBProgressbar::TCBProgressbar( int x, int y, int w, int h ) : TCBMagnet( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_PROGRESSBAR;

   min.internalSet( 0 );
   max.internalSet( 100 );
   pos.internalSet( 0 );
   undetermined.internalSet( false );

   min.setNotify( GFCreateNotify( int, TCBProgressbar, this, &TCBProgressbar::setMin ) );
   max.setNotify( GFCreateNotify( int, TCBProgressbar, this, &TCBProgressbar::setMax ) );

   pos.setNotify( GFCreateNotify( int, TCBProgressbar, this, &TCBProgressbar::setPos ) );
   pos.setRetreive( GFCreateRetreive( int, TCBProgressbar, this, &TCBProgressbar::getPos ) );

   undetermined.setNotify( GFCreateNotify( bool, TCBProgressbar, this, &TCBProgressbar::setUndetermined ) );
}

TCBProgressbar::~TCBProgressbar() {
}

void TCBProgressbar::setPos( int i ) {
   int iMin = min.internalGet();
   int iMax = max.internalGet();

   i = max( iMin, i );
   i = min( iMax, i );

   int iPos = i - iMin;
   int diff = iMax - iMin;

   double dPos = (100.0 / diff * iPos);

   iPos = static_cast<int>( dPos );

   pos.internalSet( iPos );

   BValue *value = new BValue();
   value->setInteger( iPos );
   setInfo( CB_INFOTYPE_BARPOSITION, value );
   delete value;
}

int TCBProgressbar::getPos() {
   int iMin = min.internalGet();
   int iPos = pos.internalGet();

   int diff = max.internalGet() - iMin;
   double dPos = (diff / 100.0 * iPos);

   iPos = iMin + static_cast<int>( dPos );

   return iPos;
}

void TCBProgressbar::setMin( int i ) {
   int iPos = getPos();
   min.internalSet( i );

   setPos( iPos );
}

void TCBProgressbar::setMax( int i ) {
   int iPos = getPos();
   max.internalSet( i );

   setPos( iPos );
}

void TCBProgressbar::setUndetermined( bool b ) {
   if ( b != undetermined.internalGet() ) {
      undetermined.internalSet( b );

      BValue *val = new BValue();
      val->setBoolean( b );
      setInfo( CB_INFOTYPE_BARSETUNDETERMINED, val );
      delete val;
   }
}

//------------------------------------------------------------------------------

TCBGridHeader::TCBGridHeader() : Freeable() {
   column.internalSet( 0 );
   caption.internalSet( "" );
   width.internalSet( 100 );
}

TCBGridHeader::TCBGridHeader( int column, const char *caption, int width ) : Freeable() {
   this->column.internalSet( column );
   this->caption.internalSet( caption );
   this->width.internalSet( width );
}

TCBGridHeader::~TCBGridHeader() {
}

//------------------------------------------------------------------------------

TCBGridHeaders::TCBGridHeaders() : Vector() {
}

TCBGridHeaders::~TCBGridHeaders() {
}

TCBGridHeader *TCBGridHeaders::newHeader( int column, const char *caption, int width ) {
   TCBGridHeader *header = new TCBGridHeader( column, caption, width );
   this->addElement( header );

   return header;
}

TCBGridHeader *TCBGridHeaders::getHeader( int index ) {
   return reinterpret_cast<TCBGridHeader *>( this->elementAt(index) );
}

void TCBGridHeaders::newHeadersFromTableFields( BFields *fields, int iDefaultWidth ) {
   this->clear();

   unsigned int c = fields->size();
   for ( unsigned int i = 0; i < c; i++ ) {
      BField *field = fields->getField(i);
      newHeader( i, field->name.getValue(), iDefaultWidth );
   }
}

//------------------------------------------------------------------------------

TCBGrid::TCBGrid( int x, int y, int w, int h ) : TCBMagnetWithContextMenu( x, y, w, h ) {
   this->iRemoteType = CB_TYPE_GRID;

   this->data.internalSet( NULL );
   this->headersVisible.internalSet( true );

   this->selected.internalSet( -1 );

   selected.setNotify( GFCreateNotify( int, TCBGrid, this, &TCBGrid::setSelectedRow ) );
   selected.setRetreive( GFCreateRetreive( int, TCBGrid, this, &TCBGrid::getSelectedRow ) );
}

TCBGrid::~TCBGrid() {
}


void TCBGrid::setSelectedRow( int i ) {
   BTable *table = this->data.get();
   if ( table != NULL ) {
      BRecords *records = table->records.get();
      if ( records != NULL ) {
         int c = records->size() - 1;
         if ( i < 0 ) {
            i = 0;
         } else if ( i > c ) {
            i = c;
         }
      }
   }

   BValue *value = new BValue();
   value->setInteger( i );
   setInfo( CB_INFOTYPE_GRIDROWSELECT, value );
   selected.internalSet( value->asInteger() );
   delete value;
}

int TCBGrid::getSelectedRow() {
   int i = selected.internalGet();

   BValue *value = new BValue();
   value->setInteger( i );
   getInfo( CB_INFOTYPE_GRIDROWSELECT, value );
   i = value->asInteger();
   delete value;

   selected.internalSet( i );

   return i;
}

void TCBGrid::signalUpdateHeaders() {
   headers.compress();

   setInfo( CB_INFOTYPE_GRIDHEADERSUPDATE, NULL );
}

void TCBGrid::signalUpdateAllRows() {
   headers.compress();

   setInfo( CB_INFOTYPE_GRIDFULLUPDATE, NULL );
}

void TCBGrid::signalUpdateRow( int i ) {
   BValue *value = new BValue();
   value->setInteger( i );
   setInfo( CB_INFOTYPE_GRIDROWUPDATE, value );
   selected.internalSet( value->asInteger() );
   delete value;
}

void TCBGrid::signalClearAllRows() {
   setInfo( CB_INFOTYPE_GRIDCLEAR, NULL );
}

void TCBGrid::signalDeleteRow( int i ) {
   BValue *value = new BValue();
   value->setInteger( i );
   setInfo( CB_INFOTYPE_GRIDROWDELETE, value );
   delete value;
}

void TCBGrid::signalInsertRow( int i ) {
   BValue *value = new BValue();
   value->setInteger( i );
   setInfo( CB_INFOTYPE_GRIDROWADD, value );
   delete value;
}

unsigned int TCBGrid::getRowCount() {
   BTable *table = this->data.get();
   if ( table != NULL ) {
      BRecords *records = table->records.get();
      if ( records != NULL ) {
         return records->size();
         return 1;
      }
   }

   return 0;
}

//------------------------------------------------------------------------------

TCBTree::TCBTree( int x, int y, int w, int h ) : TCBMagnetWithContextMenu( x, y, w, h ) {
   iRemoteType = CB_TYPE_TREE;

   displayfield.internalSet( "" );

   data.internalSet( NULL );
   data.setNotify( GFCreateNotify( BNode *, TCBTree, this, &TCBTree::setData ) );

   selected.internalSet( NULL );
   selected.setNotify( GFCreateNotify( BNode *, TCBTree, this, &TCBTree::setSelectedNode ) );
   selected.setRetreive( GFCreateRetreive( BNode *, TCBTree, this, &TCBTree::getSelectedNode ) );
}

TCBTree::~TCBTree() {
}

void TCBTree::setData( BNode *aNode ) {
   data.internalSet( aNode );

   signalUpdateAll();
}

void TCBTree::signalUpdateAll() {
   setInfo( CB_INFOTYPE_TREEFULLUPDATE, NULL );
}

void TCBTree::setSelectedNode( BNode *node ) {
    selected.internalSet( node );

    setInfo( CB_INFOTYPE_TREESELECT, NULL );
}

BNode *TCBTree::getSelectedNode() {
    getInfo( CB_INFOTYPE_TREESELECT, NULL );

    return selected.internalGet();
}

//------------------------------------------------------------------------------

TCBNotebookTab::TCBNotebookTab() : TCBCollector(0, 0, 0, 0) {
   iRemoteType = CB_TYPE_NOTEBOOKTAB;
}
TCBNotebookTab::~TCBNotebookTab() {
}

bool TCBNotebookTab::isSelected() {
   TCBNotebook *p = static_cast<TCBNotebook *>( parent.get() );
   if ( p != NULL ) {
      TCBNotebookTab *tab = static_cast<TCBNotebookTab *>(  p->getVector()->elementAt( p->selected.get() ) );
      return (tab == this);
   }

   return false;
}

bool TCBNotebookTab::isActuallyVisible() {
   TCBNotebook *p = static_cast<TCBNotebook *>( parent.get() );

   bool bVisible = visible.internalGet() && isSelected();
   if ( bVisible ) {
      if ( p != NULL ) {
         return p->isActuallyVisible();
      }
   }

   return bVisible;
}

TCBNotebook::TCBNotebook( int x, int y, int w, int h ) : TCBCollector(x, y, w, h) {
   iRemoteType = CB_TYPE_NOTEBOOK;

   selected.internalSet( 0 );

   selected.setNotify( GFCreateNotify( int, TCBNotebook, this, &TCBNotebook::setSelectedTab ) );
   selected.setRetreive( GFCreateRetreive( int, TCBNotebook, this, &TCBNotebook::getSelectedTab ) );
}
TCBNotebook::~TCBNotebook() {
}

void TCBNotebook::setSelectedTab( int iIndex ) {
   int c = aMagnetList->size() - 1;
   if ( iIndex < 0 ) {
      iIndex = 0;
   } else if ( iIndex > c ) {
      iIndex = c;
   }

   BValue *value = new BValue();
   value->setInteger( iIndex );
   setInfo( CB_INFOTYPE_NOTEBOOKTABSELECT, value );
   selected.internalSet( value->asInteger() );
   delete value;
}

int TCBNotebook::getSelectedTab() {
   int i = selected.internalGet();

   BValue *value = new BValue();
   value->setInteger( i );
   getInfo( CB_INFOTYPE_NOTEBOOKTABSELECT, value );
   i = value->asInteger();
   delete value;

   selected.internalSet( i );

   return i;
}

TCBNotebookTab *TCBNotebook::newTab( const char *sTitle ) {
   TCBNotebookTab *tab = new TCBNotebookTab();
   tab->caption.set( sTitle );
   tab->parent.set( this );

   return tab;
}

//------------------------------------------------------------------------------

TCBMenu::TCBMenu() : TCBCollector() {
   this->iRemoteType = CB_TYPE_MENU;
   this->checked.internalSet( false );

   this->controlkey.internalSet( 0 );

   this->checked.setNotify( GFCreateNotify( bool, TCBMenu, this, &TCBMenu::setChecked ) );
}

TCBMenu::TCBMenu( const char *sCaption ) : TCBCollector() {
   this->iRemoteType = CB_TYPE_MENU;
   this->caption.internalSet( sCaption );
   this->checked.internalSet( false );

   this->checked.setNotify( GFCreateNotify( bool, TCBMenu, this, &TCBMenu::setChecked ) );
}

TCBMenu::~TCBMenu() {
}


void TCBMenu::setChecked( bool check ) {
   checked.internalSet( check );

   BValue *value = new BValue();
   value->setBoolean( check );
   setInfo( CB_INFOTYPE_MENUCHECKED, value );
   delete value;
}

TCBCollector *TCBMenu::getNonMenuParent() {
   TCBCollector *p = parent.get();
   if ( p != NULL ) {
      if ( p->iRemoteType == CB_TYPE_MENU ) {
         return static_cast<TCBMenu *>( p )->getNonMenuParent();
      } else {
         return p;
      }
   }

   return NULL;
}

//------------------------------------------------------------------------------

TCBMenuSeparator::TCBMenuSeparator() : TCBMenu() {
   this->caption.internalSet( "-" );
}

TCBMenuSeparator::~TCBMenuSeparator() {
}
