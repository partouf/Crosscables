
#include "CBDriver.h"

//-----------------------------------------------------------------------------

void typeToChalkboardName( long iType, String *sName ) {
    switch ( iType ) {
        case CB_TYPE_LABEL: sName->setValue_ansi( "CB_TYPE_LABEL" ); break;
        case CB_TYPE_EDITBOX: sName->setValue_ansi( "CB_TYPE_EDITBOX" ); break;
        case CB_TYPE_MULTILINE: sName->setValue_ansi( "CB_TYPE_MULTILINE" ); break;
        case CB_TYPE_LISTBOX: sName->setValue_ansi( "CB_TYPE_LISTBOX" ); break;
        case CB_TYPE_COMBOBOX: sName->setValue_ansi( "CB_TYPE_COMBOBOX" ); break;
        case CB_TYPE_BUTTON: sName->setValue_ansi( "CB_TYPE_BUTTON" ); break;
        case CB_TYPE_CHECKBOX: sName->setValue_ansi( "CB_TYPE_CHECKBOX" ); break;
        case CB_TYPE_RADIOBUTTON: sName->setValue_ansi( "CB_TYPE_RADIOBUTTON" ); break;
        case CB_TYPE_PROGRESSBAR: sName->setValue_ansi( "CB_TYPE_PROGRESSBAR" ); break;
        case CB_TYPE_GRID: sName->setValue_ansi( "CB_TYPE_GRID" ); break;
        case CB_TYPE_TREE: sName->setValue_ansi( "CB_TYPE_TREE" ); break;
        case CB_TYPE_WINDOW: sName->setValue_ansi( "CB_TYPE_WINDOW" ); break;
        case CB_TYPE_GROUPBOX: sName->setValue_ansi( "CB_TYPE_GROUPBOX" ); break;
        case CB_TYPE_PANEL: sName->setValue_ansi( "CB_TYPE_PANEL" ); break;
        case CB_TYPE_NOTEBOOK: sName->setValue_ansi( "CB_TYPE_NOTEBOOK" ); break;
        case CB_TYPE_NOTEBOOKTAB: sName->setValue_ansi( "CB_TYPE_NOTEBOOKTAB" ); break;
        case CB_TYPE_SCREEN: sName->setValue_ansi( "CB_TYPE_SCREEN" ); break;
        case CB_TYPE_MENU: sName->setValue_ansi( "CB_TYPE_MENU" ); break;
    }
}

void typeToChalkboardClassName( long iType, String *sName ) {
    switch ( iType ) {
        case CB_TYPE_LABEL: sName->setValue_ansi( "TCBLabel" ); break;
        case CB_TYPE_EDITBOX: sName->setValue_ansi( "TCBEditbox" ); break;
        case CB_TYPE_MULTILINE: sName->setValue_ansi( "TCBMultiline" ); break;
        case CB_TYPE_LISTBOX: sName->setValue_ansi( "TCBListbox" ); break;
        case CB_TYPE_COMBOBOX: sName->setValue_ansi( "TCBCombobox" ); break;
        case CB_TYPE_BUTTON: sName->setValue_ansi( "TCBButton" ); break;
        case CB_TYPE_CHECKBOX: sName->setValue_ansi( "TCBCheckbox" ); break;
        case CB_TYPE_RADIOBUTTON: sName->setValue_ansi( "TCBRadioButton" ); break;
        case CB_TYPE_PROGRESSBAR: sName->setValue_ansi( "TCBProgressbar" ); break;
        case CB_TYPE_GRID: sName->setValue_ansi( "TCBGrid" ); break;
        case CB_TYPE_TREE: sName->setValue_ansi( "TCBTree" ); break;
        case CB_TYPE_WINDOW: sName->setValue_ansi( "TCBWindow" ); break;
        case CB_TYPE_GROUPBOX: sName->setValue_ansi( "TCBGroupbox" ); break;
        case CB_TYPE_PANEL: sName->setValue_ansi( "TCBPanel" ); break;
        case CB_TYPE_NOTEBOOK: sName->setValue_ansi( "TCBNotebook" ); break;
        case CB_TYPE_NOTEBOOKTAB: sName->setValue_ansi( "TCBNotebookTab" ); break;
        case CB_TYPE_SCREEN: sName->setValue_ansi( "TCBScreen" ); break;
        case CB_TYPE_MENU: sName->setValue_ansi( "TCBMenu" ); break;
    }
}

void typeToBasicName( long iType, String *sName ) {
    switch ( iType ) {
        case CB_TYPE_LABEL: sName->setValue_ansi( "label" ); break;
        case CB_TYPE_EDITBOX: sName->setValue_ansi( "edit" ); break;
        case CB_TYPE_MULTILINE: sName->setValue_ansi( "multiline" ); break;
        case CB_TYPE_LISTBOX: sName->setValue_ansi( "listbox" ); break;
        case CB_TYPE_COMBOBOX: sName->setValue_ansi( "combobox" ); break;
        case CB_TYPE_BUTTON: sName->setValue_ansi( "button" ); break;
        case CB_TYPE_CHECKBOX: sName->setValue_ansi( "checkbox" ); break;
        case CB_TYPE_RADIOBUTTON: sName->setValue_ansi( "radiobutton" ); break;
        case CB_TYPE_PROGRESSBAR: sName->setValue_ansi( "progressbar" ); break;
        case CB_TYPE_GRID: sName->setValue_ansi( "grid" ); break;
        case CB_TYPE_TREE: sName->setValue_ansi( "tree" ); break;
        case CB_TYPE_WINDOW: sName->setValue_ansi( "window" ); break;
        case CB_TYPE_GROUPBOX: sName->setValue_ansi( "groupbox" ); break;
        case CB_TYPE_PANEL: sName->setValue_ansi( "panel" ); break;
        case CB_TYPE_NOTEBOOK: sName->setValue_ansi( "notebook" ); break;
        case CB_TYPE_NOTEBOOKTAB: sName->setValue_ansi( "notebooktab" ); break;
        case CB_TYPE_SCREEN: sName->setValue_ansi( "screen" ); break;
        case CB_TYPE_MENU: sName->setValue_ansi( "menu" ); break;
    }
}

long nameToType( String *sName ) {
    if ( sName->match_ansi("CB_TYPE_LABEL") ) return CB_TYPE_LABEL;
    if ( sName->match_ansi("CB_TYPE_EDITBOX") ) return CB_TYPE_EDITBOX;
    if ( sName->match_ansi("CB_TYPE_MULTILINE") ) return CB_TYPE_MULTILINE;
    if ( sName->match_ansi("CB_TYPE_LISTBOX") ) return CB_TYPE_LISTBOX;
    if ( sName->match_ansi("CB_TYPE_COMBOBOX") ) return CB_TYPE_COMBOBOX;
    if ( sName->match_ansi("CB_TYPE_BUTTON") ) return CB_TYPE_BUTTON;
    if ( sName->match_ansi("CB_TYPE_CHECKBOX") ) return CB_TYPE_CHECKBOX;
    if ( sName->match_ansi("CB_TYPE_RADIOBUTTON") ) return CB_TYPE_RADIOBUTTON;
    if ( sName->match_ansi("CB_TYPE_PROGRESSBAR") ) return CB_TYPE_PROGRESSBAR;
    if ( sName->match_ansi("CB_TYPE_GRID") ) return CB_TYPE_GRID;
    if ( sName->match_ansi("CB_TYPE_TREE") ) return CB_TYPE_TREE;
    if ( sName->match_ansi("CB_TYPE_WINDOW") ) return CB_TYPE_WINDOW;
    if ( sName->match_ansi("CB_TYPE_GROUPBOX") ) return CB_TYPE_GROUPBOX;
    if ( sName->match_ansi("CB_TYPE_PANEL") ) return CB_TYPE_PANEL;
    if ( sName->match_ansi("CB_TYPE_NOTEBOOK") ) return CB_TYPE_NOTEBOOK;
    if ( sName->match_ansi("CB_TYPE_NOTEBOOKTAB") ) return CB_TYPE_NOTEBOOKTAB;
    if ( sName->match_ansi("CB_TYPE_SCREEN") ) return CB_TYPE_SCREEN;
    if ( sName->match_ansi("CB_TYPE_MENU") ) return CB_TYPE_MENU;

    if ( sName->match_ansi("label") ) return CB_TYPE_LABEL;
    if ( sName->match_ansi("edit") ) return CB_TYPE_EDITBOX;
    if ( sName->match_ansi("multiline") ) return CB_TYPE_MULTILINE;
    if ( sName->match_ansi("listbox") ) return CB_TYPE_LISTBOX;
    if ( sName->match_ansi("combobox") ) return CB_TYPE_COMBOBOX;
    if ( sName->match_ansi("button") ) return CB_TYPE_BUTTON;
    if ( sName->match_ansi("checkbox") ) return CB_TYPE_CHECKBOX;
    if ( sName->match_ansi("radiobutton") ) return CB_TYPE_RADIOBUTTON;
    if ( sName->match_ansi("progressbar") ) return CB_TYPE_PROGRESSBAR;
    if ( sName->match_ansi("grid") ) return CB_TYPE_GRID;
    if ( sName->match_ansi("tree") ) return CB_TYPE_TREE;
    if ( sName->match_ansi("window") ) return CB_TYPE_WINDOW;
    if ( sName->match_ansi("groupbox") ) return CB_TYPE_GROUPBOX;
    if ( sName->match_ansi("panel") ) return CB_TYPE_PANEL;
    if ( sName->match_ansi("notebook") ) return CB_TYPE_NOTEBOOK;
    if ( sName->match_ansi("noteboooktab") ) return CB_TYPE_NOTEBOOKTAB;
    if ( sName->match_ansi("screen") ) return CB_TYPE_SCREEN;
    if ( sName->match_ansi("menu") ) return CB_TYPE_MENU;

    return -1;
}


//-----------------------------------------------------------------------------


void TCBFileDialogFilterItem::addPattern( const char *sPattern ) {
    String *s = new String( sPattern );
    this->addElement( s );
}

String *TCBFileDialogFilterItem::getPattern( unsigned int i ) {
    return static_cast<String *>( elementAt(i) );
}

TCBFileDialogFilterItem *TCBFileDialogFilter::addFilterItem( const char *sCaption, const char *sPattern ) {
    TCBFileDialogFilterItem *item = new TCBFileDialogFilterItem();
    item->caption.setValue_ansi( sCaption );
    this->addElement( item );

    if ( sPattern != NULL ) {
        item->addPattern( sPattern );
    }

    return item;
}

TCBFileDialogFilterItem *TCBFileDialogFilter::getItem( unsigned int i ) {
    return static_cast<TCBFileDialogFilterItem *>( elementAt(i) );
}

//-----------------------------------------------------------------------------

TCBDriver::TCBDriver() : Freeable() {
   this->aScreen = 0;
}

TCBDriver::~TCBDriver() {
}

void TCBDriver::initialize( TCBScreen *aScreen ) {
   this->aScreen = aScreen;
}

void TCBDriver::finalize() {
}

long TCBDriver::messagebox( TCBMagnet *aParent, String *sCaption, String *sText, long iType ) {
   printf( "Message '%s'\n", sText->getValue() );

   return 0;
}

String *TCBDriver::filedialog( TCBMagnet *aParent, String *sCaption, TCBFileDialogFilter *aFilter, String *sDefaultDirectory, String *sDefaultFileName, long iType ) {
    printf( "No file dialog available\n" );

    return NULL;
}
