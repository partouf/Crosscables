
#include "CBDriverWin32.h"

#ifdef GF_OS_WIN32

#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/GarbageCollector.h>
#include <Groundfloor/Bookshelfs/BValue.h>

#ifndef _WIN32_IE
#define _WIN32_IE	0x0500
#endif

#include <commctrl.h>

#include <richedit.h>



#ifndef PBS_MARQUEE
	#define PBS_MARQUEE 8
#endif
#ifndef PBM_MARQUEE
	#define PBM_MARQUEE (WM_USER+10)
#endif

#ifndef TVN_ITEMCHANGED
    #define TVN_ITEMCHANGED        (TVN_FIRST-18)
#endif


bool bUnicodeSupported = true;

TCBDriverWin32 *CBGlobalDriver = NULL;
String *sRichEditClassName = NULL;
int iRichEditVersion = 10;

void stretchChildren( TCBMagnet *aMagnet );
void stretchMagnet( TCBMagnet *aMagnet );


void CBWin32Initialize( TCBDriverWin32 *pDriver ) {
   GFTRACE();
   CBGlobalDriver = pDriver;

   INITCOMMONCONTROLSEX comctlstruct;
   comctlstruct.dwSize  = sizeof(INITCOMMONCONTROLSEX);
   comctlstruct.dwICC   = (ICC_USEREX_CLASSES | ICC_PROGRESS_CLASS | ICC_LISTVIEW_CLASSES | ICC_NATIVEFNTCTL_CLASS | ICC_TAB_CLASSES | ICC_TREEVIEW_CLASSES);

   InitCommonControlsEx( &comctlstruct );

/*
   HRESULT hrCom = CoInitializeEx( NULL, COINIT_MULTITHREADED );
   if ( ( hrCom == S_OK ) || ( hrCom == S_FALSE ) || ( hrCom == RPC_E_CHANGED_MODE ) ) {
      MessageBoxA( NULL, "Couldn't initialize COM", "COM Error", MB_OK );
   }
*/
   sRichEditClassName = new String();
   sRichEditClassName->setValue_ansi( "EDIT" );

   if ( LoadLibraryA( "msftedit.dll" ) != NULL ) {
      sRichEditClassName->setValue_ansi( "RICHEDIT50W" );
      iRichEditVersion = 50;
   } else if ( LoadLibraryA( "Riched20.dll" ) != NULL ) {
      sRichEditClassName->setValue_ansi( "RICHEDIT20W" );
      iRichEditVersion = 20;
   } else if ( LoadLibraryA( "Riched32.dll" ) != NULL ) {
      sRichEditClassName->setValue_ansi( "RICHEDIT" );
      iRichEditVersion = 10;
   }

/*
   BValue *val = new BValue();
   String *sUnicodeTest = new String( "test" );
   sUnicodeTest->transformToWidestring( true, false );
   bUnicodeSupported = ( sUnicodeTest->getLength() == 8 );
   val->setInteger( sUnicodeTest->getLength() );
   MessageBoxA( NULL, val->asString()->getValue(), "unicode", MB_OK );
   delete sUnicodeTest;
   delete val;

   OSVERSIONINFO osvi;
   BOOL bIsWindowsXPorLater;

   ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

   GetVersionEx(&osvi);

   bIsWindowsXPorLater =
   ( (osvi.dwMajorVersion > 5) ||
   ( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1) ));

   if(bIsWindowsXPorLater)
   printf("The system meets the requirements.\n");
   else printf("The system does not meet the requirements.\n");
*/
}

void CBWin32Finalize() {
   delete sRichEditClassName;
   CBGlobalDriver = NULL;
}

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

TCBCollector *CBDriverWin32GetWindowByMenu( const Vector *windowslist, const TCBMenu *menu ) {
   TCBMagnet *container = NULL;
   TCBWin32Window *window = NULL;

   unsigned int c = windowslist->size();
   for ( unsigned int i = 0; i < c; i++ ) {
      window = static_cast<TCBWin32Window *>( windowslist->elementAt(i) );
      if ( window != NULL ) {
         container = window->magnet;
         if ( container != NULL ) {
            if ( container->iRemoteType == CB_TYPE_WINDOW ) {
               if ( static_cast<TCBWindow *>( container )->mainMenu == menu ) {
                  return reinterpret_cast<TCBCollector *>( container );
               }
            }
         }
      }
   }

   return NULL;
}

void changeNotebookTab( TCBMagnet *magnet, LPNMHDR nmhdr ) {
   TCBNotebook *notebook = static_cast<TCBNotebook *>( magnet );

   int iOldSelection = notebook->selected.internalGet();
   int iNewSelection = SendMessage( nmhdr->hwndFrom, TCM_GETCURSEL, 0, 0 );

   if ( iOldSelection != -1 ) {
      TCBNotebookTab *oldTab = static_cast<TCBNotebookTab *>( notebook->getVector()->elementAt( iOldSelection ) );

      oldTab->visible.internalSet( false );
      oldTab->selfVisibilityChange();
      oldTab->visible.internalSet( true );
   }

   if ( iNewSelection != -1 ) {
      TCBNotebookTab *newTab = static_cast<TCBNotebookTab *>( notebook->getVector()->elementAt( iNewSelection ) );

      newTab->selfVisibilityChange();
   }

   notebook->onChange.execute( notebook );
}

void stretchMagnet( TCBMagnet *aMagnet ) {
   if ( aMagnet != NULL ) {
      int iXcorr = 0, iYcorr = 0, iHcorr = 0;

      TCBMagnet *pParent = aMagnet->parent.get();
      if ( pParent != NULL ) {
         if ( pParent->iRemoteType == CB_TYPE_GROUPBOX ) {
            iYcorr = 15;
            iXcorr = 2;
         }
      }

      if ( aMagnet->iRemoteType == CB_TYPE_GROUPBOX ) {
          iHcorr = -5;
      }

      HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );
      if ( h != NULL ) {
         if ( ( aMagnet->w <= 0 ) || ( aMagnet->h <= 0 ) || (aMagnet->x < 0) || (aMagnet->y < 0) ) {
            TCBCollector *parent = aMagnet->parent.get();
            if ( parent != NULL ) {
               HWND hndParent = reinterpret_cast<HWND>( parent->iRemoteId );
               if ( hndParent != 0 ) {
                  RECT rctParent;
                  if ( GetClientRect( hndParent, &rctParent ) ) {
                     int x = aMagnet->x + iXcorr;
                     int y = aMagnet->y + iYcorr;

                     if ( x < 0 ) {
                        x = rctParent.right + x;
                     }
                     if ( y < 0 ) {
                        y = rctParent.bottom + y;
                     }

                     int width = aMagnet->w;
                     int height = aMagnet->h + iHcorr;

                     if ( width <= 0 ) {
                        width = rctParent.right - x + width;
                     }
                     if ( height <= 0 ) {
                        height = rctParent.bottom - y + height;
                     }

                     if ( aMagnet->iRemoteType == CB_TYPE_GROUPBOX ) {
                        CBGlobalDriver->hide_magnet( aMagnet );
                        UpdateWindow( hndParent );
                        MoveWindow( h, x, y, width, height, FALSE );
                        stretchChildren( aMagnet );
                        CBGlobalDriver->show_magnet( aMagnet );
                     } else {
                        MoveWindow( h, x, y, width, height, TRUE );

                        if ( aMagnet->iRemoteType >= 100 ) {
                           stretchChildren( aMagnet );
                        }
                     }

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

TCBWin32Window *getWin32WindowByMagnet( TCBMagnet *aMagnet ) {
	TCBWin32Window *window = NULL;

	unsigned int c = CBGlobalDriver->windows->size();
	for ( unsigned int i = 0; i < c; i++ ) {
		window = static_cast<TCBWin32Window *>( CBGlobalDriver->windows->elementAt(i) );

		if ( window->magnet == aMagnet ) {
			break;
		} else {
			window = NULL;
		}
	}

	return window;
}

LRESULT CALLBACK CBDriverWin32MessageHandler( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
   bool bProcessed = false;
   TCBMagnet *child = NULL;
   TCBCollector *container = NULL;
   TCBWin32Window *window = NULL;
   MultiNotify<TCBMagnet *> *notifyobj;
   WINDOWPOS *pNewPos = NULL;
   LPNMHDR nmhdr;
   long customId = 0;
   long code = 0;
   long data = 0;
   TCBMenu *menu = NULL;
   RECT size;
   TCBKey *vk = NULL;

   if ( CBGlobalDriver != NULL ) {
      HWND containerHandle = hwnd;
      if ( Message == WM_NOTIFY ) {
         nmhdr = reinterpret_cast<LPNMHDR>( lParam );
         containerHandle = nmhdr->hwndFrom;
      }

      if ( containerHandle != NULL ) {
         child = reinterpret_cast<TCBMagnet *>( GetWindowLongPtr( containerHandle, GWLP_USERDATA ) );
         if ( child != NULL ) {
            if ( child->iRemoteType >= 100 ) {
               container = reinterpret_cast<TCBCollector *>( child );
            }
         }
      }

      if ( container != NULL ) {
         window = getWin32WindowByMagnet( container );
      }

      if ( window != NULL ) {
         if (  ( window->magnet->iRemoteType != CB_TYPE_WINDOW ) &&
               ( window->magnet->iRemoteType > 100 ) ) {
            if ( window->wndclass.lpfnWndProc != NULL ) {
               // only a window should be allowed to get a WM_CLOSE
               if (Message != WM_CLOSE) {
                  CallWindowProc( window->wndclass.lpfnWndProc, hwnd, Message, wParam, lParam );
               }
            }
         }
      }

      if ( child != NULL ) {
         switch(Message) {
            case WM_COMMAND:
               customId = GetWindowLongPtr( reinterpret_cast<HWND>(lParam), GWLP_USERDATA );

               child = reinterpret_cast<TCBMagnet *>( customId );
               if ( child != NULL ) {
                  code = HIWORD( wParam );
                  data = LOWORD( wParam );

                  switch( code ) {
                     case BN_CLICKED:
                        bProcessed = true;
                        child->onClick.execute( child );
                        break;
                     case LBN_DBLCLK:
                        child->onDoubleClick.execute( child );
                        break;
                     case CBN_SELENDOK:
                        bProcessed = true;
                        child->onClick.execute( child );
                        break;
                     case EN_CHANGE:
                        bProcessed = true;
                        child->onChange.execute( child );
                        break;
                     default:
                        break;
                  }
               } else {
                  // menuitem clicks end up here
                  if ( HIWORD(wParam) == 0 ) {
                      notifyobj = static_cast<MultiNotify<TCBMagnet *> *>( CBGlobalDriver->commandlist.elementAt( LOWORD(wParam) ) );
                      if ( notifyobj != NULL ) {
                         notifyobj->execute();
                      }
                  }
                  if ( HIWORD(wParam) == 1 ) {
                      notifyobj = static_cast<MultiNotify<TCBMagnet *> *>( CBGlobalDriver->commandlist.elementAt( LOWORD(wParam) ) );
                      if ( notifyobj != NULL ) {
                         notifyobj->execute();
                      }
                  }
               }

               break;
            case WM_NOTIFY:
               nmhdr = reinterpret_cast<LPNMHDR>( lParam );
               customId = GetWindowLongPtr( reinterpret_cast<HWND>( nmhdr->hwndFrom ), GWLP_USERDATA );
               child = reinterpret_cast<TCBMagnet *>( customId );

               if ( child != NULL ) {
                  switch ( nmhdr->code ) {
                     case TCN_SELCHANGE:
                        bProcessed = true;
                        changeNotebookTab( child, nmhdr );
                        break;
                     case TVN_SELCHANGED:
                     case LVN_ITEMACTIVATE:
                        bProcessed = true;
                        child->onClick.execute( child );
                        break;
                     case TVN_ITEMCHANGED:
                     case LVN_ITEMCHANGED:
                        bProcessed = true;
                        child->onChange.execute( child );
                        break;
                     case NM_DBLCLK:
                        bProcessed = true;
                        child->onDoubleClick.execute( child );
                        break;
                     default:
                        break;
                  }
               }

               break;
            default:
               break;
         }
      }

      if ( window != NULL ) {
         // WM_CREATE never appears here, as the magnet's handle
         //  was not determined yet when this handler was called
         switch(Message) {
            case WM_SYSCOMMAND:
               switch ( wParam ) {
                  case SC_CLOSE:
                     // this is a real window close request from the user
                     break;
               }
               break;
            case WM_CLOSE:
               if ( container->iRemoteType == CB_TYPE_WINDOW ) {
                  bProcessed = true;
                  container->onHide.execute( container );
               } else {
                  bProcessed = true;
               }
               break;
            case WM_DESTROY:
               break;
            case WM_WINDOWPOSCHANGED:
               if ( container->iRemoteType == CB_TYPE_WINDOW ) {
                  pNewPos = reinterpret_cast<WINDOWPOS *>( lParam );
                  if ( ( container->x != pNewPos->x ) ||
                     ( container->y != pNewPos->y ) ) {
                     container->x = pNewPos->x;
                     container->y = pNewPos->y;

                     container->onMove.execute( container );
                  }

                  if ( pNewPos->flags & SWP_NOSIZE ) {
                  } else {
                     GetClientRect( hwnd, &size );

                     container->w = size.right;
                     container->h = size.bottom;

                     stretchChildren( container );

                     container->onResize.execute( container );
                  }
               }
               break;
            case WM_CONTEXTMENU:
               customId = GetWindowLongPtr( reinterpret_cast<HWND>( wParam ), GWLP_USERDATA );
               child = reinterpret_cast<TCBMagnet *>( customId );

               if ( child != NULL ) {
                  if ( ( child->iRemoteType == CB_TYPE_LISTBOX ) ||
                       ( child->iRemoteType == CB_TYPE_GRID ) ||
                       ( child->iRemoteType == CB_TYPE_EDITBOX ) ||
                       ( child->iRemoteType == CB_TYPE_MULTILINE ) ) {
                     if ( static_cast<TCBMagnetWithContextMenu *>(child)->shouldShowContextMenu.internalGet() ) {
                        menu = static_cast<TCBMagnetWithContextMenu *>(child)->contextmenu.internalGet();
                     } else {
                        if ( static_cast<TCBMagnetWithContextMenu *>(child)->shouldShowDefaultMenu.internalGet() ) {
                           menu = static_cast<TCBMagnetWithContextMenu *>(child)->defaultmenu.internalGet();
                        }
                     }
                  }
               }

               if ( menu != NULL ) {
                  if ( menu->iRemoteId == NULL ) {
                     CBGlobalDriver->buildMenu( true, menu );
                  }

                  long x = LOWORD(lParam);
                  long y = HIWORD(lParam);

                  if ( x == -1 ) {
                     RECT pt;

                     if ( GetWindowRect( reinterpret_cast<HWND>( wParam ), &pt ) ) {
                        x = pt.left;
                        y = pt.top;
                     }
                  }

                  container = menu->getNonMenuParent();
                  if ( container != NULL ) {
                     TrackPopupMenu( reinterpret_cast<HMENU>( menu->iRemoteId ), 0, x, y, 0, reinterpret_cast<HWND>( container->iRemoteId ), NULL);
                  }

                  bProcessed = true;
               }
               break;
            case WM_KEYDOWN:
                vk = new TCBKey( wParam );
                container->onKeyDown.execute( vk );
                delete vk;
                break;
            case WM_KEYUP:
                vk = new TCBKey( wParam );
                container->onKeyUp.execute( vk );
                delete vk;
                break;
            default:
               break;
         }
      } else {
         // ?
      }
   }

   if ( !bProcessed ) {
      return DefWindowProc(hwnd, Message, wParam, lParam);
   } else {
      return TRUE;
   }
}

void TCBDriverWin32::preparefont( TCBFont *font ) {
	int size = -MulDiv(font->size.get(), GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
	HANDLE h = CreateFontA( size, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, font->name.get() );

	font->iRemoteId = h;
}

TCBDriverWin32::TCBDriverWin32( HINSTANCE hApplication, const char *sFontName, int iFontSize ) : TCBDriver() {
   GFTRACE();
   this->hApp     = hApplication;
   this->windows  = new Vector();

   this->sCurrentFontName.setValue( sFontName, strlen(sFontName) );
   this->iCurrentFontSize = -MulDiv(iFontSize, GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);

//   this->hCurrentFont = CreateFont( iCurrentFontSize, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, sCurrentFontName.getValue() );
   this->hCurrentFont = CreateFontA( iCurrentFontSize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, sCurrentFontName.getValue() );
   this->hMonospaceFont = CreateFontA( iCurrentFontSize, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Courier New" );

   this->commandlist.addElement( NULL );  // important to release bogus WM_COMMAND messages into the void

   pArrAccelerator = NULL;
   hndAccelerator = NULL;
}

TCBDriverWin32::~TCBDriverWin32() {
   GFTRACE();
   if ( hndAccelerator != NULL ) {
      DestroyAcceleratorTable( hndAccelerator );
   }
   if ( pArrAccelerator != NULL ) {
      GFFree( pArrAccelerator );
   }

   delete windows;
}

int TCBDriverWin32::addControlKey( char cKey, WORD iCommandId ) {
/*
   int iOldSize = iAcceleratorCount * sizeof(ACCEL);
   ACCEL *pOldArr = pArrAccelerator;
   ACCEL *pNewArr = reinterpret_cast<ACCEL *>( GFMalloc( iOldSize + sizeof(ACCEL) ) );

   if ( hndAccelerator != NULL ) {
      DestroyAcceleratorTable( hndAccelerator );

      hndAccelerator = NULL;
   }

   pArrAccelerator = pNewArr;
   if ( pOldArr != NULL ) {
      memcpy( pNewArr, pOldArr, iOldSize );

      GFFree( pOldArr );
   }

   pArrAccelerator[iAcceleratorCount].fVirt  = FCONTROL;
   pArrAccelerator[iAcceleratorCount].key    = cKey;
   pArrAccelerator[iAcceleratorCount].cmd    = 0x10000 + iCommandId;

//   hndAccelerator = CreateAcceleratorTable( pArrAccelerator, iAcceleratorCount + 1 );
   if ( hndAccelerator != 0 ) {
      iAcceleratorCount++;

      return iAcceleratorCount - 1;
   } else {
      return -1;
   }
*/
   return -1;
}



void TCBDriverWin32::initialize( TCBScreen *aScreen ) {
   GFTRACE();
   TCBDriver::initialize( aScreen );

}

void TCBDriverWin32::finalize() {
   GFTRACE();

   TCBDriver::finalize();
}



bool TCBDriverWin32::selectItemGrid( TCBGrid *aMagnet, BValue *aValue ) {
   HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );
   int i = aValue->asInteger();

   ListView_SetItemState( h, i, LVIS_SELECTED, LVIS_SELECTED );

   SendMessage( h, LVM_SETSELECTIONMARK, 0, i );

   return true;
}

bool TCBDriverWin32::getSelectedItemGrid( TCBGrid *aMagnet, BValue *aValue ) {
   HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

   int i = SendMessage( h, LVM_GETSELECTIONMARK, 0, 0 );
   if ( ListView_GetItemState( h, i, LVIS_SELECTED ) != 0 ) {
      aValue->setInteger( i );
   } else {
      aValue->setInteger( -1 );
   }

   return true;
}

bool TCBDriverWin32::updateGridRow( TCBGrid *aMagnet, BValue *aValue ) {
   BTable *data = aMagnet->data.get();
   HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );
   int iRow = aValue->asInteger();
   if ( data != NULL ) {
      BRecords *records = data->records.get();
      BRecord *row = records->getRecord( iRow );
      if ( row != NULL ) {
         int d = aMagnet->headers.size();
         for ( int j = 0; j < d; j++ ) {
            TCBGridHeader *header = aMagnet->headers.getHeader( j );
            if ( header != NULL ) {
               String *s = row->getValue(header->column.get())->asString();

               String *sCopy = new String( s );
               sCopy->transformToWidestring( true );
            	LVITEMW item;
               memset( &item, 0, sizeof(LVITEMW) );
               item.mask      = LVIF_TEXT;
               item.iItem     = iRow;
               item.iSubItem  = j;
               item.pszText   = sCopy->getValueAsWide();
               if ( j != 0 ) {
                  SendMessageW( h, LVM_SETITEMW, 0, reinterpret_cast<LPARAM>(&item) );
               } else {
                  SendMessageW( h, LVM_SETITEMW, 0, reinterpret_cast<LPARAM>(&item) );
               }
               delete sCopy;

            } else {
               return false;
            }
         }

         return true;
      }
   }

   return false;
}

bool TCBDriverWin32::insertGridRow( TCBGrid *aMagnet, BValue *aValue ) {
   BTable *data = aMagnet->data.get();
   HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );
   int iRow = aValue->asInteger();
   if ( data != NULL ) {
      BRecords *records = data->records.get();
      BRecord *row = records->getRecord( iRow );
      if ( row != NULL ) {
         int d = aMagnet->headers.size();
         for ( int j = 0; j < d; j++ ) {
            TCBGridHeader *header = aMagnet->headers.getHeader( j );
            if ( header != NULL ) {
               String *s = row->getValue(header->column.get())->asString();

               String *sCopy = new String( s );
               sCopy->transformToWidestring( true );
            	LVITEMW item;
               memset( &item, 0, sizeof(LVITEMW) );
               item.mask      = LVIF_TEXT;
               item.iItem     = iRow;
               item.iSubItem  = j;
               item.pszText   = sCopy->getValueAsWide();
               if ( j != 0 ) {
                  SendMessageW( h, LVM_SETITEMW, 0, reinterpret_cast<LPARAM>(&item) );
               } else {
                  SendMessageW( h, LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&item) );
               }
               delete sCopy;

            } else {
               return false;
            }
         }

         return true;
      }
   }

   return false;
}

bool TCBDriverWin32::updateGridAllRows( TCBGrid *aMagnet, BValue *aValue ) {
   BTable *data = aMagnet->data.get();
   HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

   SendMessage( h, LVM_DELETEALLITEMS, 0, 0 );

   if ( data != NULL ) {
      int d = aMagnet->headers.size();
      BRecords *records = data->records.get();
      if ( records != NULL ) {
         unsigned int c = records->size();
         for ( unsigned int i = 0; i < c; i++ ) {
            BRecord *row = records->getRecord( i );
            for ( int j = 0; j < d; j++ ) {
               TCBGridHeader *header = aMagnet->headers.getHeader( j );
               if ( header != NULL ) {
                  String *s = row->getValue(header->column.get())->asString();

                  String *sCopy = new String( s );
                  sCopy->transformToWidestring( true );
               	LVITEMW item;
                  memset( &item, 0, sizeof(LVITEMW) );

                  item.mask      = LVIF_TEXT;
                  item.iItem     = i;
                  item.iSubItem  = j;
                  item.pszText   = sCopy->getValueAsWide();
                  if ( j == 0 ) {
                     SendMessageW( h, LVM_INSERTITEMW, 0, reinterpret_cast<LPARAM>(&item) );
                  } else {
                     SendMessageW( h, LVM_SETITEMW, 0, reinterpret_cast<LPARAM>(&item) );
                  }
                  delete sCopy;

               } else {
                  return false;
               }
            }
         }
      }
   }

   aMagnet->selected.set( aMagnet->selected.internalGet() );

   return true;
}

bool TCBDriverWin32::updateGridHeaders( TCBGrid *aMagnet, BValue *aValue ) {
   HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

   LV_COLUMNW lvc;
   memset( &lvc, 0, sizeof(LV_COLUMNW) );
   lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
   lvc.fmt = LVCFMT_LEFT;

   int s = aMagnet->headers.size();
   int c = reinterpret_cast<int>( aMagnet->magic );
   if ( s < c ) {
      int diff = c - s;
      for ( int i = 0; i < diff; i++ ) {
         ListView_DeleteColumn( h, s );
      }

      c = s;
   }

   for ( int j = 0; j < c; j++ ) {
      TCBGridHeader *header = aMagnet->headers.getHeader( j );
      String *sCopy = new String( header->caption.link() );
      sCopy->transformToWidestring( true );

      if ( header != NULL ) {
         lvc.iSubItem   = 0;
         lvc.cx         = header->width.get();
         lvc.pszText    = sCopy->getValueAsWide();

         SendMessageW( h, LVM_SETCOLUMNW, j, reinterpret_cast<LPARAM>(&lvc) );

         delete sCopy;
      } else {
         delete sCopy;

         return false;  // hint: do compress() first
      }
   }

   if ( s > c ) {
      for ( int k = c; k < s; k++ ) {
         TCBGridHeader *header = aMagnet->headers.getHeader( k );

         String *sCopy = new String( header->caption.link() );
         sCopy->transformToWidestring( true );

         if ( header != NULL ) {
            lvc.iSubItem   = 0;
            lvc.cx         = header->width.get();
            lvc.pszText    = sCopy->getValueAsWide();

            SendMessageW( h, LVM_INSERTCOLUMNW, k, reinterpret_cast<LPARAM>(&lvc) );

            delete sCopy;
         } else {
            delete sCopy;

            return false;
         }
      }
   }

   aMagnet->magic = reinterpret_cast<void *>( s );

   return true;
}


void TCBDriverWin32::addMenuItem( TCBMenu *aParentMenu, TCBMenu *aMenu ) {
   String wCaption;
   wCaption.setValue( aMenu->caption.link() );

   if ( wCaption.match_ansi( "-" ) ) {

      AppendMenuW( reinterpret_cast<HMENU>( aParentMenu->iRemoteId ), MF_SEPARATOR, 0, 0 );

   } else {
      char cKey = aMenu->controlkey.get();

      if ( cKey != 0 ) {
         wCaption.append_ansi("\tCtrl+");
         if ( ( cKey >= 'a' ) && ( cKey <= 'z' ) ) {
            cKey += ('A' - 'a');
         }
         wCaption.append( cKey );
      }

      wCaption.transformToWidestring( true );

      Vector *list = aMenu->getVector();
      unsigned int c = list->size();
      if ( c > 0 ) {
         HMENU hSubMenu = CreatePopupMenu();
         aMenu->iRemoteId = reinterpret_cast<void *>( hSubMenu );
         AppendMenuW( reinterpret_cast<HMENU>( aParentMenu->iRemoteId ), MF_STRING | MF_POPUP, (UINT)hSubMenu, wCaption.getValueAsWide() );
         for ( unsigned int i = 0; i < c; i++ ) {
            addMenuItem( aMenu, static_cast<TCBMenu *>( list->elementAt(i) ) );
         }
      } else {
         unsigned int j = commandlist.addElement( &aMenu->onClick );
         aMenu->iRemoteId = reinterpret_cast<void *>( j );
         if ( aMenu->checked.get() ) {
            if ( cKey != 0 ) {
               AppendMenuW( reinterpret_cast<HMENU>( aParentMenu->iRemoteId ), MF_STRING | MF_CHECKED, 0x10000 + j, wCaption.getValueAsWide() );
               addControlKey( cKey, j );
            } else {
               AppendMenuW( reinterpret_cast<HMENU>( aParentMenu->iRemoteId ), MF_STRING | MF_CHECKED, 0x10000 + j, wCaption.getValueAsWide() );
            }
         } else {
            AppendMenuW( reinterpret_cast<HMENU>( aParentMenu->iRemoteId ), MF_STRING | MF_UNCHECKED, 0x10000 + j, wCaption.getValueAsWide() );
            if ( cKey != 0 ) {
               addControlKey( cKey, j );
            }
         }
      }
   }
}

void TCBDriverWin32::buildMenu( bool popup, TCBMenu *aMenu ) {
   if ( aMenu != NULL ) {
      HMENU hMenu;
      if ( popup ) {
         hMenu = CreatePopupMenu();
      } else {
         hMenu = CreateMenu();
      }
      aMenu->iRemoteId = reinterpret_cast<void *>( hMenu );

      Vector *list = aMenu->getVector();
      unsigned int c = list->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBMenu *sub = static_cast<TCBMenu *>( list->elementAt(i) );
         addMenuItem( aMenu, sub );
      }
   }
}

void TCBDriverWin32::create_magnet( TCBMagnet *aMagnet ) {
   GFTRACE_D( aMagnet->iRemoteType );
   TCBWin32Window *w;
   HWND h = NULL;
   int iTmp;
   BValue *val;
   String *sTmp;
   RECT size;
   TCBWindow *win;
   HMENU hMenu = NULL;
   TCITEMA tabitem;
   HDC hdcPrimaryMonitor;

  int iXcorr = 0, iYcorr = 0;

   TCBMagnet *pParent = aMagnet->parent.get();
   if ( pParent != NULL ) {
      if ( pParent->iRemoteType == CB_TYPE_GROUPBOX ) {
         iYcorr = 15;
         iXcorr = 2;
      }
   }

   switch ( aMagnet->iRemoteType ) {
      case CB_TYPE_SCREEN:
         hdcPrimaryMonitor = GetDC( NULL );
         aMagnet->w = GetDeviceCaps( hdcPrimaryMonitor, HORZRES );
         aMagnet->h = GetDeviceCaps( hdcPrimaryMonitor, VERTRES );
         ReleaseDC( NULL, hdcPrimaryMonitor );
         break;
      case CB_TYPE_WINDOW:
         val = new BValue();
         val->setInt64( reinterpret_cast<__int64>( aMagnet ) );
         sTmp = DisposableStr( "chalkboard", GC_LIVEFOREVER );
         sTmp->append( val->asString() );
         sTmp->transformToWidestring(true, true);
         delete val;

         win = static_cast<TCBWindow *>(aMagnet);
         if ( win->mainMenu != NULL ) {
            if ( win->mainMenu->parent.get() != NULL ) {
               buildMenu( false, win->mainMenu );

               hMenu = reinterpret_cast<HMENU>( win->mainMenu->iRemoteId );
            }
         }

         w = new TCBWin32Window();
         w->magnet = static_cast<TCBCollector *>(aMagnet);
         w->wndclass.hInstance      = hApp;
         w->wndclass.lpszClassName  = sTmp->getValueAsWide();
         w->wndclass.lpfnWndProc    = CBDriverWin32MessageHandler;
         w->wndclass.style          = CS_DBLCLKS;
         w->wndclass.cbSize         = sizeof (WNDCLASSEX);
         w->wndclass.hIcon          = LoadIcon (NULL, IDI_APPLICATION);
         w->wndclass.hIconSm        = LoadIcon (NULL, IDI_APPLICATION);
         w->wndclass.hCursor        = LoadCursor (NULL, IDC_ARROW);
         w->wndclass.lpszMenuName   = NULL;
         w->wndclass.cbClsExtra     = 0;
         w->wndclass.cbWndExtra     = 0;
         w->wndclass.hbrBackground  = GetSysColorBrush(COLOR_BTNFACE);

         if (!RegisterClassExW(&w->wndclass)) {
            delete w;

            break;
         }

         size.left	= aMagnet->x + iXcorr;
         size.top		= aMagnet->y + iYcorr;
         size.bottom	= size.top + aMagnet->h;
         size.right	= size.left + aMagnet->w;

         AdjustWindowRectEx( &size, WS_OVERLAPPEDWINDOW | WS_GROUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, (hMenu != NULL), WS_EX_STATICEDGE | WS_EX_CONTROLPARENT );

         h = CreateWindowExW(
               WS_EX_STATICEDGE | WS_EX_CONTROLPARENT,
               w->wndclass.lpszClassName,
               L"",
               WS_OVERLAPPEDWINDOW | WS_GROUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
               aMagnet->x, aMagnet->y, size.right - size.left + 2, size.bottom - size.top + 2,
               NULL, NULL, this->hApp, NULL );

         if ( hMenu != NULL ) {
            SetMenu( h, hMenu );
         }

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );

			repositionWindow( aMagnet );

         windows->addElement( w );
         break;
      case CB_TYPE_GROUPBOX:
         h = CreateWindowExA(
                     WS_EX_CONTROLPARENT | WS_EX_TRANSPARENT,
                     "BUTTON",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | WS_GROUP | BS_GROUPBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h - 5,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );

         w = new TCBWin32Window();
         w->magnet = static_cast<TCBCollector *>(aMagnet);
         w->wndclass.lpfnWndProc = (WNDPROC)SetWindowLongPtr( h, GWL_WNDPROC, (DWORD)CBDriverWin32MessageHandler );

         windows->addElement( w );
         break;
      case CB_TYPE_PANEL:
         h = CreateWindowExA(
                     WS_EX_CONTROLPARENT,
                     "STATIC",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | WS_GROUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );

         w = new TCBWin32Window();
         w->magnet = static_cast<TCBCollector *>(aMagnet);
         w->wndclass.lpfnWndProc = (WNDPROC)SetWindowLongPtr( h, GWL_WNDPROC, (DWORD)CBDriverWin32MessageHandler );

         windows->addElement( w );
         break;
      case CB_TYPE_LABEL:
         iTmp = 0;
         switch ( static_cast<TCBLabel *>(aMagnet )->alignment.internalGet() ) {
            case CB_ALIGN_LEFT:
               iTmp = SS_LEFT;
               break;
            case CB_ALIGN_CENTER:
               iTmp = SS_CENTER;
               break;
            case CB_ALIGN_RIGHT:
               iTmp = SS_RIGHT;
               break;
         }

         h = CreateWindowExA(
                     0,
                     "STATIC",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | iTmp,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_EDITBOX:
         h = CreateWindowExA(
                     WS_EX_CLIENTEDGE,
                     "EDIT",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_MULTILINE:
         h = CreateWindowExA(
                     WS_EX_STATICEDGE,
                     sRichEditClassName->getValue(),
                     aMagnet->caption.internalGet(),
                     WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL | ES_WANTRETURN,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_LISTBOX:
         h = CreateWindowExA(
                     WS_EX_STATICEDGE,
                     "LISTBOX",
                     NULL,
                     WS_CHILD | WS_TABSTOP | WS_VSCROLL,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_COMBOBOX:
         // TODO: the height of Win32 comboboxes include dropdownbox height
         h = CreateWindowExA(
                     0,
                     "COMBOBOX",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST | WS_VSCROLL,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h + 150,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_BUTTON:
         h = CreateWindowExA(
                     0,
                     "BUTTON",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | BS_PUSHBUTTON | WS_TABSTOP,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_CHECKBOX:
         h = CreateWindowExA(
                     WS_EX_TRANSPARENT,
                     "BUTTON",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | BS_AUTOCHECKBOX | WS_TABSTOP,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_RADIOBUTTON:
         h = CreateWindowExA(
                     WS_EX_TRANSPARENT,
                     "BUTTON",
                     aMagnet->caption.internalGet(),
                     WS_CHILD | BS_AUTORADIOBUTTON | WS_TABSTOP,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_PROGRESSBAR:
         h = CreateWindowExA(
                     0,
                     PROGRESS_CLASSA,
                     NULL,
                     WS_CHILD | PBS_SMOOTH,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         if ( static_cast<TCBProgressbar *>( aMagnet )->undetermined.internalGet() ) {
            SetWindowLongPtr( h, GWL_STYLE, GetWindowLongPtr( h, GWL_STYLE) | PBS_MARQUEE );
            SendMessage( h, PBM_MARQUEE, TRUE, 10 );
         }

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_GRID:
         h = CreateWindowExA(
                     WS_EX_STATICEDGE,
                     "SysListView32",
                     NULL,
                     WS_CHILD | WS_HSCROLL | WS_VSCROLL | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_TABSTOP,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         SendMessage( h, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_TREE:
         h = CreateWindowExA(
                     WS_EX_STATICEDGE,
                     "SysTreeView32",
                     NULL,
                     WS_CHILD | WS_HSCROLL | WS_VSCROLL | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_NOTEBOOK:
         h = CreateWindowExA(
                     WS_EX_CONTROLPARENT | WS_EX_NOPARENTNOTIFY,
                     "SysTabControl32",
                     NULL,
                     WS_CHILD | WS_GROUP | WS_CLIPCHILDREN | WS_TABSTOP | TCS_MULTILINE | TCS_HOTTRACK | TCS_TABS,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

//         w = new TCBWin32Window();
//         w->magnet = static_cast<TCBCollector *>(aMagnet);
//         w->wndclass.lpfnWndProc = (WNDPROC)SetWindowLongPtr( h, GWL_WNDPROC, (DWORD)CBDriverWin32MessageHandler );
//         windows->addElement( w );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
      case CB_TYPE_NOTEBOOKTAB:
         h = CreateWindowExA(
                     WS_EX_TRANSPARENT | WS_EX_CONTROLPARENT,
                     "STATIC",
                     NULL,
                     WS_CHILD | WS_GROUP | WS_CLIPCHILDREN | SS_OWNERDRAW,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );

// todo wchar_t tabitem
         tabitem.mask = TCIF_TEXT;
         tabitem.iImage = 0;
         tabitem.lParam = 0;
         tabitem.dwState = 0;
         tabitem.dwStateMask = 0;
         tabitem.lParam = 0;
         tabitem.pszText = aMagnet->caption.internalGet();
         tabitem.cchTextMax = 0;

         SendMessage( reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ), TCM_INSERTITEM, aMagnet->parent.get()->getVector()->findElement( aMagnet ), (LPARAM)&tabitem );

         w = new TCBWin32Window();
         w->magnet = static_cast<TCBCollector *>(aMagnet);
         w->wndclass.lpfnWndProc = (WNDPROC)SetWindowLongPtr( h, GWL_WNDPROC, (DWORD)CBDriverWin32MessageHandler );

         windows->addElement( w );
         break;
      case CB_TYPE_MENU:
         // parent TCBWindow makes this object
         break;
      default:
         h = CreateWindowExA(
                     WS_EX_TRANSPARENT,
                     "STATIC",
                     "Unknown Control",
                     WS_CHILD,
                     aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h,
                     reinterpret_cast<HWND>( aMagnet->parent.get()->iRemoteId ),
                     0,
                     this->hApp,
                     NULL
                  );

         aMagnet->iRemoteId = reinterpret_cast<void *>( h );
         break;
   }

   if ( h != NULL ) {
      SetWindowLongPtr( h, GWLP_USERDATA, (long)aMagnet );

      stretchMagnet( aMagnet );

      BValue *value = new BValue();
      value->setString( aMagnet->caption.internalGet() );
      setinfo_magnet( aMagnet, CB_INFOTYPE_EDITTEXT, value );
      delete value;
   }
}

void TCBDriverWin32::destroy_magnet( TCBMagnet *aMagnet ) {
   GFTRACE();
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   if ( aMagnet->iRemoteId != NULL ) {
      HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

      DestroyWindow( h );
   }

   lGeneralAccess.unlock();
}

void TCBDriverWin32::hide_magnet( TCBMagnet *aMagnet ) {
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   if ( aMagnet->iRemoteId == NULL ) {
      create_magnet( aMagnet );
   }

   switch ( aMagnet->iRemoteType ) {
      default:
         ShowWindow( reinterpret_cast<HWND>( aMagnet->iRemoteId ), SW_HIDE );

         break;
   }

   lGeneralAccess.unlock();
}

void TCBDriverWin32::show_magnet( TCBMagnet *aMagnet ) {
   GFTRACE();
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   if ( aMagnet->iRemoteId == NULL ) {
      create_magnet( aMagnet );
   }

   bool bHasCustomFont = false;
   TCBFont *f = aMagnet->font.get();
   if (f != NULL) {
	   bHasCustomFont = true;
	   if (f->iRemoteId == 0) {
		   preparefont(f);
	   }
   }

   HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );
   switch ( aMagnet->iRemoteType ) {
      case CB_TYPE_LISTBOX:
         refreshItemsListbox( static_cast<TCBListControl *>( aMagnet ) );
         break;
      case CB_TYPE_COMBOBOX:
         refreshItemsCombobox( static_cast<TCBListControl *>( aMagnet ) );
         break;
      case CB_TYPE_GRID:
         updateGridHeaders( static_cast<TCBGrid *>(aMagnet), NULL );
         updateGridAllRows( static_cast<TCBGrid *>(aMagnet), NULL );
         break;
      case CB_TYPE_TREE:
         refreshTree( static_cast<TCBTree *>(aMagnet) );
         break;
   }

   if ( aMagnet->isActuallyVisible() ) {
      if ( aMagnet->iRemoteType == CB_TYPE_NOTEBOOKTAB ) {
         TCBCollector *parent = aMagnet->parent.get();

         RECT pos;
         pos.left = 0;
         pos.top = 0;
         pos.right = parent->w;
         pos.bottom = parent->h;
         SendMessage( reinterpret_cast<HWND>(parent->iRemoteId), TCM_ADJUSTRECT, (WPARAM)FALSE, (LPARAM)&pos );
         SetWindowPos( h, HWND_TOP, pos.left, pos.top, pos.right - pos.left, pos.bottom - pos.top, 0 );
      }

	  if (bHasCustomFont) {
		  SendMessage( h, WM_SETFONT, reinterpret_cast<WPARAM>( f->iRemoteId ), FALSE );
	  } else {
		  if ( aMagnet->monospacefont.get() ) {
			 SendMessage( h, WM_SETFONT, reinterpret_cast<WPARAM>( this->hMonospaceFont ), FALSE );
		  } else {
			 SendMessage( h, WM_SETFONT, reinterpret_cast<WPARAM>( this->hCurrentFont ), FALSE );
		  }
	  }
      ShowWindow( h, SW_NORMAL );

      if ( aMagnet->iRemoteType == CB_TYPE_NOTEBOOKTAB ) {
         RedrawWindow( h, NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW );
      }
   }

   lGeneralAccess.unlock();
}

void TCBDriverWin32::refreshItemsListbox( TCBListControl *aMagnet ) {
   if ( aMagnet->iRemoteId != NULL ) {
      HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );
      SendMessage( h, LB_RESETCONTENT, 0, 0 );

      unsigned int c = aMagnet->count();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBListControlItem *item = aMagnet->getItem( i );

         String *sCopy = new String( item->value.asString() );
         sCopy->transformToWidestring( true );
         SendMessageW( h, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>( sCopy->getValue() ) );
         delete sCopy;
      }

      aMagnet->selected.set( aMagnet->selected.internalGet() );
   }
}

void TCBDriverWin32::refreshItemsCombobox( TCBListControl *aMagnet ) {
   if ( aMagnet->iRemoteId != NULL ) {
      HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

      SendMessage( h, CB_RESETCONTENT, 0, 0 );

      unsigned int c = aMagnet->count();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBListControlItem *item = aMagnet->getItem( i );

         String *sCopy = new String( item->value.asString() );
         sCopy->transformToWidestring( true );
         SendMessageW( h, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>( sCopy->getValue() ) );
         delete sCopy;
      }

      aMagnet->selected.set( aMagnet->selected.internalGet() );
   }
}

void TCBDriverWin32::buildTree( TCBTree *aMagnet, BNode *aParentNode, BNode *aChildNode ) {
   TVITEMW tvi;
   TVINSERTSTRUCTW tvins;
   HTREEITEM hParent = (HTREEITEM)TVI_ROOT;

   BAttribute *attrHndItem = NULL;
   if ( aParentNode != NULL ) {
      attrHndItem = aParentNode->getAttributeByName( "__win32customdata_treeitemhandle" );
      if ( attrHndItem != NULL ) {
         hParent = reinterpret_cast<HTREEITEM>( attrHndItem->value.asInteger() );
      }
   }

   String *sWCaption = new String( &aChildNode->name );
   if ( aMagnet->displayfield.getLength() > 0 ) {
      BAttribute *attr = aChildNode->getAttributeByName(aMagnet->displayfield.get());
      if ( attr != NULL ) {
         sWCaption->setValue( attr->value.asString() );
      }
   }
   sWCaption->transformToWidestring( true );

   tvi.mask = TVIF_TEXT;
   tvi.pszText    = sWCaption->getValueAsWide();
   tvi.cchTextMax = 0;
   tvins.item = tvi;
   tvins.hInsertAfter = NULL;
   tvins.hParent = hParent;

   hParent = (HTREEITEM)SendMessageW( reinterpret_cast<HWND>( aMagnet->iRemoteId ), TVM_INSERTITEMW, 0, (LPARAM)(LPTVINSERTSTRUCTW)&tvins );

   delete sWCaption;

   attrHndItem = aChildNode->findOrCreateAttribute( "__win32customdata_treeitemhandle" );
   attrHndItem->value.setInteger( reinterpret_cast<unsigned long>( hParent ) );
   attrHndItem->hideFromXML = true;

   unsigned int c = aChildNode->childCount();
   if ( c > 0 ) {
      for ( unsigned int i = 0; i < c; i++ ) {
         BNode *child = aChildNode->getChild(i);
         if ( child != NULL ) {
            buildTree( aMagnet, aChildNode, child );
         }
      }
   }
}

void TCBDriverWin32::refreshTree( TCBTree *aMagnet ) {
   if ( aMagnet->iRemoteId != NULL ) {
      SendMessage( reinterpret_cast<HWND>( aMagnet->iRemoteId ), TVM_DELETEITEM, 0, 0 );

      BNode *data = aMagnet->data.internalGet();
      if ( data != NULL ) {
         buildTree( aMagnet, NULL, data );
      }
   }
}

void TCBDriverWin32::repositionWindow( TCBMagnet *aMagnet ) {
   HWND hwnd = reinterpret_cast<HWND>( aMagnet->iRemoteId );
	RECT size;

	int iXcorr = 0, iYcorr = 0;

	size.left	= aMagnet->x + iXcorr;
	size.top		= aMagnet->y + iYcorr;
	size.bottom	= size.top + aMagnet->h;
	size.right	= size.left + aMagnet->w;

	bool bHasMenu = false;
	if ( aMagnet->iRemoteType == CB_TYPE_WINDOW ) {
		bHasMenu = (static_cast<TCBWindow *>(aMagnet)->mainMenu != NULL);
	}

 	AdjustWindowRectEx( &size, GetWindowLong(hwnd, GWL_STYLE), bHasMenu, GetWindowLong(hwnd, GWL_EXSTYLE) );

	long w = size.right - size.left + 2;
	long h = size.bottom - size.top + 2;
   long x = aMagnet->x;
	long y = aMagnet->y;

   if ( x == CB_LOCATION_CENTEROFSCREEN ) {
      x = aMagnet->getScreen()->w / 2 - w / 2;
   }
   if ( y == CB_LOCATION_CENTEROFSCREEN ) {
      y = aMagnet->getScreen()->h / 2 - h / 2;
   }

	SetWindowPos( hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE );
}

void TCBDriverWin32::setinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aSourceValue ) {
   GFTRACE();
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   unsigned int c;
   int iTmp;
   TCBMenu *menu;
   TCBCollector *win;

   int iXcorr = 0, iYcorr = 0, iHcorr = 0;

   TCBMagnet *pParent = aMagnet->parent.get();
   if ( pParent != NULL ) {
      if ( pParent->iRemoteType == CB_TYPE_GROUPBOX ) {
         iYcorr = 15;
         iXcorr = 2;
      }
   }

   if ( aMagnet->iRemoteType == CB_TYPE_GROUPBOX ) {
       iHcorr = -5;
   }

   if ( aMagnet->iRemoteId != NULL ) {
      HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

      switch ( iInfoType ) {
         case CB_INFOTYPE_FOCUS:
            SetFocus( h );
            break;
         case CB_INFOTYPE_EDITTEXT:
            if ( aMagnet->iRemoteType == CB_TYPE_NOTEBOOKTAB ) {
            } else if ( aMagnet->iRemoteType == CB_TYPE_PANEL ) {
            } else {
               String *sTmp = new String( aSourceValue->asString() );
               sTmp->transformToWidestring( true );
               SendMessageW( h, WM_SETTEXT, 0, reinterpret_cast<LPARAM>( sTmp->getValue() ) );
               delete sTmp;
            }
            break;
         case CB_INFOTYPE_CUTTEXT:
            SendMessageA( h, WM_CUT, 0, 0 );
            break;
         case CB_INFOTYPE_COPYTEXT:
            SendMessageA( h, WM_COPY, 0, 0 );
            break;
         case CB_INFOTYPE_PASTETEXT:
            SendMessageA( h, WM_PASTE, 0, 0 );
            break;
         case CB_INFOTYPE_SELECTALLTEXT:
            SendMessageA( h, EM_SETSEL, 0, -1 );
            break;
         case CB_INFOTYPE_LISTITEMSEL:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               SendMessage( h, LB_SETCURSEL, aSourceValue->asInteger(), 0);
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               SendMessage( h, CB_SETCURSEL, aSourceValue->asInteger(), 0);
            }
            break;
         case CB_INFOTYPE_LISTITEMADD:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               refreshItemsListbox( static_cast<TCBListControl *>( aMagnet ) );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               refreshItemsCombobox( static_cast<TCBListControl *>( aMagnet ) );
            }

            break;
         case CB_INFOTYPE_LISTITEMDEL:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               refreshItemsListbox( static_cast<TCBListControl *>( aMagnet ) );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               refreshItemsCombobox( static_cast<TCBListControl *>( aMagnet ) );
            }

            break;
         case CB_INFOTYPE_LISTITEMCLR:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               refreshItemsListbox( static_cast<TCBListControl *>( aMagnet ) );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               refreshItemsCombobox( static_cast<TCBListControl *>( aMagnet ) );
            }

            break;
         case CB_INFOTYPE_BARPOSITION:
            SendMessage( h, PBM_SETPOS, aSourceValue->asInteger(), 0);
            break;
         case CB_INFOTYPE_BUTTONSTATE:
            if ( aSourceValue->asBoolean() ) {
               SendMessage( h, BM_SETCHECK, BST_CHECKED, 0 );
            } else {
               SendMessage( h, BM_SETCHECK, BST_UNCHECKED, 0 );
            }
            break;
         case CB_INFOTYPE_ALIGNMENT:
            iTmp = WS_CHILD;
            switch ( static_cast<TCBLabel *>(aMagnet )->alignment.internalGet() ) {
               case CB_ALIGN_LEFT:
                  iTmp |= SS_LEFT;
                  break;
               case CB_ALIGN_CENTER:
                  iTmp |= SS_CENTER;
                  break;
               case CB_ALIGN_RIGHT:
                  iTmp |= SS_RIGHT;
                  break;
            }

            SetWindowLongPtr( h, GWL_STYLE, iTmp );
            break;
         case CB_INFOTYPE_GRIDFULLUPDATE:
            updateGridHeaders( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            updateGridAllRows( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            break;
         case CB_INFOTYPE_GRIDROWUPDATE:
            updateGridRow( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            break;
         case CB_INFOTYPE_GRIDROWADD:
            c = ListView_GetItemCount( h );
            if ( c == static_cast<TCBGrid *>(aMagnet)->getRowCount() ) {
               updateGridRow( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            } else {
               insertGridRow( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            }
            break;
         case CB_INFOTYPE_GRIDROWDELETE:
            c = ListView_GetItemCount( h );
            if ( c == static_cast<TCBGrid *>(aMagnet)->getRowCount() ) {
               updateGridRow( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            } else {
               SendMessage( h, LVM_DELETEITEM, aSourceValue->asInteger(), 0 );
            }
            break;
         case CB_INFOTYPE_GRIDCLEAR:
            SendMessage( h, LVM_DELETEALLITEMS, 0, 0 );
            break;
         case CB_INFOTYPE_GRIDROWSELECT:
            selectItemGrid( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            break;
         case CB_INFOTYPE_GRIDHEADERSUPDATE:
            updateGridHeaders( static_cast<TCBGrid *>(aMagnet), aSourceValue );
            break;
         case CB_INFOTYPE_MENUCHECKED:
            menu = static_cast<TCBMenu *>( aMagnet->parent.get() );
            if ( menu != NULL ) {
               if ( aSourceValue->asBoolean() ) {
                  CheckMenuItem( reinterpret_cast<HMENU>( menu->iRemoteId ), reinterpret_cast<UINT>( aMagnet->iRemoteId ), MF_BYCOMMAND | MF_CHECKED );
               } else {
                  CheckMenuItem( reinterpret_cast<HMENU>( menu->iRemoteId ), reinterpret_cast<UINT>( aMagnet->iRemoteId ), MF_BYCOMMAND | MF_UNCHECKED );
               }

	           win = menu->getNonMenuParent();
			}

            if ( win != NULL ) {
               DrawMenuBar( reinterpret_cast<HWND>( win->iRemoteId ) );
            }
            break;
         case CB_INFOTYPE_SETCONTEXTMENU:
            break;
         case CB_INFOTYPE_NOTEBOOKTABSELECT:
            SendMessage( h, TCM_SETCURSEL, aSourceValue->asInteger(), 0 );
            break;
         case CB_INFOTYPE_TREEFULLUPDATE:
            refreshTree( static_cast<TCBTree *>( aMagnet ) );
            break;
         case CB_INFOTYPE_BARSETUNDETERMINED:
            if ( aSourceValue->asBoolean() ) {
               SetWindowLongPtr( h, GWL_STYLE, GetWindowLongPtr( h, GWL_STYLE ) | PBS_MARQUEE );
               SendMessage( h, PBM_MARQUEE, TRUE, 10 );
            } else {
               SendMessage( h, PBM_MARQUEE, FALSE, 10 );
               SetWindowLongPtr( h, GWL_STYLE, GetWindowLongPtr( h, GWL_STYLE ) ^ PBS_MARQUEE );
            }
            break;
        case CB_INFOTYPE_REPOSITION:
            if ( aMagnet->iRemoteType == CB_TYPE_WINDOW ) {
               repositionWindow( aMagnet );
            } else {
               SetWindowPos( h, NULL, aMagnet->x + iXcorr, aMagnet->y + iYcorr, aMagnet->w, aMagnet->h + iHcorr, SWP_NOZORDER | SWP_NOACTIVATE );
            }
            break;
         default:
            break;
      }
   }

   lGeneralAccess.unlock();
}


DWORD CALLBACK RichEditOut( DWORD cookie, LPBYTE buf, LONG BytesToWrite, LONG *bytesWritten ) {
   String *sOut = reinterpret_cast<String *>( cookie );

   sOut->append( reinterpret_cast<const char *>(buf), BytesToWrite );

   *bytesWritten = BytesToWrite;

   return 0;
}


BNode *TCBDriverWin32::getSelectedItemTreeView( TCBMagnet *aMagnet ) {
    BNode *selectedNode = NULL;

    if ( aMagnet->iRemoteId != NULL ) {
        HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

        HTREEITEM hItem = reinterpret_cast<HTREEITEM>( SendMessage( h, TVM_GETNEXTITEM, TVGN_CARET, 0 ) );

        BNode *rootNode = static_cast<TCBTree *>( aMagnet )->data.internalGet();
        if ( rootNode != NULL ) {
            BValue *searchValue = new BValue();
            searchValue->setInteger( reinterpret_cast<unsigned long>( hItem ) );
            selectedNode = rootNode->findAttributeThroughRecursiveSearch( "__win32customdata_treeitemhandle", searchValue );
            delete searchValue;
        }
    }

    return selectedNode;
}


void TCBDriverWin32::getinfo_magnet( TCBMagnet *aMagnet, int iInfoType, BValue *aDestValue ) {
   GFTRACE();
   lGeneralAccess.lockWhenAvailable( GFLOCK_INFINITEWAIT );

   unsigned int maxlen;

   if ( aMagnet->iRemoteId != NULL ) {
      HWND h = reinterpret_cast<HWND>( aMagnet->iRemoteId );

      switch ( iInfoType ) {
         case CB_INFOTYPE_EDITTEXT:
            if ( aMagnet->iRemoteType == CB_TYPE_NOTEBOOKTAB ) {
            } if ( aMagnet->iRemoteType == CB_TYPE_MULTILINE ) {
               String *sTmp = new String();
               sTmp->setLength(0);

               EDITSTREAM outRichEditStream;
               outRichEditStream.dwCookie = reinterpret_cast<DWORD_PTR>( sTmp );
               outRichEditStream.dwError = 0;
               outRichEditStream.pfnCallback = RichEditOut;

               long total;
               maxlen = SendMessageW( h, (UINT)WM_GETTEXTLENGTH, 0, 0 );
               if ( iRichEditVersion == 10 ) {
                  // unicode not supported
                  sTmp->setSize( maxlen );
                  total = SendMessage( h, EM_STREAMOUT, SF_TEXT, (LPARAM)&outRichEditStream );

                  sTmp->transformToWidestring( false );
               } else {
                  // unicode supported
                  maxlen *= sizeof(wchar_t);
                  sTmp->setSize( maxlen );
                  total = SendMessage( h, EM_STREAMOUT, SF_TEXT | SF_UNICODE, (LPARAM)&outRichEditStream );
               }

               sTmp->transformWidestringToString( true );

               aDestValue->setString( sTmp );

               delete sTmp;
            } else {
               String *sTmp = new String();
               maxlen = SendMessageW( h, (UINT)WM_GETTEXTLENGTH, 0, 0 ) * sizeof(wchar_t);
               sTmp->setLength( maxlen );
               long total = SendMessageW( h, WM_GETTEXT, maxlen + sizeof(wchar_t), reinterpret_cast<LPARAM>( sTmp->getValue() ) );
               sTmp->setLength( total * sizeof(wchar_t) );
               sTmp->transformWidestringToString( true );
               aDestValue->setString( sTmp );
               delete sTmp;
            }
            break;
         case CB_INFOTYPE_LISTITEMSEL:
            if ( aMagnet->iRemoteType == CB_TYPE_LISTBOX ) {
               aDestValue->setInteger( SendMessage( h, LB_GETCURSEL, 0, 0) );
            } else if ( aMagnet->iRemoteType == CB_TYPE_COMBOBOX ) {
               aDestValue->setInteger( SendMessage( h, CB_GETCURSEL, 0, 0) );
            }
            break;
         case CB_INFOTYPE_BARPOSITION:
            aDestValue->setInteger( SendMessage( h, PBM_GETPOS, 0, 0) );
            break;
         case CB_INFOTYPE_BUTTONSTATE:
            aDestValue->setBoolean( SendMessage( h, BM_GETCHECK, 0, 0 ) == BST_CHECKED );
            break;
         case CB_INFOTYPE_GRIDROWSELECT:
            getSelectedItemGrid( static_cast<TCBGrid *>( aMagnet ), aDestValue );
            break;
         case CB_INFOTYPE_NOTEBOOKTABSELECT:
            aDestValue->setInteger( SendMessage( h, TCM_GETCURSEL, 0, 0 ) );
            break;
         case CB_INFOTYPE_TREESELECT:
            static_cast<TCBTree *>( aMagnet )->selected.internalSet( getSelectedItemTreeView(aMagnet) );
            break;
         default:
            break;
      }
   }

   lGeneralAccess.unlock();
}

long TCBDriverWin32::messagebox( TCBMagnet *aParent, String *sCaption, String *sText, long iType ) {
   GFTRACE();
   int iRet = 0;

   HWND hndParent = 0;
   if ( aParent != NULL ) {
      hndParent = reinterpret_cast<HWND>( aParent->iRemoteId );
   }

   String *wCaption = new String( sCaption );
   if ( bUnicodeSupported ) {
      wCaption->transformToWidestring( true );
   } else {
      wCaption->append_ansi( " unicode is being faked" );
   }

   String *wText = new String( sText );
   if ( bUnicodeSupported ) {
      wText->transformToWidestring( true );
   }

   long iMb = 0;
   switch ( iType ) {
      case CB_MSGBOX_OK:
         iMb = MB_OK;
         break;
      case CB_MSGBOX_OKCANCEL:
         iMb = MB_OKCANCEL;
         break;
      case CB_MSGBOX_YESNO:
         iMb = MB_YESNO;
         break;
      case CB_MSGBOX_YESNOCANCEL:
         iMb = MB_YESNOCANCEL;
         break;
   }

   if ( bUnicodeSupported ) {
      iRet = MessageBoxW( hndParent, wText->getValueAsWide(), wCaption->getValueAsWide(), iMb );
   } else {
      iRet = MessageBoxA( hndParent, wText->getValue(), wCaption->getValue(), iMb );
   }

   delete wCaption;
   delete wText;

   switch ( iRet ) {
      case IDOK:
         return CB_MSGBOX_OK;
      case IDCANCEL:
         return CB_MSGBOX_CANCEL;
      case IDYES:
         return CB_MSGBOX_YES;
      case IDNO:
         return CB_MSGBOX_NO;
   }

   return 0;
}

String *TCBDriverWin32::filedialog( TCBMagnet *aParent, String *sCaption, TCBFileDialogFilter *aFilter, String *sDefaultDirectory, String *sDefaultFileName, long iType ) {
    String sCaptionW, sDefDirW, sDefFileW;

    if ( sCaption != NULL ) {
        sCaptionW.setValue( sCaption );
        sCaptionW.transformToWidestring( true );
    }

    if ( sDefaultDirectory != NULL ) {
        sDefDirW.setValue( sDefaultDirectory );
        sDefDirW.transformToWidestring( true );
    }

    if ( sDefaultFileName != NULL ) {
        sDefFileW.setValue( sDefaultFileName );
        sDefFileW.transformToWidestring( true );
    }

    long iBufferSize = 2048;
    while ( !sDefFileW.setLength( iBufferSize * sizeof(wchar_t) ) ) {
        iBufferSize /= 2;
    }


    OPENFILENAMEW ofn;

/*
    WCHAR *szFilter = L"Graphics Data Files (*.dat)\0"
    TEXT("*.dat\0")
    TEXT("All Files (*.*)\0")
    TEXT("*.*\0");
*/

    String sFilterW;
    if ( aFilter != NULL ) {
        unsigned int c = aFilter->size();
        for ( unsigned int i = 0; i < c; i++ ) {
            TCBFileDialogFilterItem *item = aFilter->getItem(i);

            String w;
            w.setValue( &item->caption );
            w.transformToWidestring( true );

            sFilterW.append( &w );
            sFilterW.setLength( sFilterW.getLength() + sizeof(wchar_t) );

            unsigned int d = item->size();
            for ( unsigned int j = 0; j < d; j++ ) {
                w.setValue( item->getPattern(j) );
                if ( (j + 1) < d ) {
                    w.append_ansi( ";" );
                }
                w.transformToWidestring( true );

                sFilterW.append( &w );
            }

            sFilterW.setLength( sFilterW.getLength() + sizeof(wchar_t) );
        }
    }

    nullifyMem( &ofn, sizeof(OPENFILENAMEW) );
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    //ofn.lpfnHook =
    ofn.lpstrFilter = sFilterW.getValueAsWide();
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = sDefFileW.getValueAsWide();
    ofn.nMaxFile = iBufferSize;
    if ( sDefaultDirectory != NULL ) {
        ofn.lpstrInitialDir = sDefDirW.getValueAsWide();
    } else {
        ofn.lpstrInitialDir = NULL;
    }
    if ( sCaption != NULL ) {
        ofn.lpstrTitle = sCaptionW.getValueAsWide();
    } else {
        ofn.lpstrTitle = NULL;
    }
    ofn.nMaxFileTitle = sizeof(ofn.lpstrTitle);

    ofn.hwndOwner = NULL;

    if ( aParent != NULL ) {
        ofn.hwndOwner = reinterpret_cast<HWND>( aParent->iRemoteId );
    }

    ofn.Flags = OFN_EXPLORER;

    bool bOk = false;

    if ( iType == CB_FILEDIALOG_SAVE ) {
        bOk = (GetSaveFileNameW( &ofn ) == TRUE);
    } else {
        bOk = (GetOpenFileNameW( &ofn ) == TRUE);
    }

    if ( bOk ) {
        String *sFile = new String();
        sFile->setValue_wide( sDefFileW.getValueAsWide() );
        sFile->transformWidestringToString( true );

        return sFile;
    } else {
        DWORD error = CommDlgExtendedError();
        if ( error == FNERR_BUFFERTOOSMALL ) {
            printf( "Allocated buffer of %ld unicode characters was not enough for this path.\n", iBufferSize );
        }
    }

    return NULL;
}

void TCBDriverWin32::mainloop_iteration() {
   MSG message;

   if ( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ) {
      bool bIsDialogMsg = false;
      unsigned int c = windows->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         TCBWin32Window *window = static_cast<TCBWin32Window *>( windows->elementAt(i) );

         if ( hndAccelerator != NULL ) {
            bIsDialogMsg = (TranslateAccelerator( reinterpret_cast<HWND>( window->magnet->iRemoteId ), hndAccelerator, &message ) != 0);
            if ( bIsDialogMsg ) {
               break;
            }
         }

         bIsDialogMsg = (IsDialogMessage( reinterpret_cast<HWND>( window->magnet->iRemoteId ), &message ) == TRUE);
         if ( bIsDialogMsg ) {
            break;
         }
      }

      if ( !bIsDialogMsg ) {
         TranslateMessage(&message);
         DispatchMessage(&message);
      }
   }
}


#endif
