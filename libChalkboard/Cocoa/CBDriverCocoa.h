
#ifndef __CBDriverCocoa_H
#define __CBDriverCocoa_H

#include <Groundfloor/Atoms/Defines.h>

#ifdef GF_OS_MAC

class TCBCocoaWindow;
class TCBDriverCocoa;

#include "../CBDriver.h"
#include "../CBControls.h"
#include <Groundfloor/Atoms/Lockable.h>


/*
Implementation specific Voodoo of CBDriverGTK

Use of CBCocoaWindow list
-----------------------

Use of MAGIC (magnet->magic)
----------------------------
*/

using namespace Groundfloor;

void CBCocoaInitialize( TCBDriverCocoa *pDriver );
void CBCocoaFinalize();

class TCBCocoaWindow: public Freeable
{
   public:
      TCBCollector *magnet;
      unsigned long pointer;
};

class TCBDriverCocoa: public TCBDriver
{
   protected:
      TCBMagnet *pStartingWindow;
      Lockable lGeneralAccess;

      unsigned long pDefaultFont;
      unsigned long pMonospaceFont;

      void refreshItemsListbox( TCBListControl *aMagnet );
      void refreshItemsCombobox( TCBListControl *aMagnet );

      void addMenuItem( TCBMenu *aParentMenu, TCBMenu *aMenu );
      void buildMenu( bool popup, TCBMenu *aMenu );

      TCBCocoaWindow *getWindowObject( TCBMagnet *aMagnet );
   public:
      Vector *windows;

      TCBDriverCocoa();
      ~TCBDriverCocoa();

      void initialize( TCBScreen *aScreen );
      void finalize();

      void create_magnet( TCBMagnet *aMagnet );
      void hide_magnet( TCBMagnet *aMagnet );
      void show_magnet( TCBMagnet *aMagnet );
      void destroy_magnet( TCBMagnet *aMagnet );

      void setinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aSourceValue );
      void getinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aDestValue );

      long messagebox( TCBMagnet *aParent, String *sCaption, String *sText, long iType );

      void setStartingWindow( TCBMagnet *aWindow );
      void mainloop_iteration();
};

#endif // GF_OS_MAC
#endif // __CBDriverCocoa_H
