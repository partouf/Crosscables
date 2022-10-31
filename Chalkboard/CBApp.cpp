
#include "CBApp.h"


#include <Groundfloor/Atoms/Initialize.h>
#include <Groundfloor/Materials/GarbageCollector.h>
#include <Groundfloor/Materials/Thread.h>
#include <Groundfloor/Materials/FifoVector.h>

#include <cassert>

TCBDriver *mainGUIDriver;
TCBScreen *mainGUIScreen;

FifoVector *mainMessageQueue;

TCBScreen *CBAppGetScreen() {
   assert( mainGUIScreen != NULL );

   return mainGUIScreen;
}

bool CBAppInitialize() {
   if ( initGroundfloor() ) {
      initGlobalGarbageCollector( 100 );

      #ifdef GUI_WIN32
         mainGUIDriver = new TCBDriverWin32( NULL, "Microsoft Sans Serif", 10 );

         CBWin32Initialize( static_cast<TCBDriverWin32 *>( mainGUIDriver ) );
      #endif

      #ifdef GUI_COCOA
         mainGUIDriver = new TCBDriverCocoa();

         CBCocoaInitialize( static_cast<TCBDriverCocoa *>( mainGUIDriver ) );
      #endif

      #ifdef GUI_GTK
         mainGUIDriver = new TCBDriverGTK( "Sans", 10 );

         CBGTKInitialize( static_cast<TCBDriverGTK *>( mainGUIDriver ) );
      #endif

      mainGUIScreen = new TCBScreen();
      mainGUIScreen->driver.set( mainGUIDriver );

      mainMessageQueue = new FifoVector( 1024 );

      return true;
   }

   return false;
}

void CBAppRun( TCBWindow *mainwindow ) {
   #ifdef GUI_COCOA
      if ( mainwindow != NULL ) {
         static_cast<TCBDriverCocoa *>( mainGUIDriver )->setStartingWindow( mainwindow );
      } else {
         if ( mainGUIScreen->mainwindow != NULL ) {
            static_cast<TCBDriverCocoa *>( mainGUIDriver )->setStartingWindow( mainGUIScreen->mainwindow );
         } else {
            static_cast<TCBDriverCocoa *>( mainGUIDriver )->setStartingWindow( static_cast<TCBMagnet *>(mainGUIScreen->getVector()->elementAt(0))->getWindow() );
         }
      }
      
      mainGUIDriver->mainloop_iteration();
   #else
      while ( mainGUIScreen->visible.get() ) {
         mainGUIDriver->mainloop_iteration();

         GFMillisleep(1);
         if ( !mainMessageQueue->isEmpty() ) {
            Callable *obj = static_cast<Callable *>( mainMessageQueue->trypop() );
            if ( obj != NULL ) {
               obj->execute();
               delete mainMessageQueue->pop();
            }
         }
         GFMillisleep(1);
      }
   #endif
}

void CBAppAddEvent( Callable *event ) {
   #ifdef GUI_COCOA
      event->execute();
      delete event;
   #else
      while ( mainMessageQueue->isFull() ) {
         GFMillisleep(5);
      }
      mainMessageQueue->push( event );
   #endif
}

void CBAppFinalize() {
   delete mainMessageQueue;
   delete mainGUIScreen;

   #ifdef GUI_WIN32
      CBWin32Finalize();
   #endif

   #ifdef GUI_COCOA
      CBCocoaFinalize();
   #endif

   #ifdef GUI_GTK
      CBGTKFinalize();
   #endif

   delete mainGUIDriver;

   finiGlobalGarbageCollector();
   finiGroundfloor();
}
