
#ifndef __CocoaIntf_H
#define __CocoaIntf_H

typedef bool (*CINTF_EVENTFUNC)(unsigned long sender, unsigned long customid);
typedef bool (*CINTF_TABSELECTEVENTFUNC)(unsigned long customid, unsigned long selectedItemCustomId);
typedef bool (*CINTF_SIZEEVENTFUNC)(unsigned long sender, unsigned long customid, long width, long height);

typedef bool (*CINTF_GRIDEDITABLEFUNC)(unsigned long customid, int column, long row);
typedef long (*CINTF_GRIDNUMBEROFROWSFUNC)(unsigned long customid);
typedef char *(*CINTF_GRIDGETVALUEFUNC)(unsigned long customid, int column, long row);
typedef void (*CINTF_GRIDSETVALUEFUNC)(unsigned long customid, int column, long row, const char *value);

typedef long (*CINTF_TREEGETCHILDCOUNTFUNC)( unsigned long customid, unsigned long itemid );
typedef unsigned long (*CINTF_TREEGETCHILDIDFUNC)( unsigned long customid, unsigned long parentitemid, long index );
typedef char *(*CINTF_TREEGETVALUEFUNC)( unsigned long customid, unsigned long itemid );
typedef void (*CINTF_TREESETCUSTOMVALUEFUNC)( unsigned long customid, unsigned long itemid, unsigned long value );
typedef unsigned long (*CINTF_TREEGETCUSTOMVALUEFUNC)( unsigned long customid, unsigned long itemid );

void setCocoaMenuClickEventMethod( CINTF_EVENTFUNC func );
void setCocoaClickEventMethod( CINTF_EVENTFUNC func );
void setCocoaCloseEventMethod( CINTF_EVENTFUNC func );
void setCocoaTabSelectEventMethod( CINTF_TABSELECTEVENTFUNC func );
void setCocoaWindowResizeEventMethod( CINTF_SIZEEVENTFUNC func );

unsigned long addWindow( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid );
unsigned long addBox( int x, int y, int w, int h, const char *caption, unsigned long parent );
unsigned long addPanel( int x, int y, int w, int h, const char *caption, unsigned long parent );
unsigned long addScrollView( int x, int y, int w, int h, const char *caption, unsigned long parent );
unsigned long addTabView( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid );
unsigned long addTabViewItem( int index, const char *caption, unsigned long parent, unsigned long customid );

unsigned long addButton( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid );
unsigned long addTextFieldLabel( int x, int y, int w, int h, const char *caption, unsigned long parent );
unsigned long addTextField( int x, int y, int w, int h, const char *caption, unsigned long parent );
unsigned long addCheckbox( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid );
unsigned long addRadioButton( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid );
unsigned long addCombobox( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid );
unsigned long addProgressIndicator( int x, int y, int w, int h, const char *caption, unsigned long parent );

unsigned long newMenu( const char *caption );
unsigned long addMenuItem( const char *caption, unsigned long parent, char controlkey = 0 );
unsigned long addMenuItemSeparator( unsigned long parent );
void linkSubMenu( unsigned long parentmenu, unsigned long menuitem, unsigned long newmenu );
void setApplicationMenuBar( unsigned long obj );

void setContextMenu_NSTableView( unsigned long obj, unsigned long menu );

unsigned long addTextView( int x, int y, int w, int h, const char *caption, unsigned long parent );
unsigned long addListbox( int x, int y, int w, int h, const char *caption, unsigned long parent );

unsigned long addTableView( int x, int y, int w, int h, const char *caption, unsigned long parent );
void setupTableViewDataSource( unsigned long obj, unsigned long customid, int columncount, CINTF_GRIDEDITABLEFUNC a, CINTF_GRIDNUMBEROFROWSFUNC b, CINTF_GRIDGETVALUEFUNC c, CINTF_GRIDSETVALUEFUNC d );
void setTableViewColumnCount( unsigned long obj, unsigned long newsize );
void setTableViewColumnTitle( unsigned long obj, unsigned long index, const char *caption );
void setTableViewColumnWidth( unsigned long obj, unsigned long index, int width );
void signalTableViewRowUpdate( unsigned long obj, long row);
void signalTableViewRowCountChanged( unsigned long obj, long start, long newlen );

unsigned long addTreeView( int x, int y, int w, int h, const char *caption, unsigned long parent );
void setupTreeDataSource( unsigned long obj, unsigned long customid, CINTF_TREEGETCHILDCOUNTFUNC a, CINTF_TREEGETCHILDIDFUNC b, CINTF_TREEGETVALUEFUNC c, CINTF_TREEGETCUSTOMVALUEFUNC d, CINTF_TREESETCUSTOMVALUEFUNC e );
void signalTreeFullUpdate( unsigned long obj );

long addItem_NSPopUpButton( unsigned long obj, const char *caption );
void removeItem_NSPopUpButton( unsigned long obj, long index );
void clearItems_NSPopUpButton( unsigned long obj );
long getSelected_NSPopUpButton( unsigned long obj );
void setSelected_NSPopUpButton( unsigned long obj, long index );

long getSelected_NSTableView( unsigned long obj );
void setSelected_NSTableView( unsigned long obj, long index );

bool getState_NSButton( unsigned long obj );
void setState_NSButton( unsigned long obj, bool state, bool isRadio );

void setProgressIndicatorValue( unsigned long obj, double value );
void setProgressIndicatorIndetermined( unsigned long obj, bool indetermined );

const char *getText_NSControl( unsigned long obj );
void setText_NSControl( unsigned long obj, const char *text );
const char *getText_NSButton( unsigned long obj );
void setText_NSButton( unsigned long obj, const char *text );

const char *getText_NSTextView( unsigned long obj );
void setText_NSTextView( unsigned long obj, const char *caption );

void setTitle_Container( unsigned long obj, const char *caption );

void getSize_NSView( unsigned long obj, long *w, long *h );

unsigned long createFont( const char *name, int size );
void setFont_NSText( unsigned long obj, unsigned long fontid );

void cut_NSText( unsigned long obj );
void copy_NSText( unsigned long obj );
void paste_NSText( unsigned long obj );
void selectAll_NSText( unsigned long obj );

void reposition_NSView( unsigned long obj, unsigned long parent, int x, int y, int w, int h );
void reposition_CBCocoaTable( unsigned long obj, unsigned long parent, int x, int y, int w, int h );
void reposition_CBCocoaTextView( unsigned long obj, unsigned long parent, int x, int y, int w, int h );

void showWindow( unsigned long obj );
void closeWindow( unsigned long obj );

int cocoaMessageBox( const char *caption, const char *text, const char *button1, const char *button2, const char *button3 );

void initCocoa();
void finiCocoa();
void runCocoa( unsigned long mainwindow );
void stopCocoa();

#endif // __CocoaIntf_H
