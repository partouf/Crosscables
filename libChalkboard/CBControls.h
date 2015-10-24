
#ifndef __CBControls_H
#define __CBControls_H

#include "CBBase.h"
#include <Groundfloor/Bookshelfs/BValue.h>
#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Molecules/Property.h>

#include <Groundfloor/Bookshelfs/BTable.h>
#include <Groundfloor/Bookshelfs/BNode.h>


class TCBWindow;
class TCBGroupbox;
class TCBPanel;

class TCBLabel;
class TCBMagnetWithContextMenu;
class TCBTextControl;
class TCBEditbox;
class TCBMultiline;
class TCBListControl;
class TCBListbox;
class TCBCombobox;
class TCBButton;
class TCBStateButton;
class TCBCheckbox;
class TCBRadiobutton;

class TCBProgressbar;
class TCBGrid;
class TCBTree;

class TCBNotebookTab;
class TCBNotebook;

class TCBMenu;
class TCBMenuSeparator;

class TCBDefaultEditMenu;


using namespace Groundfloor;

class TCBWindow: public TCBCollector {
   protected:
   public:
      TCBWindow( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBWindow();

      TCBMenu *mainMenu;

      TCBMagnet *getWindow();
};

class TCBGroupbox: public TCBCollector {
   protected:
   public:
      TCBGroupbox( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBGroupbox();
};

class TCBPanel: public TCBCollector {
   protected:
   public:
      TCBPanel( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBPanel();
};

class TCBLabel: public TCBMagnet {
   protected:
   public:
      TCBLabel( int x = 0, int y = 0, int w = 0, int h = 16 );
      ~TCBLabel();

      Property<int> alignment;

      void setAlignment( int i );
};

class TCBSplitPanel: public TCBCollector {
    protected:
    public:
        TCBSplitPanel();
        ~TCBSplitPanel();

        Property<long> minimumSize;
        Property<long> maximumSize;
        Property<long> size;
};

class TCBSplitter: public TCBMagnet {
    protected:
    public:
        TCBSplitter( int x = 0, int y = 0, int w = 0, int h = 0 );
        ~TCBSplitter();

        TCBSplitPanel panelOne;
        TCBSplitPanel panelTwo;

        Property<int> type;
};

class TCBMagnetWithContextMenu: public TCBMagnet {
   protected:
   public:
      TCBMagnetWithContextMenu( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBMagnetWithContextMenu();

      Property<TCBMenu *> defaultmenu;
      Property<bool> shouldShowDefaultMenu;

      Property<TCBMenu *> contextmenu;
      Property<bool> shouldShowContextMenu;

      void setContextMenu( TCBMenu *menu );
};

class TCBTextControl: public TCBMagnetWithContextMenu {
   protected:
   public:
      TCBTextControl( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBTextControl();

      void setParent( TCBCollector *aParent );

      void cut();
      void copy();
      void paste();
      void selectall();
};

class TCBEditbox: public TCBTextControl {
   protected:
   public:
      TCBEditbox( int x = 0, int y = 0, int w = 0, int h = 23 );
      ~TCBEditbox();
};

class TCBMultiline: public TCBTextControl {
   protected:
   public:
      TCBMultiline( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBMultiline();
};

class TCBListControlItem: public Freeable {
   public:
      BValue value;
      int id;
};

class TCBListControl: public TCBMagnetWithContextMenu {
   protected:
      Vector *items;
   public:
      TCBListControl( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBListControl();

      Property<int> selected;

      void setSelectedItem( int iIndex );
      int getSelectedItem();

      int addItem( String *sValue, int iId = -1 );
      int addItem( BValue *sValue, int iId = -1 );
      void removeItem( int iIndex );

      int count();
      TCBListControlItem *getItem( int iIndex );

      void clearItems();
};

class TCBListbox: public TCBListControl {
   protected:
   public:
      TCBListbox( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBListbox();
};

class TCBCombobox: public TCBListControl {
   protected:
   public:
      TCBCombobox( int x = 0, int y = 0, int w = 0, int h = 27 );
      ~TCBCombobox();
};


class TCBButton: public TCBMagnet {
   protected:
   public:
      TCBButton( int x = 0, int y = 0, int w = 0, int h = 27 );
      ~TCBButton();
};

class TCBStateButton: public TCBMagnet {
   protected:
   public:
      TCBStateButton( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBStateButton();

      Property<bool> state;

      void setState( bool bState );
      bool getState();
};

class TCBCheckbox: public TCBStateButton {
   protected:
   public:
      TCBCheckbox( int x = 0, int y = 0, int w = 0, int h = 22 );
      ~TCBCheckbox();
};

class TCBRadiobutton: public TCBStateButton {
   protected:
   public:
      TCBRadiobutton( int x = 0, int y = 0, int w = 0, int h = 22 );
      ~TCBRadiobutton();
};

class TCBProgressbar: public TCBMagnet {
   public:
      TCBProgressbar( int x = 0, int y = 0, int w = 0, int h = 23 );
      ~TCBProgressbar();

      Property<int> min;
      Property<int> max;
      Property<int> pos;

      void setPos( int i );
      int getPos();

      void setMin( int i );
      void setMax( int i );

      Property<bool> undetermined;
      void setUndetermined( bool b );
};

class TCBGridHeader: public Freeable {
   public:
      TCBGridHeader();
      TCBGridHeader( int column, const char *caption, int width );
      ~TCBGridHeader();

      Property<int> column;
      StringProperty caption;
      Property<int> width;
};

class TCBGridHeaders: public Vector {
   public:
      TCBGridHeaders();
      ~TCBGridHeaders();

      TCBGridHeader *newHeader( int column, const char *caption, int width );
      TCBGridHeader *getHeader( int index );

      void newHeadersFromTableFields( BFields *fields, int iDefaultWidth = 100 );
};

class TCBGrid: public TCBMagnetWithContextMenu {
   public:
      TCBGrid( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBGrid();

      Property<BTable *> data;
      Property<bool> headersVisible;
      Property<int> selected;

      TCBGridHeaders headers;

      void signalUpdateHeaders();

      void signalUpdateAllRows();
      void signalUpdateRow( int i );

      void signalClearAllRows();
      void signalDeleteRow( int i );
      void signalInsertRow( int i );

      void setSelectedRow( int i );
      int getSelectedRow();

      unsigned int getRowCount();
};

class TCBTree: public TCBMagnetWithContextMenu {
   public:
      TCBTree( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBTree();

      Property<BNode *> data;
      Property<BNode *> selected;

      StringProperty displayfield;

      void setData( BNode *aNode );

      void signalUpdateAll();

      void setSelectedNode( BNode *node );
      BNode *getSelectedNode();

/*
      void openNode( BNodePath *path );
      void closeNode( BNodePath *path );
      void deleteNode( BNodePath *path );
      void insertNode( BNodePath *path, BNode *node );
      void selectNode( BNodePath *path );
      BNode *getNode( BNodePath *path );

      void signalUpdateNode( BNodePath *path );
      void signalUpdateSelection();

      BNodePath *getSelection();
*/
};

class TCBNotebookTab: public TCBCollector {
   protected:
   public:
      TCBNotebookTab();
      ~TCBNotebookTab();

      bool isSelected();
      bool isActuallyVisible();
};

class TCBNotebook: public TCBCollector {
   protected:
   public:
      TCBNotebook( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBNotebook();

      Property<int> selected;

      void setSelectedTab( int iIndex );
      int getSelectedTab();

      TCBNotebookTab *newTab( const char *sTitle );
};

class TCBMenu: public TCBCollector {
   protected:
   public:
      TCBMenu();
      TCBMenu( const char *sCaption );
      ~TCBMenu();

      Property<char> controlkey;
      Property<bool> checked;
      void setChecked( bool check );

      TCBCollector *getNonMenuParent();
};

class TCBMenuSeparator: public TCBMenu {
   protected:
   public:
      TCBMenuSeparator();
      ~TCBMenuSeparator();
};

class TCBDefaultEditMenu: public TCBMenu {
   protected:
      TCBTextControl *pObject;

      void createDefaultMenu();
   public:
      TCBDefaultEditMenu( TCBTextControl *object );
      ~TCBDefaultEditMenu();

      void cut( TCBMagnet *magnet );
      void copy( TCBMagnet *magnet );
      void paste( TCBMagnet *magnet );
      void selectall( TCBMagnet *magnet );
};

#endif // __CBControls_H
