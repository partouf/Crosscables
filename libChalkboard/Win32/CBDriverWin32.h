
#ifndef __CBDriverWin32_H
#define __CBDriverWin32_H

#include <Groundfloor/Atoms/Defines.h>

#ifdef GF_OS_WIN32

#include <windows.h>

class TCBWin32Window;
class TCBDriverWin32;

#include "../CBDriver.h"
#include "../CBControls.h"
#include <Groundfloor/Atoms/Lockable.h>

using namespace Groundfloor;

LRESULT CALLBACK CBDriverWin32MessageHandler(HWND, UINT, WPARAM, LPARAM);

void CBWin32Initialize( TCBDriverWin32 *pDriver );
void CBWin32Finalize();


class TCBWin32Window: public Freeable
{
   public:
      TCBCollector *magnet;
      WNDCLASSEXW wndclass;
};


class TCBDriverWin32: public TCBDriver
{
   protected:
      Lockable lGeneralAccess;

      String sCurrentFontName;
      int iCurrentFontSize;
      HFONT hCurrentFont;
      HFONT hMonospaceFont;

      int iAcceleratorCount;
      ACCEL *pArrAccelerator;
      HACCEL hndAccelerator;

      int addControlKey( char cKey, WORD iCommandId );

      void refreshItemsListbox( TCBListControl *aMagnet );
      void refreshItemsCombobox( TCBListControl *aMagnet );

      void buildTree( TCBTree *aMagnet, BNode *aParentNode, BNode *aChildNode );
      void refreshTree( TCBTree *aMagnet );
      BNode *getSelectedItemTreeView( TCBMagnet *aMagnet );

      bool updateGridHeaders( TCBGrid *aMagnet, BValue *aValue );

      bool selectItemGrid( TCBGrid *aMagnet, BValue *aValue );
      bool getSelectedItemGrid( TCBGrid *aMagnet, BValue *aValue );

      bool insertGridRow( TCBGrid *aMagnet, BValue *aValue );
      bool updateGridRow( TCBGrid *aMagnet, BValue *aValue );
      bool updateGridAllRows( TCBGrid *aMagnet, BValue *aValue );

      void addMenuItem( TCBMenu *aParentMenu, TCBMenu *aMenu );

      void repositionWindow( TCBMagnet *aMagnet );
   public:
      void buildMenu( bool popup, TCBMenu *aMenu );
      Vector commandlist;

      Vector *windows;
      HINSTANCE hApp;

      TCBDriverWin32( HINSTANCE hApplication, const char *sFontName, int iFontSize );
      ~TCBDriverWin32();

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

	  void preparefont( TCBFont *font );

      void mainloop_iteration();
};

#endif // GF_OS_WIN32
#endif // __CBDriverWin32_H
