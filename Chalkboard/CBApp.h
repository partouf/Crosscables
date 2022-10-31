
#ifndef __CBApp_H
#define __CBApp_H

#include <Groundfloor/Atoms/Defines.h>

#include "CBControls.h"
#include "CBDriver.h"

//#define FORCE_GUI_GTK

#ifdef GF_OS_WIN32
   #ifdef FORCE_GUI_GTK
      #include "GTK/CBDriverGTK.h"
      #define GUI_GTK
   #else
      #include "Win32/CBDriverWin32.h"
      #define GUI_WIN32
   #endif
#else
   #ifdef GF_OS_MAC
      #include "Cocoa/CBDriverCocoa.h"
      #define GUI_COCOA
   #else
      #include "GTK/CBDriverGTK.h"
      #define GUI_GTK
   #endif
#endif

#include <Groundfloor/Molecules/Callbacks.h>

using namespace Groundfloor;

bool CBAppInitialize();
TCBScreen *CBAppGetScreen();
void CBAppRun( TCBWindow *mainwindow );
void CBAppAddEvent( Callable *event );
void CBAppFinalize();

#endif
