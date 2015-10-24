
#ifndef __CBDriverGTK_H
#define __CBDriverGTK_H

#include <Groundfloor/Atoms/Defines.h>

#ifndef GF_OS_MAC

class TCBGTKWindow;
class TCBDriverGTK;

#include "../CBDriver.h"
#include "../CBControls.h"
#include <gtk/gtk.h>

#include <Groundfloor/Atoms/Lockable.h>

/*
Implementation specific Voodoo of CBDriverGTK

Use of CBGTKWindow list
-----------------------
TCBWindow:     iRemoteId == <GtkWindow *>; widget == <GtkFixed *>
TCBGroupbox:   iRemoteId == <GtkFrame *>; widget == <GtkFixed *>
TCBPanel:      iRemoteId == <GtkFrame *>; widget == <GtkFixed *>

Use of MAGIC (magnet->magic)
----------------------------
TCBWindow:     <TCBPoint *> during window resize
TCBMultiline:  <GtkTextView *>
TCBListbox:    <GtkTreeView *>
TCBGrid:       <GtkTreeView *>
Others:        NULL
*/


void CBGTKInitialize( TCBDriverGTK *pDriver );
void CBGTKFinalize();

using namespace Groundfloor;

class TCBGTKWindow: public Freeable
{
   public:
      TCBCollector *magnet;
      GtkWidget *widget;
};

class TCBDriverGTK: public TCBDriver
{
   protected:
      String sCurrentFontName;
      int iCurrentFontSize;
      PangoFontDescription *pDefaultFont;
      PangoFontDescription *pMonospaceFont;
      void refreshItemsListbox( TCBListControl *aMagnet );
      void refreshItemsCombobox( TCBListControl *aMagnet );

      GtkListStore *getGridStore( TCBGrid *aMagnet );

      bool updateGridHeaders( TCBGrid *aMagnet, BValue *aValue );
      bool updateGridAllRows( TCBGrid *aMagnet, BValue *aValue );
      bool updateGridRow( TCBGrid *aMagnet, BValue *aValue );
      bool insertGridRow( TCBGrid *aMagnet, BValue *aValue );
      bool deleteGridRow( TCBGrid *aMagnet, BValue *aValue );

      void addMenuItem( TCBMenu *aParentMenu, TCBMenu *aMenu );

      void buildTree( TCBTree *aMagnet, BNode *aParentNode, BNode *aChildNode );
      void refreshTree( TCBTree *aMagnet );

      BNode *getSelectedItemTreeView( TCBMagnet *aMagnet );
   public:
      Lockable lGeneralAccess;
      Vector *windows;
      void buildMenu( bool popup, TCBMenu *aMenu );
      TCBGTKWindow *getGTKWindowObject( TCBMagnet *aMagnet );

      TCBDriverGTK( const char *sFontName, int iFontSize );
      ~TCBDriverGTK();

      void initialize( TCBScreen *aScreen );
      void finalize();

      void create_magnet( TCBMagnet *aMagnet );
      void hide_magnet( TCBMagnet *aMagnet );
      void show_magnet( TCBMagnet *aMagnet );
      void destroy_magnet( TCBMagnet *aMagnet );

      void setinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aSourceValue );
      void getinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aDestValue );

      long messagebox( TCBMagnet *aParent, String *sCaption, String *sText, long iType );
      String *filedialog( TCBMagnet *aParent, String *sCaption, TCBFileDialogFilter *aFilter, String *sDefaultDirectory, String *sDefaultFileName, long iType );

      void mainloop_iteration();
};

#endif // GF_OS_MAC
#endif // __CBDriverGTK_H
