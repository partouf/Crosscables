
#include "CBBase.h"
#include <cstdio>

TCBFont::TCBFont(const char *sName, long iSize) : Freeable() {
	this->iRemoteId = 0;

	this->name.internalSet(sName);
	this->size.internalSet(iSize);
}

//----------------------------------------------------------------------------

TCBKey::TCBKey( DWORD vkcode ) : Freeable() {
    this->vkcode = vkcode;
}
TCBKey::~TCBKey() {
}

//----------------------------------------------------------------------------

TCBPoint::TCBPoint( long x, long y ) : Freeable() {
   this->x = x;
   this->y = y;
}

TCBPoint::TCBPoint( TCBPoint *anotherPoint ) : Freeable() {
   this->x = anotherPoint->x;
   this->y = anotherPoint->y;
}

TCBPoint::~TCBPoint() {
}

void TCBPoint::addPoint( TCBPoint *anotherPoint ) {
   x += anotherPoint->x;
   y += anotherPoint->y;
}

void TCBPoint::clear() {
   x = 0;
   y = 0;
}

//----------------------------------------------------------------------------

TCBRect::TCBRect( long x, long y, long w, long h ) : TCBPoint( x, y ) {
   this->w = w;
   this->h = h;
}

TCBRect::TCBRect( TCBRect *anotherRect ) : TCBPoint( anotherRect ) {
   this->w = anotherRect->w;
   this->h = anotherRect->h;
}

TCBRect::~TCBRect() {
}

bool TCBRect::isPointInRect( TCBPoint *aPoint ) {
   return
   (
      ( aPoint->x >= this->x ) &&
      ( aPoint->y >= this->y ) &&
      ( aPoint->x <= (this->x + this->w) ) &&
      ( aPoint->y <= (this->y + this->h) )
   );
}

void TCBRect::clear() {
   TCBPoint::clear();

   w = 0;
   h = 0;
}

void TCBRect::copyFrom( TCBRect *anotherRect ) {
   this->x = anotherRect->x;
   this->y = anotherRect->y;
   this->w = anotherRect->w;
   this->h = anotherRect->h;
}

//----------------------------------------------------------------------------

void TCBMagnet::debug_click( TCBMagnet *sender ) {
   //printf( "debug_click(%d)\n", reinterpret_cast<unsigned long>( sender ) );
}



TCBMagnet::TCBMagnet( long x, long y, long w, long h ) : TCBRect( x, y, w, h ) {
   iRemoteId      = NULL;
   iRemoteType    = -1;
   magic          = NULL;

   parent.set( 0 );
   visible.set( true );
   enabled.set( true );

   monospacefont.set( false );

   identifier.set( "magnet" );

   caption.set( "" );
   caption.setNotify( GFCreateNotify( const char *, TCBMagnet, this, &TCBMagnet::setCaption ) );
   caption.setRetreive( GFCreateRetreive( char *, TCBMagnet, this, &TCBMagnet::getCaption ) );

   onClick.setDefaultParam( this );
   onClick += GFCreateNotify( TCBMagnet *, TCBMagnet, this, &TCBMagnet::debug_click );

   focus.internalSet(false);
   focus.setNotify( GFCreateNotify( bool, TCBMagnet, this, &TCBMagnet::setFocus ) );

   visible.setNotify( GFCreateNotify( bool, TCBMagnet, this, &TCBMagnet::setVisible ) );
   parent.setNotify( GFCreateNotify( TCBCollector *, TCBMagnet, this, &TCBMagnet::setParent ) );
}

TCBMagnet::~TCBMagnet() {
   TCBDriver *driver = getDriver();
   if ( driver != NULL ) {
      driver->destroy_magnet( this );
   }
}

void TCBMagnet::setInfo( int iInfoType, BValue *aValue ) {
   TCBDriver *driver = getDriver();
   if ( driver != NULL ) {
      driver->setinfo_magnet( this, iInfoType, aValue );
   }
}

void TCBMagnet::getInfo( int iInfoType, BValue *aValue ) {
   TCBDriver *driver = getDriver();
   if ( driver != NULL ) {
      driver->getinfo_magnet( this, iInfoType, aValue );
   }
}

void TCBMagnet::click() {
   onClick.execute( this );
}

void TCBMagnet::paint() {
   onPaint.execute( this );
}

void TCBMagnet::setVisible( bool bVisible ) {
   if ( bVisible != visible.internalGet() ) {
      visible.internalSet( bVisible );

      bool bParentVisible = true;
      TCBMagnet *p = parent.get();
      if ( p != NULL ) {
         bParentVisible = p->isActuallyVisible();
      }

      if ( bParentVisible ) {
         selfVisibilityChange();
      }
   }
}

bool TCBMagnet::isActuallyVisible() {
   TCBMagnet *p = parent.get();

   bool bVisible = visible.internalGet();
   if ( bVisible ) {
      if ( p != NULL ) {
         return p->isActuallyVisible();
      }
   }

   return bVisible;
}

void TCBMagnet::update() {
   setInfo( CB_INFOTYPE_REPOSITION, NULL );
   /*
   if ( iRemoteId != -1 ) {
      BValue *val = new BValue();
      val->setString( caption.get() );
      setInfo( CB_INFOTYPE_EDITTEXT, val );
      delete val;
   }
   */
}

void TCBMagnet::setCaption( const char *sCaption ) {
   if ( iRemoteId != NULL ) {
      BValue *val = new BValue();
      val->setString( sCaption );
      setInfo( CB_INFOTYPE_EDITTEXT, val );
      delete val;
   }

   caption.internalSet( sCaption );
}

char *TCBMagnet::getCaption() {
   BValue *val = new BValue();
   if ( iRemoteId != NULL ) {
      val->setString( caption.internalGet() );
      getInfo( CB_INFOTYPE_EDITTEXT, val );
      caption.internalSetCopy( val->asString() );
   }
   delete val;

   return caption.internalGet();
}

void TCBMagnet::setFocus( bool bFocus ) {
   if ( iRemoteId != NULL ) {
      BValue *val = new BValue();
      val->setBoolean( bFocus );
      setInfo( CB_INFOTYPE_FOCUS, val );
      delete val;
   }

   focus.internalSet( bFocus );
}

bool TCBMagnet::getFocus() {
   BValue *val = new BValue();
   if ( iRemoteId != NULL ) {
      val->setBoolean( focus.internalGet() );
      getInfo( CB_INFOTYPE_FOCUS, val );
      focus.internalSet( val->asBoolean() );
   }
   delete val;

   return focus.internalGet();
}

void TCBMagnet::parentVisibilityChange( bool bParentActuallyVisible ) {
   TCBDriver *d = getDriver();
   if ( d != NULL ) {
      if ( visible.internalGet() ) {
         if ( bParentActuallyVisible ) {
            d->show_magnet( this );
            onShow.execute( this );
         } else {
            d->hide_magnet( this );
            onHide.execute( this );
         }
      }
   }
}

void TCBMagnet::selfVisibilityChange() {
   TCBDriver *d = getDriver();
   if ( d != NULL ) {
      if ( visible.internalGet() ) {
         d->show_magnet( this );
         onShow.execute( this );
      } else {
         d->hide_magnet( this );
         onHide.execute( this );
      }
   }
}

void TCBMagnet::updateToAbsolutePosition( TCBPoint *aPoint ) {
   // terugwaards zoek algorithme voor het bepalen van de absolute positie van dit magnet
   aPoint->addPoint( this );

   TCBMagnet *p = parent.get();
   if ( p != NULL ) {
      p->updateToAbsolutePosition( aPoint );
   }
}

TCBMagnet *TCBMagnet::getMagnetAtPoint( TCBPoint *aBasePoint, TCBPoint *aPoint ) {
   // we zijn geen collector, we kunnen niet verder kijken
   return NULL;
}

void TCBMagnet::setParent( TCBCollector *aParent ) {
   TCBCollector *oldParent = this->parent.internalGet();
   if ( oldParent != NULL ) {
      oldParent->removeMagnet( this );
   }

   if ( aParent != NULL ) {
      aParent->addMagnet( this );
   }
}

TCBScreen *TCBMagnet::getScreen() {
   TCBMagnet *p = parent.get();

   if ( p != NULL ) {
      return p->getScreen();
   } else {
      return NULL;
   }
}

TCBDriver *TCBMagnet::getDriver() {
   TCBScreen *scr = getScreen();

   if ( scr != NULL ) {
      return scr->getDriver();
   }

   return NULL;
}

TCBMagnet *TCBMagnet::getWindow() {
   TCBMagnet *p = parent.get();

   if ( p != NULL ) {
      return p->getWindow();
   } else {
      return NULL;
   }
}

//----------------------------------------------------------------------------


TCBCollector::TCBCollector( int x, int y, int w, int h ) : TCBMagnet( x, y, w, h ) {
   aMagnetList = new Vector();
   aMagnetList->autoClear = true;
}

TCBCollector::~TCBCollector() {
   delete aMagnetList;
}

void TCBCollector::addMagnet( TCBMagnet *aMagnet ) {
   TCBCollector *p = aMagnet->parent.internalGet();
   if ( p != 0 ) {
      p->removeMagnet( p );
   }

   aMagnet->parent.internalSet( this );

   aMagnetList->addElement( aMagnet );
}

void TCBCollector::removeMagnet( TCBMagnet *aMagnet ) {
   aMagnetList->removeElement( aMagnet );

   aMagnet->parent.internalSet( 0 );
}

Vector *TCBCollector::getVector() {
   return aMagnetList;
}

TCBMagnet *TCBCollector::getMagnetAtPoint( TCBPoint *aBasePoint, TCBPoint *aPoint ) {
   TCBMagnet *magnet = 0;

   // voorwaards zoek algorithme, waardoor het sneller is om de x/y telkens bij elkaar op te tellen,
   // dan updateToAbsolutePosition() te gebruiken...
   TCBRect *absoluteRect = new TCBRect();

   unsigned int c = aMagnetList->size();
   for ( unsigned int i = 0; i < c; i++ ) {
      TCBMagnet *tmpMagnet = static_cast<TCBMagnet *>( aMagnetList->elementAt( i ) );
      if ( tmpMagnet != 0 ) {

         // indien visible
         if ( tmpMagnet->visible.get() ) {

            // kijk of punt binnen het rect ligt
            absoluteRect->clear();
            absoluteRect->addPoint( aBasePoint );
            absoluteRect->addPoint( tmpMagnet );
            absoluteRect->w = tmpMagnet->w;
            absoluteRect->h = tmpMagnet->h;
            if ( absoluteRect->isPointInRect( aPoint ) ) {

               // misschien moeten we verder kijken
               magnet = getMagnetAtPoint( absoluteRect, aPoint );
               if ( magnet == 0 ) {
                  magnet = tmpMagnet;
               }

               // gevonden
               break;

            }

         }

      }
   }

   delete absoluteRect;

   return magnet;
}



/*

      unsigned int c = aMagnetList->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBMagnet *tmpMagnet = static_cast<TCBMagnet *>( aMagnetList->elementAt( i ) );
         if ( tmpMagnet != 0 ) {
            tmpMagnet->notifyStateToDriver();
         }
      }

*/

void TCBCollector::parentVisibilityChange( bool bParentActuallyVisible ) {
   TCBDriver *d = getDriver();
   if ( d != NULL ) {
      if ( visible.internalGet() ) {
         if ( bParentActuallyVisible ) {
            d->show_magnet( this );
         } else {
            d->hide_magnet( this );
         }

         unsigned int c = aMagnetList->size();
         for ( unsigned int i = 0; i < c; i++ ) {
            TCBMagnet *tmpMagnet = static_cast<TCBMagnet *>( aMagnetList->elementAt( i ) );
            if ( tmpMagnet != 0 ) {
               tmpMagnet->parentVisibilityChange( bParentActuallyVisible );
            }
         }

         if ( bParentActuallyVisible ) {
            onShow.execute( this );
         } else {
            onHide.execute( this );
         }
      }
   }
}

void TCBCollector::selfVisibilityChange() {
   TCBDriver *d = getDriver();
   if ( d != NULL ) {
      bool bVisible = visible.internalGet();
      if ( bVisible ) {
         d->show_magnet( this );
      } else {
         d->hide_magnet( this );
      }

      unsigned int c = aMagnetList->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBMagnet *tmpMagnet = static_cast<TCBMagnet *>( aMagnetList->elementAt( i ) );
         if ( tmpMagnet != 0 ) {
            tmpMagnet->parentVisibilityChange( bVisible );
         }
      }

      if ( bVisible ) {
         onShow.execute( this );
      } else {
         onHide.execute( this );
      }
   }
}

TCBMagnet *TCBCollector::getMagnetByIdentifier( const char *sIdentifier, bool bRecursive ) {
   unsigned int l = strlen( sIdentifier );
   unsigned int c = aMagnetList->size();
   for ( unsigned int i = 0; i < c; i++ ) {
      TCBMagnet *tmpMagnet = static_cast<TCBMagnet *>( aMagnetList->elementAt( i ) );
		if ( tmpMagnet->identifier.link()->match( sIdentifier, l ) ) {
			 return tmpMagnet;
		}
		if ( (bRecursive) && ( tmpMagnet->iRemoteType >= 100 ) ) {
			tmpMagnet = static_cast<TCBCollector *>( tmpMagnet )->getMagnetByIdentifier( sIdentifier );
			if ( tmpMagnet != NULL ) {
				return tmpMagnet;
			}
		}
   }

   return NULL;
}


//----------------------------------------------------------------------------


TCBScreen::TCBScreen() : TCBCollector() {
   iRemoteType    = CB_TYPE_SCREEN;

   mainwindow     = 0;

   iScreenWidth   = -1;
   iScreenHeight  = -1;

   visible.internalSet( false );

   driver.internalSet( 0 );
   driver.setNotify( GFCreateNotify( TCBDriver *, TCBScreen, this, &TCBScreen::setDriver ) );
}

TCBScreen::~TCBScreen() {
}

TCBScreen *TCBScreen::getScreen() {
   return this;
}

void TCBScreen::setDriver( TCBDriver *aDriver ) {
   TCBDriver *d = driver.internalGet();
   if ( d != 0 ) {
      d->finalize();
   }

   driver.internalSet( aDriver );
   aDriver->initialize( this );
}

TCBDriver *TCBScreen::getDriver() {
   return driver.get();
}
