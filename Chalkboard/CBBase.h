
#ifndef __CBBase_H
#define __CBBase_H

#include <Groundfloor/Atoms/Freeable.h>
#include <Groundfloor/Molecules/String.h>
#include <Groundfloor/Molecules/Vector.h>

#include <Groundfloor/Molecules/Property.h>
#include <Groundfloor/Molecules/Callbacks.h>

// support classes
class TCBFont;
class TCBColor;

// base classes for chalkboard
class TCBPoint;
class TCBRect;
class TCBMagnet;
class TCBCollector;

class TCBScreen;


using namespace Groundfloor;

#define CBMagnetEvent NotifyAbstract<TCBMagnet *>
#define CBKeyEvent NotifyAbstract<TCBKey *>
#define CBMagnetEventDispatcher MultiNotify<TCBMagnet *>
#define CBMagnetKeyEventDispatcher MultiNotify<TCBKey *>

#include "CBDriver.h"

//-----------------------------------------------------------------------------

// support classes
class TCBFont: public Freeable {
	protected:
	public:
		Property<int> size;
		StringProperty name;

		TCBFont(const char *sName, long iSize);

		void *iRemoteId;
};

class TCBColor: public Freeable {
   public:
      char r;
      char g;
      char b;
      char a;
};

class TCBKey: public Freeable {
    public:
        TCBKey( DWORD vkcode = 0 );
        ~TCBKey();

        DWORD vkcode;
};

//
class TCBPoint: public Freeable {
   public:
      TCBPoint( long x = 0, long y = 0 );
      TCBPoint( TCBPoint *anotherPoint );
      ~TCBPoint();

      virtual void addPoint( TCBPoint *anotherPoint );
      virtual void clear();

      long x;
      long y;
};

class TCBRect: public TCBPoint {
   public:
      TCBRect( long x = 0, long y = 0, long w = 0, long h = 0 );
      TCBRect( TCBRect *anotherRect );
      ~TCBRect();

      bool isPointInRect( TCBPoint *aPoint );
      void clear();

	  virtual void copyFrom( TCBRect *anotherRect );

      long w;
      long h;
};

/*
- driver
- visible, parent
- enable, disable
- caption
- font
- foregroundcolor, backgroundcolor
- onhide, onshow, onpaint, onclick (mouse&key), onmousedown, onmouseup, onkeydown, onkeyup, onkeypress
*/
class TCBMagnet: public TCBRect {
   protected:
      virtual void setInfo( int iInfoType, BValue *aValue );
      virtual void getInfo( int iInfoType, BValue *aValue );
   public:
      TCBMagnet( long x = 0, long y = 0, long w = 0, long h = 0 );
      ~TCBMagnet();

      void *iRemoteId;
      int iRemoteType;
      void *magic;

      StringProperty caption;
      StringProperty identifier;

      void setCaption( const char *sCaption );
      char *getCaption();

      void setFocus( bool bFocus );
      bool getFocus();

      // properties
      Property<TCBCollector *> parent;
      Property<TCBFont *> font;
      Property<TCBColor *> foregroundcolor;
      Property<TCBColor *> backgroundcolor;

      Property<bool> monospacefont;

      Property<bool> visible;
      Property<bool> enabled;
      Property<bool> focus;

      // events
      CBMagnetEventDispatcher onHide;
      CBMagnetEventDispatcher onShow;
      CBMagnetEventDispatcher onPaint;

      CBMagnetEventDispatcher onFocus;

      CBMagnetEventDispatcher onChange;

      CBMagnetEventDispatcher onClick;
      CBMagnetEventDispatcher onDoubleClick;
      CBMagnetEventDispatcher onContextMenu;
/*
      MultiNotify<TCBMagnet *> onMouseDown;
      MultiNotify<TCBMagnet *> onMouseUp;
      MultiNotify<TCBMagnet *> onMousePress;

      MultiNotify<TCBMagnet *> onKeyPress;
*/
      CBMagnetKeyEventDispatcher onKeyDown;
      CBMagnetKeyEventDispatcher onKeyUp;

      // control functions
      virtual void click();
      virtual void paint();

      // control / callbacks
      void setVisible( bool bVisible );
      void setParent( TCBCollector *aParent );

      // check functions
      virtual bool isActuallyVisible();

      // test functions / callbacks
      virtual void debug_click( TCBMagnet *sender );

      virtual void updateToAbsolutePosition( TCBPoint *aPoint );
      virtual TCBMagnet *getMagnetAtPoint( TCBPoint *aBasePoint, TCBPoint *aPoint );

      // gets
      virtual TCBScreen *getScreen();
      virtual TCBDriver *getDriver();
      virtual TCBMagnet *getWindow();

      virtual void update();
      virtual void parentVisibilityChange( bool bParentActuallyVisible );
      virtual void selfVisibilityChange();
};


/*
TCBCollector
- magnets
- addmagnet, removemagnet
- onresize
*/

class TCBCollector: public TCBMagnet
{
   protected:
      Vector *aMagnetList;
   public:
      TCBCollector( int x = 0, int y = 0, int w = 0, int h = 0 );
      ~TCBCollector();

		CBMagnetEventDispatcher onMove;
      CBMagnetEventDispatcher onResize;

      void addMagnet( TCBMagnet *aMagnet );
      void removeMagnet( TCBMagnet *aMagnet );

      Vector *getVector();

      TCBMagnet *getMagnetAtPoint( TCBPoint *aBasePoint, TCBPoint *aPoint );

      void parentVisibilityChange( bool bParentActuallyVisible );
      void selfVisibilityChange();

      TCBMagnet *getMagnetByIdentifier( const char *sIdentifier, bool bRecursive = false );
};

//----------------------------------------------------------------------------

class TCBScreen: public TCBCollector {
   protected:
   public:
      TCBScreen();
      ~TCBScreen();

      TCBMagnet *mainwindow;
      Property<TCBDriver *> driver;
      TCBPoint mousepointer;

      int iScreenWidth;
      int iScreenHeight;

      void setDriver( TCBDriver *aDriver );

      TCBScreen *getScreen();
      TCBDriver *getDriver();
};



#endif // __CBBase_H

