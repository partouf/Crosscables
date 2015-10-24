

#include "CBDriverCocoa.h"

#ifdef GF_OS_MAC

#include "CocoaIntf.h"

#ifndef max
   #define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif


TCBDriverCocoa *CBGlobalDriver = NULL;

//------------------------------------------------------------------------------
void stretchChildren( TCBMagnet *aMagnet );
void stretchMagnet( TCBMagnet *aMagnet );

void stretchMagnet( TCBMagnet *aMagnet ) {
   if ( aMagnet != NULL ) {
      unsigned long obj = reinterpret_cast<unsigned long>( aMagnet->iRemoteId );
      if ( obj != 0 ) {
         if ( aMagnet->iRemoteType != CB_TYPE_MENU ) {
            if ( ( aMagnet->w <= 0 ) || ( aMagnet->h <= 0 ) ) {
               TCBCollector *parent = aMagnet->parent.get();
               if ( parent != NULL ) {
                  unsigned long widParent = reinterpret_cast<unsigned long>( parent->iRemoteId );
                  if ( widParent != 0 ) {
                     long cocoawidth = 0;
                     long cocoaheight = 0;

                     if ( parent->magic != NULL ) {
                        TCBPoint *pt = reinterpret_cast<TCBPoint *>( parent->magic );

                        cocoawidth = pt->x;
                        cocoaheight = pt->y;
                     } else {
                        if ( parent->iRemoteType != CB_TYPE_NOTEBOOK ) {
                           getSize_NSView( widParent, &cocoawidth, &cocoaheight );
                        }
                     }

                     int width = aMagnet->w;
                     int height = aMagnet->h;

                     if ( width <= 0 ) {
                        width = cocoawidth - aMagnet->x + width;
                     }
                     if ( height <= 0 ) {
                        height = cocoaheight - aMagnet->y + height;
                     }

                     if ( aMagnet->iRemoteType == CB_TYPE_MULTILINE ) {
                        reposition_CBCocoaTextView( obj, widParent, aMagnet->x, aMagnet->y, width, height );
                     } else if (aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
                        reposition_CBCocoaTable( obj, widParent, aMagnet->x, aMagnet->y, width, height );
                     } else if ( (aMagnet->iRemoteType == CB_TYPE_GRID) || (aMagnet->iRemoteType == CB_TYPE_TREE ) ){
                        reposition_CBCocoaTable( obj, widParent, aMagnet->x, aMagnet->y, width, height );
                     } else if ( aMagnet->iRemoteType == CB_TYPE_NOTEBOOKTAB ) {
                     } else {
                        reposition_NSView( obj, widParent, aMagnet->x, aMagnet->y, width, height );
                     }

                     stretchChildren( aMagnet );
                  }
               }
            }
         }
      }
   }
}

void stretchChildren( TCBMagnet *aMagnet ) {
   if ( aMagnet != NULL ) {
      if ( aMagnet->iRemoteType >= 100 ) {
         TCBCollector *col = static_cast<TCBCollector *>( aMagnet );
         Vector *controls = col->getVector();
         unsigned int c = controls->size();
         for ( unsigned int i = 0; i < c; i++ ) {
            stretchMagnet( static_cast<TCBMagnet *>( controls->elementAt(i) ) );
         }
      }
   }
}

//------------------------------------------------------------------------------

TCBMagnet *getMagnetByRemoteId( TCBCollector *obj, void *id ) {
   Vector *list = obj->getVector();
   unsigned int c = list->size();
   for ( unsigned int i = 0; i < c; i++ ) {
      TCBMagnet *magnet = static_cast<TCBMagnet *>( list->elementAt(i) );
      if ( magnet->iRemoteId == id ) {
         return magnet;
      }

      // if collector -> recursie...
      if ( magnet->iRemoteType > 100 ) {
         magnet = getMagnetByRemoteId( static_cast<TCBCollector *>(magnet), id );
         if ( magnet != NULL ) {
            return magnet;
         }
      }
   }

   return NULL;
}

TCBCocoaWindow *TCBDriverCocoa::getWindowObject( TCBMagnet *aMagnet ) {
   TCBCocoaWindow *window = NULL;

   unsigned int c = windows->size();
   for ( unsigned int i = 0; i < c; i++ ) {
      TCBCocoaWindow *tmp = static_cast<TCBCocoaWindow *>( windows->elementAt(i) );
      if ( tmp->magnet == aMagnet ) {
         window = tmp;

         break;
      }
   }

   return window;
}

//------------------------------------------------------------------------

bool cocoaClickEvent( unsigned long sender, unsigned long customid ) {
   TCBMagnet *magnet = NULL;
   if ( customid != 0 ) {
      printf( "customid execution\n" );
      magnet = reinterpret_cast<TCBMagnet *>( customid );
      magnet->onClick.execute( magnet );
   } else {
      unsigned int c = CBGlobalDriver->windows->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBCocoaWindow *tmp = static_cast<TCBCocoaWindow *>( CBGlobalDriver->windows->elementAt(i) );
         if ( tmp != NULL ) {
            TCBMagnet *magnet = getMagnetByRemoteId( reinterpret_cast<TCBCollector *>( tmp->magnet ), reinterpret_cast<void *>( sender ) );
            if ( magnet != NULL ) {
               magnet->onClick.execute( magnet );
               break;
            }
         }
      }
   }

   return true;
}

bool cocoaCloseEvent( unsigned long sender, unsigned long customid ) {
   TCBMagnet *magnet = NULL;
   if ( customid != 0 ) {
      magnet = reinterpret_cast<TCBMagnet *>( customid );
      magnet->onHide.execute( magnet );
   } else {
      unsigned int c = CBGlobalDriver->windows->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBCocoaWindow *tmp = static_cast<TCBCocoaWindow *>( CBGlobalDriver->windows->elementAt(i) );
         if ( tmp != NULL ) {
            if ( tmp->pointer == sender ) {
               tmp->magnet->onHide.execute( tmp->magnet );
               break;
            }
         }
      }
   }

   return true;
}

bool cocoaWindowResizeEvent( unsigned long sender, unsigned long customid, long newwidth, long newheight ) {
   TCBMagnet *magnet = NULL;
   if ( customid != 0 ) {
      magnet = reinterpret_cast<TCBMagnet *>( customid );

      // resize is nog niet toegepast voor getSize_NSView(), dus moeten we het zelf onthouden
      TCBPoint *pt = new TCBPoint( newwidth, newheight );
      magnet->magic = reinterpret_cast<void *>( pt );

      stretchChildren( magnet );

      magnet->magic = NULL;
      delete pt;
   }

   return true;
}

bool cocoaMenuClickEvent( unsigned long sender, unsigned long customid ) {
   TCBMagnet *magnet = NULL;
   if ( customid != 0 ) {
      magnet = reinterpret_cast<TCBMagnet *>( customid );
      magnet->onClick.execute( magnet );
   } else {
      return cocoaClickEvent( sender, customid );
   }

   return true;
}

bool cocoaGridIsEditable( unsigned long customid, int column, long row) {
   return false;
}

long cocoaGridNumberOfRows( unsigned long customid ) {
   TCBGrid *control = reinterpret_cast<TCBGrid *>(customid);
   BTable *table = control->data.get();
   if ( table != NULL ) {
      BRecords *records = table->records.get();
      if ( records != NULL ) {
         return records->size();
      }
   }

   return 0;
}

char *cocoaGridGetValue( unsigned long customid, int column, long row ) {
   TCBGrid *control = reinterpret_cast<TCBGrid *>(customid);
   TCBGridHeader *header = control->headers.getHeader( column );

   BTable *table = control->data.get();
   if ( table != NULL ) {
      BRecords *records = table->records.get();
      if ( records != NULL ) {
         BRecord *record = records->getRecord( row );
         BValue *val = record->getValue( header->column.get() );
         return val->asString()->getValue();
      }
   }

   return "";
}

void cocoaGridSetValue( unsigned long customid, int column, long row, const char *value ) {
   TCBGrid *control = reinterpret_cast<TCBGrid *>(customid);
   TCBGridHeader *header = control->headers.getHeader( column );

   BTable *table = control->data.get();
   if ( table != NULL ) {
      BRecords *records = table->records.get();
      if ( records != NULL ) {
         BRecord *record = records->getRecord( row );
         BValue *val = record->getValue( header->column.get() );
         val->setString( value );
      }
   }
}

void updateCocoaGridHeaders( TCBGrid *aMagnet ) {
   unsigned long pointer = reinterpret_cast<unsigned long>( aMagnet->iRemoteId );

   unsigned int c = aMagnet->headers.size();

   setTableViewColumnCount( pointer, c );

   for ( unsigned int i = 0; i < c; i++ ) {
      TCBGridHeader *header = aMagnet->headers.getHeader( i );
      setTableViewColumnTitle( pointer, i, header->caption.get() );
      setTableViewColumnWidth( pointer, i, header->width.get() );
   }
}

//-----------------------------------------------
bool cocoaListboxIsEditable( unsigned long customid, int column, long row) {
   return false;
}

long cocoaListboxNumberOfRows( unsigned long customid ) {
   TCBListControl *control = reinterpret_cast<TCBListControl *>(customid);
   return control->count();
}

char *cocoaListboxGetValue( unsigned long customid, int column, long row ) {
   TCBListControl *control = reinterpret_cast<TCBListControl *>(customid);

   TCBListControlItem *item = control->getItem( row );

   if ( item != NULL ) {
      return item->value.asString()->getValue();

   }

   return "";
}

void cocoaListboxSetValue( unsigned long customid, int column, long row, const char *value ) {
}

//-----------------------------------------------

long cocoaTreeGetChildCount( unsigned long customid, unsigned long itemid ) {
   if ( itemid != 0 ) {
      BNode *node = reinterpret_cast<BNode *>( itemid );
      return node->childCount();
   } else {
      TCBTree *tree = reinterpret_cast<TCBTree *>( customid );
      BNode *node = tree->data.internalGet();
      if ( node != NULL ) {
         return 1;
      } else {
         return 0;
      }
   }
}

unsigned long cocoaTreeGetChildId( unsigned long customid, unsigned long parentitemid, long index ) {
   if ( parentitemid != 0 ) {
      BNode *node = reinterpret_cast<BNode *>( parentitemid );

      return reinterpret_cast<unsigned long>( node->getChild( index ) );
   } else {
      TCBTree *tree = reinterpret_cast<TCBTree *>( customid );
      return reinterpret_cast<unsigned long>( tree->data.internalGet() );
   }
}

char *cocoaTreeGetValue( unsigned long customid, unsigned long itemid ) {
   if ( itemid != 0 ) {
      BNode *node = reinterpret_cast<BNode *>( itemid );
      return node->name.getValue();
   } else {
      TCBTree *tree = reinterpret_cast<TCBTree *>( customid );
      BNode *node = tree->data.internalGet();
      if ( node != NULL ) {
         return node->name.getValue();
      } else {
         return "";
      }
   }
}

void cocoaTreeSetCustomValue( unsigned long customid, unsigned long itemid, unsigned long value ) {
   if ( itemid != 0 ) {
      BNode *node = reinterpret_cast<BNode *>( itemid );
      BAttribute *attrib = node->findOrCreateAttribute( "__cocoacustomvalue" );
      attrib->value.setInteger( value );
      attrib->hideFromXML = true;
   }
}

unsigned long cocoaTreeGetCustomValue( unsigned long customid, unsigned long itemid ) {
   if ( itemid != 0 ) {
      BNode *node = reinterpret_cast<BNode *>( itemid );
      BAttribute *attrib = node->getAttributeByName( "__cocoacustomvalue" );
      if ( attrib != NULL ) {
         return attrib->value.asInteger();
      }
   }

   return 0;
}


//-----------------------------------------------

void CBCocoaInitialize( TCBDriverCocoa *pDriver ) {
   CBGlobalDriver = pDriver;

   initCocoa();
   setCocoaClickEventMethod( cocoaClickEvent );
   setCocoaCloseEventMethod( cocoaCloseEvent );
   setCocoaMenuClickEventMethod( cocoaMenuClickEvent );
   setCocoaWindowResizeEventMethod( cocoaWindowResizeEvent );
}

void CBCocoaFinalize() {
   CBGlobalDriver = NULL;
   finiCocoa();
}

TCBDriverCocoa::TCBDriverCocoa() {
   this->windows  = new Vector();
}

TCBDriverCocoa::~TCBDriverCocoa() {
   delete windows;
}

void TCBDriverCocoa::initialize( TCBScreen *aScreen ) {
   TCBDriver::initialize( aScreen );

   pDefaultFont = createFont( "", 12 );
   pMonospaceFont = createFont( "Courier", 12 );
}

void TCBDriverCocoa::finalize() {
   TCBDriver::finalize();
}

//-----------------------------------------------

void TCBDriverCocoa::preparefont(TCBFont *font) {
   // todo
}

void TCBDriverCocoa::refreshItemsCombobox( TCBListControl *aMagnet ) {
   if ( aMagnet->iRemoteId != NULL ) {
      unsigned long pointer = reinterpret_cast<unsigned long>( aMagnet->iRemoteId );

      clearItems_NSPopUpButton( pointer );

      unsigned int c = aMagnet->count();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBListControlItem *item = aMagnet->getItem( i );

         addItem_NSPopUpButton( pointer, item->value.asString()->getValue() );
      }

      aMagnet->selected.set( aMagnet->selected.internalGet() );
   }
}

//-----------------------------------------------

void TCBDriverCocoa::addMenuItem( TCBMenu *aParentMenu, TCBMenu *aMenu ) {
   String sCaption;
   sCaption.setValue( aMenu->caption.link() );

   unsigned long iParentMenu = reinterpret_cast<unsigned long>( aParentMenu->magic );

   if ( sCaption.match_ansi( "-" ) ) {
      aMenu->iRemoteId = reinterpret_cast<void *>( addMenuItemSeparator( iParentMenu ) );
   } else {
      Vector *list = aMenu->getVector();
      unsigned int c = list->size();

      unsigned long iMenuItem = ::addMenuItem( aMenu->caption.get(), iParentMenu, aMenu->controlkey.get() );
      unsigned long iMenu;

      aMenu->iRemoteId = reinterpret_cast<void *>( iMenuItem );

      if ( c > 0 ) {
         iMenu = newMenu( aMenu->caption.get() );
         linkSubMenu( iParentMenu, iMenuItem, iMenu );

         aMenu->magic = reinterpret_cast<void *>( iMenu );
      }

      for ( unsigned int i = 0; i < c; i++ ) {
         addMenuItem( aMenu, static_cast<TCBMenu *>( list->elementAt(i) ) );
      }
   }
}

void TCBDriverCocoa::buildMenu( bool popup, TCBMenu *aMenu ) {
   if ( aMenu != NULL ) {
      unsigned long iMenu;
      if ( popup ) {
         iMenu = newMenu( "" );
      } else {
         iMenu = newMenu( "" );
      }

      aMenu->iRemoteId = reinterpret_cast<void *>( iMenu );
      aMenu->magic = reinterpret_cast<void *>( iMenu );

      Vector *list = aMenu->getVector();
      unsigned int c = list->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         addMenuItem( aMenu, static_cast<TCBMenu *>( list->elementAt(i) ) );
      }
   }
}

//-----------------------------------------------


void TCBDriverCocoa::create_magnet( TCBMagnet *aMagnet ) {
   unsigned long pointer = 0;
   TCBCocoaWindow *w;
   TCBMenu *menu;

   int width = max( 10, aMagnet->w );
   int height = max( 10, aMagnet->h );
   long cocoawidth = 0;
   long cocoaheight = 0;

   int iNewX;
   int iNewY;

   unsigned long parent = 0;
   if ( aMagnet->parent.get() != NULL ) {
      parent = reinterpret_cast<unsigned long>( aMagnet->parent.get()->iRemoteId );

      if ( parent != 0 ) {
         if ( aMagnet->parent.get()->iRemoteType != CB_TYPE_NOTEBOOK ) {
            getSize_NSView( parent, &cocoawidth, &cocoaheight );
         }

         width = aMagnet->w;
         height = aMagnet->h;

         if ( width <= 0 ) {
            width = cocoawidth - aMagnet->x + width;
         }
         if ( height <= 0 ) {
            height = cocoaheight - aMagnet->y + height;
         }
      }
   }

   switch ( aMagnet->iRemoteType ) {
      case CB_TYPE_SCREEN:
         getSize_NSView( parent, &cocoawidth, &cocoaheight );
         aMagnet->w = cocoawidth;
         aMagnet->h = cocoaheight;

         break;
      case CB_TYPE_WINDOW:
         iNewX = aMagnet->x;
         iNewY = aMagnet->y;

         if ( iNewX == CB_LOCATION_CENTEROFSCREEN ) {
            iNewX = aMagnet->getScreen()->w / 2 - width / 2;
         }
         if ( iNewY == CB_LOCATION_CENTEROFSCREEN ) {
            iNewY = aMagnet->getScreen()->h / 2 - height / 2;
         }

         pointer = addWindow( iNewX, iNewY, width, height, aMagnet->caption.get(), 0, reinterpret_cast<unsigned long>(aMagnet) );

         w = new TCBCocoaWindow();
         w->magnet = static_cast<TCBCollector *>( aMagnet );
         w->pointer = pointer;
         windows->addElement( w );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_GROUPBOX:
         pointer = addBox( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         w = new TCBCocoaWindow();
         w->magnet = static_cast<TCBCollector *>( aMagnet );
         w->pointer = pointer;
         windows->addElement( w );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_PANEL:
         pointer = addPanel( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         w = new TCBCocoaWindow();
         w->magnet = static_cast<TCBCollector *>( aMagnet );
         w->pointer = pointer;
         windows->addElement( w );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_LABEL:
         pointer = addTextFieldLabel( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_EDITBOX:
         pointer = addTextField( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_MULTILINE:
         pointer = addTextView( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         if ( aMagnet->monospacefont.get() ) {
            setFont_NSText( pointer, pMonospaceFont );
         }

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_LISTBOX:
         pointer = addListbox( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         menu = static_cast<TCBListbox *>(aMagnet)->contextmenu.internalGet();
         if ( menu != NULL ) {
            if ( menu->iRemoteId == 0 ) {
               buildMenu( true, static_cast<TCBMenu *>( menu ) );
            }

            setContextMenu_NSTableView( pointer, reinterpret_cast<unsigned long>( menu->iRemoteId ) );
         } else {
            setContextMenu_NSTableView( pointer, 0 );
         }

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_COMBOBOX:
         pointer = addCombobox( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent, reinterpret_cast<unsigned long>(aMagnet) );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_BUTTON:
         pointer = addButton( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent, reinterpret_cast<unsigned long>(aMagnet) );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_CHECKBOX:
         pointer = addCheckbox( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent, reinterpret_cast<unsigned long>(aMagnet) );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_RADIOBUTTON:
         pointer = addRadioButton( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent, reinterpret_cast<unsigned long>(aMagnet) );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_PROGRESSBAR:
         pointer = addProgressIndicator( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_GRID:
         pointer = addTableView( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         menu = static_cast<TCBGrid *>(aMagnet)->contextmenu.internalGet();
         if ( menu != NULL ) {
            if ( menu->iRemoteId == 0 ) {
               buildMenu( true, static_cast<TCBMenu *>( menu ) );
            }

            setContextMenu_NSTableView( pointer, reinterpret_cast<unsigned long>( menu->iRemoteId ) );
         } else {
            setContextMenu_NSTableView( pointer, 0 );
         }

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_MENU:
         break;
      case CB_TYPE_NOTEBOOK:
         pointer = addTabView( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent, reinterpret_cast<unsigned long>(aMagnet) );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_NOTEBOOKTAB:
         pointer = addTabViewItem( -1, aMagnet->caption.get(), parent, reinterpret_cast<unsigned long>(aMagnet) );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      case CB_TYPE_TREE:
         pointer = addTreeView( aMagnet->x, aMagnet->y, width, height, aMagnet->caption.get(), parent );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
      default:
         pointer = addTextFieldLabel( aMagnet->x, aMagnet->y, width, height, "Unknown control", parent );

         aMagnet->iRemoteId = reinterpret_cast<void *>( pointer );
         break;
   }

   if ( pointer != 0 ) {
      if ( aMagnet->iRemoteType != CB_TYPE_WINDOW ) {
         stretchMagnet( aMagnet );
      }
   }
}

void TCBDriverCocoa::hide_magnet( TCBMagnet *aMagnet ) {
   if ( aMagnet->iRemoteId == NULL ) {
      create_magnet( aMagnet );
   }

   if ( aMagnet->iRemoteType == CB_TYPE_SCREEN ) {
      stopCocoa();
   }

   if ( aMagnet->iRemoteId != NULL ) {
      unsigned long pointer = reinterpret_cast<unsigned long>( aMagnet->iRemoteId );

      switch ( aMagnet->iRemoteType ) {
         case CB_TYPE_WINDOW:
            closeWindow( pointer );
            break;
         default:
            break;
      }
   }
}

void TCBDriverCocoa::show_magnet( TCBMagnet *aMagnet ) {
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   if ( aMagnet->iRemoteId == NULL ) {
      create_magnet( aMagnet );
   }

   if ( aMagnet->iRemoteId != NULL ) {
      unsigned long pointer = reinterpret_cast<unsigned long>( aMagnet->iRemoteId );
      TCBGrid *grid;
      TCBWindow *win;

      switch ( aMagnet->iRemoteType ) {
         case CB_TYPE_WINDOW:
            showWindow( pointer );

            win = static_cast<TCBWindow *>( aMagnet );
            if ( win->mainMenu != NULL ) {
               if ( win->mainMenu->iRemoteId == NULL ) {
                  buildMenu( false, win->mainMenu );
               }

               setApplicationMenuBar( reinterpret_cast<unsigned long>( win->mainMenu->iRemoteId ) );
            }
            break;
         case CB_TYPE_LISTBOX:
            setupTableViewDataSource( pointer, reinterpret_cast<unsigned long>(aMagnet), 1, cocoaListboxIsEditable, cocoaListboxNumberOfRows, cocoaListboxGetValue, cocoaListboxSetValue );
            break;
         case CB_TYPE_COMBOBOX:
            refreshItemsCombobox( static_cast<TCBListControl *>( aMagnet ) );
            break;
         case CB_TYPE_GRID:
            grid = reinterpret_cast<TCBGrid *>( aMagnet );
            setupTableViewDataSource( pointer, reinterpret_cast<unsigned long>(aMagnet), grid->headers.size(), cocoaGridIsEditable, cocoaGridNumberOfRows, cocoaGridGetValue, cocoaGridSetValue );
            updateCocoaGridHeaders( grid );
            break;
         case CB_TYPE_TREE:
            setupTreeDataSource( pointer, reinterpret_cast<unsigned long>(aMagnet), cocoaTreeGetChildCount, cocoaTreeGetChildId, cocoaTreeGetValue, cocoaTreeGetCustomValue, cocoaTreeSetCustomValue );
            break;
      }
   }

   lGeneralAccess.unlock();
}

void TCBDriverCocoa::destroy_magnet( TCBMagnet *aMagnet ) {
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   lGeneralAccess.unlock();
}

void TCBDriverCocoa::setinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aSourceValue ) {
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   TCBListControlItem *item;
   TCBGrid *grid;
   TCBMenu *menu;

   if ( aMagnet->iRemoteId != NULL ) {
      unsigned long pointer = reinterpret_cast<unsigned long>( aMagnet->iRemoteId );

      switch ( iInfoType ) {
         case CB_INFOTYPE_FOCUS:
            break;
         case CB_INFOTYPE_CUTTEXT:
            cut_NSText( pointer );
            break;
         case CB_INFOTYPE_COPYTEXT:
            copy_NSText( pointer );
            break;
         case CB_INFOTYPE_PASTETEXT:
            paste_NSText( pointer );
            break;
         case CB_INFOTYPE_SELECTALLTEXT:
            selectAll_NSText( pointer );
            break;
         case CB_INFOTYPE_EDITTEXT:
            if ( aMagnet->iRemoteType == CB_TYPE_BUTTON ) {
               setText_NSButton( pointer, aSourceValue->asString()->getValue() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_RADIOBUTTON) {
               setText_NSButton( pointer, aSourceValue->asString()->getValue() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_CHECKBOX) {
               setText_NSButton( pointer, aSourceValue->asString()->getValue() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_EDITBOX ) {
               setText_NSControl( pointer, aSourceValue->asString()->getValue() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_LABEL ) {
               setText_NSControl( pointer, aSourceValue->asString()->getValue() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_MULTILINE ) {
               setText_NSTextView( pointer, aSourceValue->asString()->getValue() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_PROGRESSBAR ) {
            } else if ( aMagnet->iRemoteType == CB_TYPE_GROUPBOX) {
               setTitle_Container( pointer, aSourceValue->asString()->getValue() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_WINDOW ) {
               setTitle_Container( pointer, aSourceValue->asString()->getValue() );
            }
            break;
         case CB_INFOTYPE_LISTITEMSEL:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               setSelected_NSTableView( pointer, aSourceValue->asInteger() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               setSelected_NSPopUpButton( pointer, aSourceValue->asInteger() );
            }
            break;
         case CB_INFOTYPE_BARPOSITION:
            setProgressIndicatorValue( pointer, aSourceValue->asDouble() );
            break;
         case CB_INFOTYPE_LISTITEMADD:
            item = static_cast<TCBListControl *>( aMagnet )->getItem( aSourceValue->asInteger() );
            if ( item != NULL ) {
               if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
//                  signalTableViewRowUpdate( pointer, aSourceValue->asInteger() );
                  signalTableViewRowCountChanged( pointer, aSourceValue->asInteger(), static_cast<TCBListControl *>( aMagnet )->count() );
               }else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
                  addItem_NSPopUpButton( pointer, item->value.asString()->getValue() );
               }
            }
            break;
         case CB_INFOTYPE_LISTITEMDEL:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
//               removeItem_NSListbox( pointer, aSourceValue->asInteger() );
               signalTableViewRowCountChanged( pointer, aSourceValue->asInteger(), static_cast<TCBListControl *>( aMagnet )->count() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               removeItem_NSPopUpButton( pointer, aSourceValue->asInteger() );
            }
            break;
         case CB_INFOTYPE_LISTITEMCLR:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
//               refreshItemsListbox( static_cast<TCBListControl *>( aMagnet ) );
               signalTableViewRowCountChanged( pointer, aSourceValue->asInteger(), static_cast<TCBListControl *>( aMagnet )->count() );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               refreshItemsCombobox( static_cast<TCBListControl *>( aMagnet ) );
            }
            break;
         case CB_INFOTYPE_BUTTONSTATE:
            if ( aMagnet->iRemoteType == CB_TYPE_RADIOBUTTON ) {
               setState_NSButton( pointer, aSourceValue->asBoolean(), true );
            } else {
               setState_NSButton( pointer, aSourceValue->asBoolean(), false );
            }
            break;
         case CB_INFOTYPE_ALIGNMENT:
            break;
         case CB_INFOTYPE_GRIDFULLUPDATE:
            updateCocoaGridHeaders( static_cast<TCBGrid *>( aMagnet ) );
            signalTableViewRowUpdate( pointer, -1 );
            break;
         case CB_INFOTYPE_GRIDROWUPDATE:
            signalTableViewRowUpdate( pointer, aSourceValue->asInteger() );
            break;
         case CB_INFOTYPE_GRIDROWADD:
            grid = static_cast<TCBGrid *>( aMagnet );
            signalTableViewRowCountChanged( pointer, aSourceValue->asInteger(), grid->getRowCount() );
            break;
         case CB_INFOTYPE_GRIDROWDELETE:
            grid = static_cast<TCBGrid *>( aMagnet );
            signalTableViewRowCountChanged( pointer, aSourceValue->asInteger(), grid->getRowCount() );
            break;
         case CB_INFOTYPE_GRIDCLEAR:
            signalTableViewRowUpdate( pointer, -1 );
            break;
         case CB_INFOTYPE_GRIDROWSELECT:
            setSelected_NSTableView( pointer, aSourceValue->asInteger() );
            break;
         case CB_INFOTYPE_GRIDHEADERSUPDATE:
            updateCocoaGridHeaders( static_cast<TCBGrid *>( aMagnet ) );
            break;
         case CB_INFOTYPE_SETCONTEXTMENU:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               menu = static_cast<TCBListbox *>(aMagnet)->contextmenu.internalGet();
            } else if ( aMagnet->iRemoteType == CB_TYPE_GRID ) {
               menu = static_cast<TCBGrid *>(aMagnet)->contextmenu.internalGet();
            }

            if ( menu != NULL ) {
               if ( menu->iRemoteType == 0 ) {
                  buildMenu( true, static_cast<TCBMenu *>( aMagnet ) );
               }

               setContextMenu_NSTableView( pointer, reinterpret_cast<unsigned long>( menu->iRemoteId ) );
            } else {
               setContextMenu_NSTableView( pointer, 0 );
            }
            break;
         case CB_INFOTYPE_TREEFULLUPDATE:
            signalTreeFullUpdate( pointer );
            break;
         default:
            break;
      }
   }

   lGeneralAccess.unlock();
}

void TCBDriverCocoa::getinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aDestValue ) {
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   TCBListControlItem *item;

   if ( aMagnet->iRemoteId != NULL ) {
      unsigned long pointer = reinterpret_cast<unsigned long>( aMagnet->iRemoteId );

      switch ( iInfoType ) {
         case CB_INFOTYPE_EDITTEXT:
            if ( aMagnet->iRemoteType == CB_TYPE_EDITBOX ) {
               aDestValue->setString( getText_NSControl( pointer ) );
            } else if ( aMagnet->iRemoteType == CB_TYPE_MULTILINE ) {
               aDestValue->setString( getText_NSTextView( pointer ) );
            }
            break;
         case CB_INFOTYPE_LISTITEMSEL:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               aDestValue->setInteger( getSelected_NSTableView( pointer ) );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               aDestValue->setInteger( getSelected_NSPopUpButton( pointer ) );
            }
            break;
         case CB_INFOTYPE_GRIDROWSELECT:
            aDestValue->setInteger( getSelected_NSTableView( pointer ) );
            break;
         default:
            break;
      }
   }

   lGeneralAccess.unlock();
}

long TCBDriverCocoa::messagebox( TCBMagnet *aParent, String *sCaption, String *sText, long iType ) {
   int iRet = 0;

   switch ( iType ) {
      case CB_MSGBOX_OK:
         iRet = cocoaMessageBox( sCaption->getValue(), sText->getValue(), "Ok", NULL, NULL );
         if ( iRet == 1 ) {
            return CB_MSGBOX_OK;
         }
         break;
      case CB_MSGBOX_OKCANCEL:
         iRet = cocoaMessageBox( sCaption->getValue(), sText->getValue(), "Ok", "Cancel", NULL );
         if ( iRet == 1 ) {
            return CB_MSGBOX_OK;
         } else if ( iRet == 2 ) {
            return CB_MSGBOX_CANCEL;
         }
         break;
      case CB_MSGBOX_YESNO:
         iRet = cocoaMessageBox( sCaption->getValue(), sText->getValue(), "Yes", "No", NULL );
         if ( iRet == 1 ) {
            return CB_MSGBOX_YES;
         } else if ( iRet == 2 ) {
            return CB_MSGBOX_NO;
         }
         break;
      case CB_MSGBOX_YESNOCANCEL:
         iRet = cocoaMessageBox( sCaption->getValue(), sText->getValue(), "Yes", "Cancel", "No" );
         if ( iRet == 1 ) {
            return CB_MSGBOX_YES;
         } else if ( iRet == 2 ) {
            return CB_MSGBOX_CANCEL;
         } else if ( iRet == 3 ) {
            return CB_MSGBOX_NO;
         }
         break;
      default:
         break;
   }

   return 0;
}

void TCBDriverCocoa::setStartingWindow( TCBMagnet *aWindow ) {
   this->pStartingWindow = aWindow;
}

void TCBDriverCocoa::mainloop_iteration() {
   runCocoa( reinterpret_cast<unsigned long>( this->pStartingWindow->iRemoteId) );
}

#endif // GF_OS_MAC
