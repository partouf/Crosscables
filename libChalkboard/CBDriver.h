
#ifndef __CBDriver_H
#define __CBDriver_H

#include <Groundfloor/Atoms/Freeable.h>
#include <Groundfloor/Bookshelfs/BValue.h>
#include <Groundfloor/Molecules/String.h>

class TCBDriver;

// special consts
#define CB_LOCATION_CENTEROFSCREEN 65535

// iRemoteType
#define CB_TYPE_LABEL         1
#define CB_TYPE_EDITBOX       2
#define CB_TYPE_MULTILINE     3
#define CB_TYPE_LISTBOX       4
#define CB_TYPE_COMBOBOX      5
#define CB_TYPE_BUTTON        6
#define CB_TYPE_CHECKBOX      7
#define CB_TYPE_RADIOBUTTON   8
#define CB_TYPE_PROGRESSBAR   9
#define CB_TYPE_GRID          10
#define CB_TYPE_TREE          11


// collectors
#define CB_TYPE_WINDOW        101
#define CB_TYPE_GROUPBOX      102
#define CB_TYPE_PANEL         103

#define CB_TYPE_NOTEBOOK      104
#define CB_TYPE_NOTEBOOKTAB   105

#define CB_TYPE_SCREEN        201

#define CB_TYPE_MENU          301


// iInfoType
#define CB_INFOTYPE_FOCUS        100

#define CB_INFOTYPE_LISTITEMADD  101
#define CB_INFOTYPE_LISTITEMDEL  102
#define CB_INFOTYPE_LISTITEMCLR  103
#define CB_INFOTYPE_LISTITEMSEL  104

#define CB_INFOTYPE_EDITTEXT        201
#define CB_INFOTYPE_CUTTEXT         202
#define CB_INFOTYPE_COPYTEXT        203
#define CB_INFOTYPE_PASTETEXT       204
#define CB_INFOTYPE_SELECTALLTEXT   205

#define CB_INFOTYPE_BARPOSITION  301
#define CB_INFOTYPE_BARSETUNDETERMINED  302

#define CB_INFOTYPE_BUTTONSTATE  401

#define CB_INFOTYPE_ALIGNMENT    501

#define CB_INFOTYPE_GRIDFULLUPDATE  601
#define CB_INFOTYPE_GRIDROWUPDATE   602
#define CB_INFOTYPE_GRIDROWADD      603
#define CB_INFOTYPE_GRIDROWDELETE   604
#define CB_INFOTYPE_GRIDCLEAR       605
#define CB_INFOTYPE_GRIDROWSELECT   606
#define CB_INFOTYPE_GRIDHEADERSUPDATE 607

#define CB_INFOTYPE_NOTEBOOKTABSELECT  701

#define CB_INFOTYPE_MENUCHECKED     801
#define CB_INFOTYPE_SETCONTEXTMENU  802

#define CB_INFOTYPE_TREEFULLUPDATE  901
#define CB_INFOTYPE_TREESELECT      902

#define CB_INFOTYPE_REPOSITION      1000



// alignment types
#define CB_ALIGN_LEFT            0
#define CB_ALIGN_CENTER          1
#define CB_ALIGN_RIGHT           2


// message box masks
#define CB_MSGBOX_OK             0x1
#define CB_MSGBOX_CANCEL         0x2
#define CB_MSGBOX_YES            0x4
#define CB_MSGBOX_NO             0x8

// possible combinations
#define CB_MSGBOX_OKCANCEL       (CB_MSGBOX_OK | CB_MSGBOX_CANCEL)
#define CB_MSGBOX_YESNO          (CB_MSGBOX_YES | CB_MSGBOX_NO)
#define CB_MSGBOX_YESNOCANCEL    (CB_MSGBOX_YES | CB_MSGBOX_NO | CB_MSGBOX_CANCEL)


// file dialog masks
#define CB_FILEDIALOG_OPEN         1
#define CB_FILEDIALOG_SAVE         2


#include "CBBase.h"

using namespace Groundfloor;

//-----------------------------------------------------------------------------

void typeToChalkboardName( long iType, String *sName );
void typeToBasicName( long iType, String *sName );
void typeToChalkboardClassName( long iType, String *sName );
long nameToType( String *sName );

//-----------------------------------------------------------------------------

class TCBFileDialogFilterItem: public Vector {
    protected:
    public:
        String caption;

        void addPattern( const char *sPattern );
        String *getPattern( unsigned int i );
};

class TCBFileDialogFilter: public Vector {
    protected:
    public:
        TCBFileDialogFilterItem *addFilterItem( const char *sCaption, const char *sPattern = NULL );
        TCBFileDialogFilterItem *getItem( unsigned int i );
};



class TCBDriver: public Freeable
{
   protected:
      TCBScreen *aScreen;

   public:
      TCBDriver();
      ~TCBDriver();

      virtual void initialize( TCBScreen *aScreen );

      virtual void create_magnet( TCBMagnet *aMagnet ) = 0;
      virtual void hide_magnet( TCBMagnet *aMagnet ) = 0;
      virtual void show_magnet( TCBMagnet *aMagnet ) = 0;
      virtual void destroy_magnet( TCBMagnet *aMagnet ) = 0;

      virtual void setinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aSourceValue ) = 0;
      virtual void getinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aDestValue ) = 0;

      virtual void finalize();

	  virtual void preparefont( TCBFont *font ) = 0;

      virtual long messagebox( TCBMagnet *aParent, String *sCaption, String *sText, long iType );
      virtual String *filedialog( TCBMagnet *aParent, String *sCaption, TCBFileDialogFilter *aFilter, String *sDefaultDirectory, String *sDefaultFileName, long iType );

      virtual void mainloop_iteration() = 0;
};


#endif // __CBDriver_H

