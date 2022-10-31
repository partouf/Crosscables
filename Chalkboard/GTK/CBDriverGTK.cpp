
#include "CBDriverGTK.h"

#ifndef GF_OS_MAC

#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/GarbageCollector.h>
#include <Groundfloor/Bookshelfs/BValue.h>


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifdef GF_OS_LINUX
#define C_GLADEFUNC extern "C"
#include <cstring>
#else
#define C_GLADEFUNC extern "C" __declspec(dllexport)
#endif

TCBMagnet *getMagnetByRemoteId(TCBCollector *obj, void *id);

C_GLADEFUNC void CBGTK_DeleteEvent(GtkWidget *widget, gpointer data);
C_GLADEFUNC void CBGTK_ClickEvent(GtkWidget *widget, gpointer data);
C_GLADEFUNC void CBGTK_MenuClickEvent(GtkMenuItem *menuitem, gpointer data);
C_GLADEFUNC void CBGTK_ChangeEvent(GtkWidget *widget, gpointer data);
C_GLADEFUNC void CBGTK_RowActivatedEvent(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);

C_GLADEFUNC gboolean CBGTK_PopupEvent(GtkWidget *widget, gpointer data);

C_GLADEFUNC void CBGTK_SetDefaultFont(GtkWidget *widget, gpointer data);

C_GLADEFUNC gboolean CBGTK_ButtonReleaseEvent(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
C_GLADEFUNC void CBGTK_SizeRequestEvent(GtkWidget *widget, GtkRequisition *requisition, gpointer user_data);
C_GLADEFUNC gboolean CBGTK_ConfigureEvent(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data);


void globalGtkLock_Lock() {
}

void globalGtkLock_Unlock() {
}


TCBDriverGTK *CBGlobalDriver = NULL;


C_GLADEFUNC void CBGTK_SetDefaultFont(GtkWidget *widget, gpointer data) {
   GFTRACE();
   PangoFontDescription *pFont = reinterpret_cast<PangoFontDescription *>(data);

   GtkStyle *style = gtk_widget_get_style(widget);
   pango_font_description_merge(style->font_desc, pFont, true);
}

//------------------------------------------------------------------------------

void CBGTKInitialize(TCBDriverGTK *pDriver) {
   GFTRACE();
   CBGlobalDriver = pDriver;

   gdk_threads_set_lock_functions(globalGtkLock_Lock, globalGtkLock_Unlock);

   //   g_thread_init( NULL );
   //   gdk_threads_init();

   gtk_init(0, 0);
}

void CBGTKFinalize() {
   GFTRACE();
   CBGlobalDriver = NULL;
}

//------------------------------------------------------------------------------
void stretchChildren(TCBMagnet *aMagnet);
void stretchMagnet(TCBMagnet *aMagnet);

void stretchMagnet(TCBMagnet *aMagnet) {
   TCBGTKWindow *w = NULL;
   if (aMagnet != NULL) {
      GtkWidget *wid = reinterpret_cast<GtkWidget *>(aMagnet->iRemoteId);
      if (wid != NULL) {
         if (aMagnet->iRemoteType != CB_TYPE_MENU) {
            if ((aMagnet->w <= 0) || (aMagnet->h <= 0) || (aMagnet->x < 0) || (aMagnet->y < 0)) {
               TCBCollector *parent = aMagnet->parent.get();
               if (parent != NULL) {
                  GtkWidget *widParent = NULL;
                  widParent = reinterpret_cast<GtkWidget *>(parent->iRemoteId);
                  if (widParent != NULL) {
                     long width, height;

                     width = aMagnet->w;
                     height = aMagnet->h;

                     int x = aMagnet->x;
                     int y = aMagnet->y;


                     GtkRequisition size;

                     if (parent->magic != NULL) {
                        TCBPoint *pt = reinterpret_cast<TCBPoint *>(parent->magic);

                        size.width = pt->x;
                        size.height = pt->y;
                     }
                     else {
                        gtk_widget_size_request(widParent, &size);
                     }


                     if (x < 0) {
                        x += size.width;
                     }
                     if (y < 0) {
                        y += size.height;
                     }

                     if (width <= 0) {
                        width = size.width - x + width;
                     }
                     if (height <= 0) {
                        height = size.height - y + height;
                     }

                     if (aMagnet->iRemoteType >= 100) {
                        guint borderwidth = gtk_container_get_border_width(GTK_CONTAINER(wid));
                        width -= borderwidth * 2;
                        height -= borderwidth * 2;
                     }

                     if (parent->iRemoteType == CB_TYPE_WINDOW) {
                        TCBMenu *menu = static_cast<TCBWindow *>(parent)->mainMenu;
                        if (menu != NULL) {
                           if (menu->iRemoteId != 0) {
                              gtk_widget_size_request(GTK_WIDGET(menu->iRemoteId), &size);

                              height -= size.height;
                           }
                        }
                        height -= 5;
                     }

                     if ((aMagnet->x < 0) || (aMagnet->y < 0)) {
                        w = CBGlobalDriver->getGTKWindowObject(parent);
                        if (w != NULL) {
                           gtk_fixed_move(GTK_FIXED(w->widget), wid, x, y);
                        }
                     }

                     gtk_widget_set_size_request(wid, width, height);

                     stretchChildren(aMagnet);
                  }
               }
            }
         }
      }
   }
}

void stretchChildren(TCBMagnet *aMagnet) {
   if (aMagnet != NULL) {
      if (aMagnet->iRemoteType >= 100) {
         TCBCollector *col = static_cast<TCBCollector *>(aMagnet);
         Vector *controls = col->getVector();
         unsigned int c = controls->size();
         for (unsigned int i = 0; i < c; i++) {
            stretchMagnet(static_cast<TCBMagnet *>(controls->elementAt(i)));
         }
      }
   }
}

//------------------------------------------------------------------------------


C_GLADEFUNC void CBGTK_DeleteEvent(GtkWidget *widget, gpointer data) {
   GFTRACE();

   TCBMagnet *magnet = reinterpret_cast<TCBMagnet *>(data);
   magnet->onHide.execute(magnet);
}

C_GLADEFUNC void CBGTK_MenuClickEvent(GtkMenuItem *widget, gpointer data) {
   bool bOldCheck, bNewCheck;
   TCBMagnet *magnet = reinterpret_cast<TCBMagnet *>(data);

   bOldCheck = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(magnet->iRemoteId));
   bNewCheck = static_cast<TCBMenu *>(magnet)->checked.get();

   if (bOldCheck != bNewCheck) {
      gtk_signal_handler_block_by_data(GTK_OBJECT(widget), data);
      gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(widget), static_cast<TCBMenu *>(magnet)->checked.get());
      gtk_signal_handler_unblock_by_data(GTK_OBJECT(widget), data);
   }

   magnet->onClick.execute(magnet);
}

C_GLADEFUNC void CBGTK_ClickEvent(GtkWidget *widget, gpointer data) {
   GFTRACE();
   TCBMagnet *magnet = NULL;

   magnet = reinterpret_cast<TCBMagnet *>(data);

   // avoid default radiogroup behaviour concerning click events for every radiobutton in the group
   if (magnet->iRemoteType == CB_TYPE_RADIOBUTTON) {
      if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(magnet->iRemoteId))) {
         return;
      }
   }

   magnet->onClick.execute(magnet);
}

C_GLADEFUNC void CBGTK_ChangeEvent(GtkWidget *widget, gpointer data) {
   GFTRACE();
   TCBMagnet *magnet = NULL;

   magnet = reinterpret_cast<TCBMagnet *>(data);
   magnet->onChange.execute(magnet);
}

C_GLADEFUNC gboolean CBGTK_ButtonReleaseEvent(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
   if (event->button == 3) {
      return CBGTK_PopupEvent(widget, user_data);
   }

   return false;
}

C_GLADEFUNC void CBGTK_RowActivatedEvent(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data) {
   GFTRACE();

   printf("row activated\n");
}

C_GLADEFUNC gboolean CBGTK_PopupEvent(GtkWidget *widget, gpointer data) {
   GFTRACE();

   TCBMagnet *magnet = NULL;
   TCBMenu *menu = NULL;

   magnet = reinterpret_cast<TCBMagnet *>(data);
   if ((magnet->iRemoteType == CB_TYPE_LISTBOX) ||
      (magnet->iRemoteType == CB_TYPE_GRID) ||
      (magnet->iRemoteType == CB_TYPE_MULTILINE)) {
      if (static_cast<TCBMagnetWithContextMenu *>(magnet)->shouldShowContextMenu.internalGet()) {
         menu = static_cast<TCBMagnetWithContextMenu *>(magnet)->contextmenu.internalGet();
      }
      else {
         if (static_cast<TCBMagnetWithContextMenu *>(magnet)->shouldShowDefaultMenu.internalGet()) {
            menu = static_cast<TCBMagnetWithContextMenu *>(magnet)->defaultmenu.internalGet();
         }
      }
   }

   if (menu != NULL) {
      if (menu->iRemoteId == NULL) {
         CBGlobalDriver->buildMenu(true, menu);
      }

      gtk_menu_popup(GTK_MENU(menu->iRemoteId), NULL, 0, 0, 0, 0, gtk_get_current_event_time());

      return true;
   }

   return false;
}


C_GLADEFUNC void CBGTK_SizeRequestEvent(GtkWidget *widget, GtkRequisition *requisition, gpointer user_data) {
   GFTRACE();

   TCBMagnet *magnet = reinterpret_cast<TCBMagnet *>(user_data);
   if (magnet != NULL) {
      if (magnet->iRemoteType == CB_TYPE_WINDOW) {
         stretchChildren(magnet);
      }
   }
}

C_GLADEFUNC gboolean CBGTK_ConfigureEvent(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data) {
   GFTRACE();

   TCBMagnet *magnet = reinterpret_cast<TCBMagnet *>(user_data);
   if (magnet != NULL) {
      if (magnet->iRemoteType == CB_TYPE_WINDOW) {
         // printf("CBGTK_ConfigureEvent\n");

         // resize is nog niet toegepast voor getSize_NSView(), dus moeten we het zelf onthouden
         TCBPoint *pt = new TCBPoint(event->width, event->height);
         magnet->magic = reinterpret_cast<void *>(pt);

         stretchChildren(magnet);

         magnet->magic = NULL;
         delete pt;

         //gtk_widget_set_size_request( widget, event->width, event->height );
      }
   }

   return FALSE;
}


//------------------------------------------------------------------------------


TCBMagnet *getMagnetByRemoteId(TCBCollector *obj, void *id) {
   GFTRACE();
   Vector *list = obj->getVector();
   unsigned int c = list->size();
   for (unsigned int i = 0; i < c; i++) {
      TCBMagnet *magnet = static_cast<TCBMagnet *>(list->elementAt(i));
      if (magnet->iRemoteId == id) {
         return magnet;
      }

      // if collector -> recursie...
      if (magnet->iRemoteType > 100) {
         magnet = getMagnetByRemoteId(static_cast<TCBCollector *>(magnet), id);
         if (magnet != NULL) {
            return magnet;
         }
      }
   }

   return NULL;
}


TCBDriverGTK::TCBDriverGTK(const char *sFontName, int iFontSize) : TCBDriver::TCBDriver() {
   GFTRACE();
   //   this->hApp     = hApplication;
   this->windows = new Vector();

   this->sCurrentFontName.setValue(sFontName, strlen(sFontName));

#ifdef GF_OS_LINUX
   this->iCurrentFontSize = iFontSize + 2;
#else
   this->iCurrentFontSize = iFontSize + 2;
#endif

   BValue *val = new BValue();
   val->setInteger(iCurrentFontSize);
   val->asString()->append("px", 2);
   val->asString()->prepend(" ", 1);
   val->asString()->prepend(&sCurrentFontName);

   pDefaultFont = pango_font_description_from_string(val->asString()->getValue());

   val->setInteger(iCurrentFontSize);
   val->asString()->append("px", 2);
   val->asString()->prepend(" ", 1);
   val->asString()->prepend_ansi("Courier");

   pMonospaceFont = pango_font_description_from_string(val->asString()->getValue());

   delete val;
}

TCBDriverGTK::~TCBDriverGTK() {
   GFTRACE();
   delete windows;
}

void TCBDriverGTK::initialize(TCBScreen *aScreen) {
   GFTRACE();
   TCBDriver::initialize(aScreen);

}

void TCBDriverGTK::finalize() {
   GFTRACE();

   TCBDriver::finalize();
}

void TCBDriverGTK::preparefont(TCBFont *font) {
   // todo: implement

   /*

   we do this in Win32, 

   int size = -MulDiv(font->size.get(), GetDeviceCaps(GetDC(NULL), LOGPIXELSY), 72);
   HANDLE h = CreateFontA( size, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, font->name.get() );

   font->iRemoteId = h;
   
   */
}


TCBGTKWindow *TCBDriverGTK::getGTKWindowObject(TCBMagnet *aMagnet) {
   GFTRACE();
   TCBGTKWindow *window = NULL;

   unsigned int c = windows->size();
   for (unsigned int i = 0; i < c; i++) {
      TCBGTKWindow *tmp = static_cast<TCBGTKWindow *>(windows->elementAt(i));
      if (tmp->magnet == aMagnet) {
         window = tmp;

         break;
      }
   }

   return window;
}

void TCBDriverGTK::addMenuItem(TCBMenu *aParentMenu, TCBMenu *aMenu) {
   String sCaption;
   sCaption.setValue(aMenu->caption.link());

   GtkWidget *pMenuLabel;
   if (sCaption.match_ansi("-")) {
      pMenuLabel = gtk_separator_menu_item_new();
      gtk_widget_show(pMenuLabel);

      gtk_menu_shell_append(GTK_MENU_SHELL(aParentMenu->magic), pMenuLabel);
      aMenu->iRemoteId = reinterpret_cast<void *>(pMenuLabel);
   }
   else {
      Vector *list = aMenu->getVector();
      unsigned int c = list->size();

      GtkWidget *pMenu;
      if (c > 0) {
         pMenuLabel = gtk_menu_item_new_with_label(sCaption.getValue());

         pMenu = gtk_menu_new();
         gtk_menu_item_set_submenu(GTK_MENU_ITEM(pMenuLabel), pMenu);
         aMenu->magic = reinterpret_cast<void *>(pMenu);
      }
      else {
         pMenuLabel = gtk_check_menu_item_new_with_label(sCaption.getValue());
         gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(pMenuLabel), aMenu->checked.get());

         gtk_signal_connect(GTK_OBJECT(pMenuLabel), "activate", GTK_SIGNAL_FUNC(CBGTK_MenuClickEvent), aMenu);
      }

      gtk_widget_show(pMenuLabel);

      gtk_menu_shell_append(GTK_MENU_SHELL(aParentMenu->magic), pMenuLabel);
      aMenu->iRemoteId = reinterpret_cast<void *>(pMenuLabel);

      for (unsigned int i = 0; i < c; i++) {
         addMenuItem(aMenu, static_cast<TCBMenu *>(list->elementAt(i)));
      }
   }
}

void TCBDriverGTK::buildMenu(bool popup, TCBMenu *aMenu) {
   if (aMenu != NULL) {
      GtkWidget *pMenu;
      if (popup) {
         pMenu = gtk_menu_new();
      }
      else {
         pMenu = gtk_menu_bar_new();
      }
      gtk_widget_show(pMenu);
      aMenu->iRemoteId = reinterpret_cast<void *>(pMenu);
      aMenu->magic = reinterpret_cast<void *>(pMenu);

      Vector *list = aMenu->getVector();
      unsigned int c = list->size();
      for (unsigned int i = 0; i < c; i++) {
         addMenuItem(aMenu, static_cast<TCBMenu *>(list->elementAt(i)));
      }
   }
}

void TCBDriverGTK::create_magnet(TCBMagnet *aMagnet) {
   GFTRACE();
   TCBGTKWindow *w;
   GtkWidget *widget = NULL;
   GtkWidget *layout = NULL;
   GtkTextBuffer *aBuffer;
   TCBCollector *parent;
   bool bIsBin = false;
   GtkListStore *lbStore;
   GtkTreeStore *trStore;
   GtkTreeViewColumn *column;
   GtkCellRenderer *renderer;
   GtkWidget *vbox;

   GtkTreeSelection *selection;

   GdkScreen *screen;
   GdkRectangle screenrect;

   int width = max(0, aMagnet->w);
   int height = max(0, aMagnet->h);
   int iNewX = aMagnet->x;
   int iNewY = aMagnet->y;

   switch (aMagnet->iRemoteType) {
   case CB_TYPE_SCREEN:
      screen = gdk_screen_get_default();
      gdk_screen_get_monitor_geometry(screen, 0, &screenrect);
      aMagnet->w = screenrect.width;
      aMagnet->h = screenrect.height;
      break;
   case CB_TYPE_WINDOW:
      widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      vbox = gtk_vbox_new(FALSE, 0);
      layout = gtk_fixed_new();

      gtk_container_add(GTK_CONTAINER(widget), vbox);

      if (static_cast<TCBWindow *>(aMagnet)->mainMenu != NULL) {
         buildMenu(false, static_cast<TCBWindow *>(aMagnet)->mainMenu);
         gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(static_cast<TCBWindow *>(aMagnet)->mainMenu->iRemoteId), FALSE, TRUE, 0);
      }

      gtk_container_add(GTK_CONTAINER(vbox), layout);
      gtk_widget_show(layout);
      gtk_widget_show(vbox);

      gtk_widget_set_size_request(widget, width, height);

      if (iNewX == CB_LOCATION_CENTEROFSCREEN) {
         iNewX = aMagnet->getScreen()->w / 2 - width / 2;
      }
      if (iNewY == CB_LOCATION_CENTEROFSCREEN) {
         iNewY = aMagnet->getScreen()->h / 2 - height / 2;
      }
      gtk_window_move(GTK_WINDOW(widget), iNewX, iNewY);

      gtk_window_set_title(GTK_WINDOW(widget), aMagnet->caption.internalGet());

      w = new TCBGTKWindow();
      w->magnet = static_cast<TCBCollector *>(aMagnet);
      w->widget = layout;
      windows->addElement(w);

      gtk_window_set_policy(GTK_WINDOW(widget), true, true, true);

      g_signal_connect(G_OBJECT(widget), "hide", G_CALLBACK(CBGTK_DeleteEvent), aMagnet);
      //         g_signal_connect( G_OBJECT( widget ), "size-request", G_CALLBACK( CBGTK_SizeRequestEvent ), aMagnet );
      g_signal_connect(G_OBJECT(widget), "configure-event", G_CALLBACK(CBGTK_ConfigureEvent), aMagnet);
      g_signal_connect(G_OBJECT(widget), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), aMagnet);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_GROUPBOX:
      if (strlen(aMagnet->caption.internalGet()) != 0) {
         widget = gtk_frame_new(aMagnet->caption.internalGet());
      }
      else {
         widget = gtk_frame_new(NULL);
      }
      layout = gtk_fixed_new();
      gtk_container_add(GTK_CONTAINER(widget), layout);
      gtk_widget_show(layout);

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.get()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      w = new TCBGTKWindow();
      w->magnet = static_cast<TCBCollector *>(aMagnet);
      w->widget = layout;
      windows->addElement(w);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_PANEL:
      widget = gtk_frame_new(NULL);
      layout = gtk_fixed_new();
      gtk_container_add(GTK_CONTAINER(widget), layout);
      gtk_widget_show(layout);

      gtk_frame_set_shadow_type(GTK_FRAME(widget), GTK_SHADOW_NONE);

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.get()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      w = new TCBGTKWindow();
      w->magnet = static_cast<TCBCollector *>(aMagnet);
      w->widget = layout;
      windows->addElement(w);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_LABEL:
      widget = gtk_label_new(aMagnet->caption.internalGet());
      gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_LEFT);

      switch (static_cast<TCBLabel *>(aMagnet)->alignment.internalGet()) {
      case CB_ALIGN_LEFT:
         gtk_misc_set_alignment(GTK_MISC(widget), 0, 0.5);
         break;
      case CB_ALIGN_CENTER:
         gtk_misc_set_alignment(GTK_MISC(widget), 0.5, 0.5);
         break;
      case CB_ALIGN_RIGHT:
         gtk_misc_set_alignment(GTK_MISC(widget), 1, 0.5);
         break;
      }

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_EDITBOX:
      widget = gtk_entry_new();

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);
      gtk_entry_set_text(GTK_ENTRY(widget), aMagnet->caption.internalGet());

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_MULTILINE:
      widget = gtk_text_view_new();
      layout = gtk_scrolled_window_new(NULL, NULL);

      gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(layout), GTK_SHADOW_ETCHED_IN);

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), layout);
         gtk_fixed_move(GTK_FIXED(w->widget), layout, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), layout);
      }

      //         gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(layout), widget );
      gtk_container_add(GTK_CONTAINER(layout), widget);
      gtk_widget_show(widget);

      gtk_widget_set_size_request(widget, width, height);

      aBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));
      gtk_text_buffer_set_text(aBuffer, aMagnet->caption.internalGet(), strlen(aMagnet->caption.internalGet()));

      aMagnet->magic = reinterpret_cast<void *>(widget);
      aMagnet->iRemoteId = reinterpret_cast<void *>(layout);
      break;
   case CB_TYPE_LISTBOX:
      lbStore = gtk_list_store_new(1, G_TYPE_STRING);
      widget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(lbStore));
      layout = gtk_scrolled_window_new(NULL, NULL);

      gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(layout), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
      gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(layout), GTK_SHADOW_ETCHED_IN);

      renderer = gtk_cell_renderer_text_new();
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(widget), 0, "item", renderer, "text", 0, NULL);
      column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), 0);

      gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(widget), false);

      gtk_container_add(GTK_CONTAINER(layout), widget);
      gtk_widget_show(widget);

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), layout);
         gtk_fixed_move(GTK_FIXED(w->widget), layout, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), layout);
      }

      gtk_widget_set_size_request(widget, width, height);

      g_signal_connect(G_OBJECT(widget), "row-activated", G_CALLBACK(CBGTK_RowActivatedEvent), aMagnet);
      g_signal_connect(G_OBJECT(widget), "popup-menu", G_CALLBACK(CBGTK_PopupEvent), aMagnet);
      g_signal_connect(G_OBJECT(widget), "button-release-event", G_CALLBACK(CBGTK_ButtonReleaseEvent), aMagnet);

      aMagnet->magic = reinterpret_cast<void *>(widget);
      aMagnet->iRemoteId = reinterpret_cast<void *>(layout);
      break;
   case CB_TYPE_COMBOBOX:
      widget = gtk_combo_box_new_text();

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      g_signal_connect(G_OBJECT(widget), "changed", G_CALLBACK(CBGTK_ClickEvent), aMagnet);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_BUTTON:
      widget = gtk_button_new();

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);
      gtk_button_set_label(GTK_BUTTON(widget), aMagnet->caption.internalGet());

      g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(CBGTK_ClickEvent), aMagnet);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);

      bIsBin = true;
      break;
   case CB_TYPE_CHECKBOX:
      widget = gtk_check_button_new_with_label(aMagnet->caption.get());

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(CBGTK_ClickEvent), aMagnet);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);

      bIsBin = true;
      break;
   case CB_TYPE_RADIOBUTTON:
      parent = aMagnet->parent.internalGet();
      if (parent->magic != NULL) {
         widget = gtk_radio_button_new_with_label(reinterpret_cast<GSList *>(parent->magic), aMagnet->caption.internalGet());
      }
      else {
         widget = gtk_radio_button_new_with_label(NULL, aMagnet->caption.internalGet());
         parent->magic = reinterpret_cast<void *>(gtk_radio_button_get_group(GTK_RADIO_BUTTON(widget)));
      }

      w = getGTKWindowObject(parent);
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(parent->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      g_signal_connect(G_OBJECT(widget), "clicked", G_CALLBACK(CBGTK_ClickEvent), aMagnet);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);

      bIsBin = true;
      break;
   case CB_TYPE_PROGRESSBAR:
      widget = gtk_progress_bar_new();

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widget), aMagnet->caption.internalGet());

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_GRID:
      widget = gtk_tree_view_new();
      layout = gtk_scrolled_window_new(NULL, NULL);

      gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(layout), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
      gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(layout), GTK_SHADOW_ETCHED_IN);
      gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(widget), true);

      gtk_container_add(GTK_CONTAINER(layout), widget);
      gtk_widget_show(widget);

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), layout);
         gtk_fixed_move(GTK_FIXED(w->widget), layout, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), layout);
      }

      gtk_widget_set_size_request(widget, width, height);

      g_signal_connect(G_OBJECT(widget), "row-activated", G_CALLBACK(CBGTK_RowActivatedEvent), aMagnet);
      g_signal_connect(G_OBJECT(widget), "popup-menu", G_CALLBACK(CBGTK_PopupEvent), aMagnet);
      g_signal_connect(G_OBJECT(widget), "button-release-event", G_CALLBACK(CBGTK_ButtonReleaseEvent), aMagnet);

      aMagnet->magic = reinterpret_cast<void *>(widget);
      aMagnet->iRemoteId = reinterpret_cast<void *>(layout);
      break;
   case CB_TYPE_NOTEBOOK:
      widget = gtk_notebook_new();

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.get()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_NOTEBOOKTAB:
      widget = gtk_frame_new(NULL);
      layout = gtk_fixed_new();
      gtk_container_add(GTK_CONTAINER(widget), layout);
      gtk_widget_show(layout);

      gtk_frame_set_shadow_type(GTK_FRAME(widget), GTK_SHADOW_NONE);

      parent = aMagnet->parent.internalGet();
      if (gtk_notebook_insert_page(GTK_NOTEBOOK(parent->iRemoteId), widget, NULL, parent->getVector()->findElement(aMagnet)) != -1) {
         gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(parent->iRemoteId), widget, aMagnet->caption.internalGet());
      }

      w = new TCBGTKWindow();
      w->magnet = static_cast<TCBCollector *>(aMagnet);
      w->widget = layout;
      windows->addElement(w);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   case CB_TYPE_MENU:
      break;
   case CB_TYPE_TREE:
      trStore = gtk_tree_store_new(1, G_TYPE_STRING);
      widget = gtk_tree_view_new_with_model(GTK_TREE_MODEL(trStore));
      layout = gtk_scrolled_window_new(NULL, NULL);

      gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(layout), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
      gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(layout), GTK_SHADOW_ETCHED_IN);

      renderer = gtk_cell_renderer_text_new();
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(widget), 0, "item", renderer, "text", 0, NULL);
      column = gtk_tree_view_get_column(GTK_TREE_VIEW(widget), 0);

      gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(widget), false);

      gtk_container_add(GTK_CONTAINER(layout), widget);
      gtk_widget_show(widget);

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), layout);
         gtk_fixed_move(GTK_FIXED(w->widget), layout, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), layout);
      }

      gtk_widget_set_size_request(widget, width, height);

      selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
      gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);

      g_signal_connect(G_OBJECT(widget), "row-activated", G_CALLBACK(CBGTK_RowActivatedEvent), aMagnet);
      g_signal_connect(G_OBJECT(widget), "popup-menu", G_CALLBACK(CBGTK_PopupEvent), aMagnet);
      g_signal_connect(G_OBJECT(widget), "button-release-event", G_CALLBACK(CBGTK_ButtonReleaseEvent), aMagnet);

      aMagnet->magic = reinterpret_cast<void *>(widget);
      aMagnet->iRemoteId = reinterpret_cast<void *>(layout);
      break;
   default:
      widget = gtk_label_new("Unknown Control");
      gtk_label_set_justify(GTK_LABEL(widget), GTK_JUSTIFY_LEFT);
      gtk_misc_set_alignment(GTK_MISC(widget), 0.5, 0.5);

      w = getGTKWindowObject(aMagnet->parent.internalGet());
      if (w != NULL) {
         gtk_container_add(GTK_CONTAINER(w->widget), widget);
         gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
      }
      else {
         gtk_container_add(GTK_CONTAINER(aMagnet->parent.internalGet()->iRemoteId), widget);
      }

      gtk_widget_set_size_request(widget, width, height);

      aMagnet->iRemoteId = reinterpret_cast<void *>(widget);
      break;
   }


   if (aMagnet->iRemoteId != 0) {
      stretchMagnet(aMagnet);
   }
   ///*
   // this code doesn't really work on all widgets, so rather not use them
   if (widget != NULL) {
      if (!aMagnet->monospacefont.get()) {
         gtk_widget_modify_font(widget, pDefaultFont);
      }
      else {
         gtk_widget_modify_font(widget, pMonospaceFont);
      }
      /*
      if ( bIsBin ) {
         if ( !aMagnet->monospacefont.get() ) {
            gtk_container_forall( GTK_CONTAINER( widget ), CBGTK_SetDefaultFont, pDefaultFont );
         } else {
            gtk_container_forall( GTK_CONTAINER( widget ), CBGTK_SetDefaultFont, pMonospaceFont );
         }
      }
      */
   }
   //*/
}

void TCBDriverGTK::destroy_magnet(TCBMagnet *aMagnet) {
   lGeneralAccess.lockWhenAvailable(GFLOCK_INFINITEWAIT);
   if (aMagnet->iRemoteId != NULL) {
   }
   lGeneralAccess.unlock();
}

void TCBDriverGTK::hide_magnet(TCBMagnet *aMagnet) {
   lGeneralAccess.lockWhenAvailable(GFLOCK_INFINITEWAIT);
   if (aMagnet->iRemoteId == NULL) {
      create_magnet(aMagnet);
   }

   if (aMagnet->iRemoteId != NULL) {
      GtkWidget *widget = reinterpret_cast<GtkWidget *>(aMagnet->iRemoteId);
      switch (aMagnet->iRemoteType) {
      default:
         gtk_widget_hide(widget);

         break;
      }
   }
   lGeneralAccess.unlock();
}

void TCBDriverGTK::show_magnet(TCBMagnet *aMagnet) {
   lGeneralAccess.lockWhenAvailable(GFLOCK_INFINITEWAIT);
   if (aMagnet->iRemoteId == NULL) {
      create_magnet(aMagnet);
   }

   if (aMagnet->iRemoteId != NULL) {
      GtkWidget *widget = reinterpret_cast<GtkWidget *>(aMagnet->iRemoteId);
      switch (aMagnet->iRemoteType) {
      case CB_TYPE_LISTBOX:
         refreshItemsListbox(static_cast<TCBListControl *>(aMagnet));
         break;
      case CB_TYPE_COMBOBOX:
         refreshItemsCombobox(static_cast<TCBListControl *>(aMagnet));
         break;
      case CB_TYPE_GRID:
         updateGridHeaders(static_cast<TCBGrid *>(aMagnet), NULL);
         updateGridAllRows(static_cast<TCBGrid *>(aMagnet), NULL);
         break;
      case CB_TYPE_TREE:
         refreshTree(static_cast<TCBTree *>(aMagnet));
         break;
      }

      gtk_widget_show(widget);
   }
   lGeneralAccess.unlock();
}

GtkListStore *TCBDriverGTK::getGridStore(TCBGrid *aMagnet) {
   GtkListStore *store = NULL;
   GtkTreeView *view = GTK_TREE_VIEW(aMagnet->magic);
   if (view != NULL) {

      store = GTK_LIST_STORE(gtk_tree_view_get_model(view));
      if (store == NULL) {
         BTable *table = aMagnet->data.get();
         if (table != NULL) {
            BFields *fields = table->fields.get();
            if (fields != NULL) {
               unsigned int cDataFields = fields->size();
               GFTRACE_D(cDataFields);

               GType *types = new GType[cDataFields];
               for (unsigned int i = 0; i < cDataFields; i++) {
                  types[i] = G_TYPE_STRING;
               }
               store = gtk_list_store_newv(cDataFields, types);
               delete types;

               gtk_tree_view_set_model(view, GTK_TREE_MODEL(store));
            }
         }
      }
   }

   return store;
}

bool TCBDriverGTK::updateGridHeaders(TCBGrid *aMagnet, BValue *aValue) {
   GFTRACE();

   GtkListStore *store = getGridStore(aMagnet);
   if (store != NULL) {
      GtkCellRenderer *renderer;
      GtkTreeViewColumn *col;
      GtkTreeView *view = GTK_TREE_VIEW(aMagnet->magic);
      unsigned int cFields = aMagnet->headers.size();

      GFTRACE();
      unsigned int iAlreadyExists = 0;
      col = gtk_tree_view_get_column(view, iAlreadyExists);
      while (col != NULL) {
         if (cFields <= iAlreadyExists) {
            GFTRACE();
            gtk_tree_view_remove_column(view, col);
         }
         else {
            GFTRACE();
            TCBGridHeader *header = aMagnet->headers.getHeader(iAlreadyExists);
            GList *listRend = gtk_tree_view_column_get_cell_renderers(col);
            renderer = GTK_CELL_RENDERER(listRend->data);
            g_list_free(listRend);

            gtk_tree_view_column_set_title(col, header->caption.get());
            gtk_tree_view_column_set_attributes(col, renderer, "text", header->column.get(), NULL);
            gtk_tree_view_column_set_fixed_width(col, header->width.get());
         }

         iAlreadyExists++;
         col = gtk_tree_view_get_column(view, iAlreadyExists);
      }

      for (unsigned int i = iAlreadyExists; i < cFields; i++) {
         GFTRACE_D(i);
         GFTRACE_D(cFields);
         TCBGridHeader *header = aMagnet->headers.getHeader(i);
         if (header != NULL) {
            gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), i, header->caption.get(), gtk_cell_renderer_text_new(), "text", header->column.get(), NULL);
            col = gtk_tree_view_get_column(view, i);
            gtk_tree_view_column_set_sizing(col, GTK_TREE_VIEW_COLUMN_FIXED);
            gtk_tree_view_column_set_fixed_width(col, header->width.get());
         }
      }
   }

   GFTRACE_S("end");
   return true;
}

bool TCBDriverGTK::updateGridAllRows(TCBGrid *aMagnet, BValue *aValue) {
   GFTRACE();

   GtkListStore *store = getGridStore(aMagnet);
   if (store != NULL) {

      BTable *table = aMagnet->data.get();
      //      GtkTreeView *view = GTK_TREE_VIEW( aMagnet->magic );
      if (table != NULL) {
         GtkTreeIter iter;

         BField *field;

         BFields *fields = table->fields.get();
         if (fields != NULL) {
            unsigned int cFields = fields->size();

            BRecords *records = table->records.get();
            if (records != NULL) {
               unsigned int cRows = records->size();
               GValue gValue = { 0,0 };
               BRecord *record;

               unsigned int j = 0;

               if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store), &iter)) {
                  while (true) {
                     if (j <= cRows) {
                        if (!gtk_list_store_remove(store, &iter)) {
                           break;
                        }
                     }
                     else {
                        record = records->getRecord(j);
                        if (record != NULL) {
                           for (unsigned int k = 0; k < cFields; k++) {
                              field = fields->getField(k);

                              g_value_init(&gValue, G_TYPE_STRING);
                              g_value_set_string(&gValue, record->getValue(k)->asString()->getValue());
                              gtk_list_store_set_value(store, &iter, k, &gValue);
                              g_value_unset(&gValue);
                           }

                           if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(store), &iter)) {
                              break;
                           }
                        }

                        j++;
                     }
                  }
               }

               for (; j < cRows; j++) {
                  record = records->getRecord(j);
                  if (record != NULL) {
                     GValue *values = new GValue[cFields];
                     gint *columns = new gint[cFields];
                     memset(values, 0, cFields * sizeof(GValue));
                     for (unsigned int k = 0; k < cFields; k++) {
                        field = fields->getField(k);
                        columns[k] = k;

                        g_value_init(&values[k], G_TYPE_STRING);
                        g_value_set_string(&values[k], record->getValue(k)->asString()->getValue());
                     }
                     gtk_list_store_insert_with_valuesv(store, &iter, j, columns, values, cFields);
                  }
               }


               aMagnet->selected.set(aMagnet->selected.internalGet());
               return true;
            }
         }
      }
   }

   return false;
}

bool TCBDriverGTK::updateGridRow(TCBGrid *aMagnet, BValue *aValue) {
   GFTRACE();

   GtkListStore *store = getGridStore(aMagnet);
   if (store != NULL) {

      BTable *table = aMagnet->data.get();
      //    GtkTreeView *view = GTK_TREE_VIEW( aMagnet->magic );
      int iRow = aValue->asInteger();
      if (table != NULL) {
         GtkTreeIter iter;

         BFields *fields = table->fields.get();
         unsigned int cFields = fields->size();
         BField *field;

         BRecords *records = table->records.get();
         if (records != NULL) {
            BRecord *record = records->getRecord(iRow);
            GValue gValue = { 0,0 };

            if (record != NULL) {
               if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, iRow)) {
                  for (unsigned int k = 0; k < cFields; k++) {
                     field = fields->getField(k);

                     g_value_init(&gValue, G_TYPE_STRING);
                     g_value_set_string(&gValue, record->getValue(k)->asString()->getValue());
                     gtk_list_store_set_value(store, &iter, k, &gValue);
                     g_value_unset(&gValue);
                  }
               }
               return true;
            }
         }
      }

   }

   return false;
}

bool TCBDriverGTK::insertGridRow(TCBGrid *aMagnet, BValue *aValue) {
   GFTRACE();

   GtkListStore *store = getGridStore(aMagnet);
   if (store != NULL) {
      BTable *table = aMagnet->data.get();
      //      GtkTreeView *view = GTK_TREE_VIEW( aMagnet->magic );
      int iRow = aValue->asInteger();
      if (table != NULL) {
         GtkTreeIter iter;

         BFields *fields = table->fields.get();
         unsigned int cFields = fields->size();
         BField *field;

         BRecords *records = table->records.get();
         if (records != NULL) {
            BRecord *record = records->getRecord(iRow);
            if (record != NULL) {
               GValue *values = new GValue[cFields];
               gint *columns = new gint[cFields];
               memset(values, 0, cFields * sizeof(GValue));
               for (unsigned int k = 0; k < cFields; k++) {
                  field = fields->getField(k);
                  columns[k] = k;

                  g_value_init(&values[k], G_TYPE_STRING);
                  g_value_set_string(&values[k], record->getValue(k)->asString()->getValue());
               }
               gtk_list_store_insert_with_valuesv(store, &iter, iRow, columns, values, cFields);
            }
         }
      }

   }

   return false;
}

bool TCBDriverGTK::deleteGridRow(TCBGrid *aMagnet, BValue *aValue) {
   GFTRACE();
   GtkListStore *store = getGridStore(aMagnet);
   if (store != NULL) {
      //      GtkTreeView *view = GTK_TREE_VIEW( aMagnet->magic );
      GtkTreeIter iter;
      int iRow = aValue->asInteger();
      if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter, NULL, iRow)) {
         gtk_list_store_remove(store, &iter);

         return true;
      }
   }

   return false;
}

void TCBDriverGTK::refreshItemsListbox(TCBListControl *aMagnet) {
   GFTRACE();
   if (aMagnet->iRemoteId != NULL) {
      GtkTreeIter iter;
      GValue value = { 0,0 };
      GtkTreeView *view = reinterpret_cast<GtkTreeView *>(aMagnet->magic);
      GtkTreeModel *model = gtk_tree_view_get_model(view);

      gtk_list_store_clear(GTK_LIST_STORE(model));

      unsigned int c = aMagnet->count();
      for (unsigned int i = 0; i < c; i++) {
         TCBListControlItem *item = aMagnet->getItem(i);

         g_value_init(&value, G_TYPE_STRING);
         g_value_set_string(&value, item->value.asString()->getValue());

         gtk_list_store_append(GTK_LIST_STORE(model), &iter);
         gtk_list_store_set_value(GTK_LIST_STORE(model), &iter, 0, &value);

         g_value_unset(&value);
      }

      aMagnet->selected.set(aMagnet->selected.internalGet());
   }
}

void TCBDriverGTK::refreshItemsCombobox(TCBListControl *aMagnet) {
   GFTRACE();
   if (aMagnet->iRemoteId != NULL) {
      GtkComboBox *widget = GTK_COMBO_BOX(aMagnet->iRemoteId);

      GtkTreeModel *model = gtk_combo_box_get_model(widget);
      gtk_list_store_clear(GTK_LIST_STORE(model));

      unsigned int c = aMagnet->count();
      for (unsigned int i = 0; i < c; i++) {
         TCBListControlItem *item = aMagnet->getItem(i);

         gtk_combo_box_insert_text(widget, i, item->value.asString()->getValue());
      }

      aMagnet->selected.set(aMagnet->selected.internalGet());
   }
}

void TCBDriverGTK::buildTree(TCBTree *aMagnet, BNode *aParentNode, BNode *aChildNode) {
   GtkTreeIter *iterParent = NULL;

   BAttribute *attrIter = NULL;
   if (aParentNode != NULL) {
      attrIter = aParentNode->getAttributeByName("__gtkcustomdata_treeiter");
      if (attrIter != NULL) {
         iterParent = reinterpret_cast<GtkTreeIter *>(attrIter->value.asInteger());
      }
   }

   GValue value = { 0,0 };

   GtkTreeView *view = reinterpret_cast<GtkTreeView *>(aMagnet->magic);
   GtkTreeModel *model = gtk_tree_view_get_model(view);

   g_value_init(&value, G_TYPE_STRING);
   g_value_set_string(&value, aChildNode->name.getValue());

   GtkTreeIter *iter = new GtkTreeIter;
   gtk_tree_store_append(GTK_TREE_STORE(model), iter, iterParent);
   gtk_tree_store_set_value(GTK_TREE_STORE(model), iter, 0, &value);


   attrIter = aChildNode->findOrCreateAttribute("__gtkcustomdata_treeiter");
   attrIter->value.setInteger(reinterpret_cast<unsigned long>(iter));
   attrIter->hideFromXML = true;

   unsigned int c = aChildNode->childCount();
   if (c > 0) {
      for (unsigned int i = 0; i < c; i++) {
         BNode *child = aChildNode->getChild(i);
         if (child != NULL) {
            buildTree(aMagnet, aChildNode, child);
         }
      }
   }
}

void TCBDriverGTK::refreshTree(TCBTree *aMagnet) {
   if (aMagnet->iRemoteId != NULL) {
      GtkTreeView *view = reinterpret_cast<GtkTreeView *>(aMagnet->magic);
      GtkTreeModel *model = gtk_tree_view_get_model(view);

      gtk_tree_store_clear(GTK_TREE_STORE(model));

      BNode *data = aMagnet->data.internalGet();
      if (data != NULL) {
         buildTree(aMagnet, NULL, data);
      }
   }
}

BNode *TCBDriverGTK::getSelectedItemTreeView(TCBMagnet *aMagnet) {
   BNode *selectedNode = NULL;

   if (aMagnet->iRemoteId != NULL) {
      GtkTreeView *view = reinterpret_cast<GtkTreeView *>(aMagnet->magic);
      GtkTreeModel *model = gtk_tree_view_get_model(view);

      GtkTreeSelection *selection = gtk_tree_view_get_selection(view);
      GtkTreeIter iter;

      if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
         GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
         gchar* sPath = gtk_tree_path_to_string(path);
         BNodePath *nodepath = new BNodePath();
         printf("sPath: %s\n", sPath);
         if ((sPath[0] == '0') && (sPath[1] == ':')) {
            nodepath->readFromString(sPath + 2);
         }
         else {
            nodepath->readFromString(sPath);
         }
         BNode *rootNode = static_cast<TCBTree *>(aMagnet)->data.internalGet();
         if (rootNode != NULL) {
            selectedNode = rootNode->getChildByPath(nodepath);
         }
         delete nodepath;
         g_free(sPath);
         gtk_tree_path_free(path);
      }
   }

   return selectedNode;
}


void TCBDriverGTK::setinfo_magnet(TCBMagnet *aMagnet, int iInfoType, BValue *aSourceValue) {
   lGeneralAccess.lockWhenAvailable(GFLOCK_INFINITEWAIT);

   //   TCBGTKWindow *w;
   GtkTextBuffer *aBuffer;
   TCBListControlItem *item;
   GtkTreeIter iter;
   GValue value = { 0,0 };
   GtkTreePath *path;
   GtkWidget *subWidget;
   GtkTreeModel *model;
   TCBMagnet *parentMagnet;
   bool bTmp;
   long iNewX, iNewY;
   TCBGTKWindow *w;
   //   TCBMenu *menu = NULL;

   GtkTextBuffer *buffer;
   GtkClipboard *clipboard;
   GtkTextIter start, end;

   if (aMagnet->iRemoteId != NULL) {
      GtkWidget *widget = reinterpret_cast<GtkWidget *>(aMagnet->iRemoteId);
      GtkWidget *magicwidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);

      switch (iInfoType) {
      case CB_INFOTYPE_FOCUS:
         gtk_widget_grab_focus(widget);
         break;
      case CB_INFOTYPE_CUTTEXT:
         buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(magicwidget));
         clipboard = gtk_widget_get_clipboard(magicwidget, GDK_SELECTION_CLIPBOARD);
         gtk_text_buffer_cut_clipboard(buffer, clipboard, true);
         gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(magicwidget), gtk_text_buffer_get_insert(buffer), 0, FALSE, 0.0, 0.0);
         break;
      case CB_INFOTYPE_COPYTEXT:
         buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(magicwidget));
         clipboard = gtk_widget_get_clipboard(magicwidget, GDK_SELECTION_CLIPBOARD);
         gtk_text_buffer_copy_clipboard(buffer, clipboard);
         break;
      case CB_INFOTYPE_PASTETEXT:
         buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(magicwidget));
         clipboard = gtk_widget_get_clipboard(magicwidget, GDK_SELECTION_CLIPBOARD);
         gtk_text_buffer_paste_clipboard(buffer, clipboard, NULL, true);
         gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(widget), gtk_text_buffer_get_insert(buffer), 0, FALSE, 0.0, 0.0);
         break;
      case CB_INFOTYPE_SELECTALLTEXT:
         buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(magicwidget));
         gtk_text_buffer_get_bounds(buffer, &start, &end);
         gtk_text_buffer_select_range(buffer, &start, &end);
         break;
      case CB_INFOTYPE_EDITTEXT:
         if (aMagnet->iRemoteType == CB_TYPE_BUTTON) {
            gtk_button_set_label(GTK_BUTTON(widget), aSourceValue->asString()->getValue());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_RADIOBUTTON) {
            gtk_button_set_label(GTK_BUTTON(widget), aSourceValue->asString()->getValue());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_CHECKBOX) {
            gtk_button_set_label(GTK_BUTTON(widget), aSourceValue->asString()->getValue());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_EDITBOX) {
            gtk_entry_set_text(GTK_ENTRY(widget), aSourceValue->asString()->getValue());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_LABEL) {
            gtk_label_set_label(GTK_LABEL(widget), aSourceValue->asString()->getValue());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_MULTILINE) {
            subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);
            aBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));
            gtk_text_buffer_set_text(aBuffer, aSourceValue->asString()->getValue(), aSourceValue->asString()->getLength());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_PROGRESSBAR) {
            gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widget), aSourceValue->asString()->getValue());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_GROUPBOX) {
            if (aSourceValue->asString()->getLength() != 0) {
               gtk_frame_set_label(GTK_FRAME(widget), aSourceValue->asString()->getValue());
            }
            else {
               gtk_frame_set_label(GTK_FRAME(widget), NULL);
            }
         }
         else if (aMagnet->iRemoteType == CB_TYPE_WINDOW) {
            gtk_window_set_title(GTK_WINDOW(widget), aSourceValue->asString()->getValue());
         }
         else if (aMagnet->iRemoteType == CB_TYPE_NOTEBOOKTAB) {
            parentMagnet = aMagnet->parent.get();
            if (parentMagnet != NULL) {
               gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(parentMagnet->iRemoteId), widget, aSourceValue->asString()->getValue());
            }
         }

         break;
      case CB_INFOTYPE_LISTITEMSEL:
         if (aMagnet->iRemoteType == CB_TYPE_LISTBOX) {
            subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);
            if (aSourceValue->asInteger() >= 0) {
               path = gtk_tree_path_new_from_indices(aSourceValue->asInteger(), -1);
               gtk_tree_view_set_cursor(GTK_TREE_VIEW(subWidget), path, NULL, false);
               gtk_tree_path_free(path);
            }
         }
         else if (aMagnet->iRemoteType == CB_TYPE_COMBOBOX) {
            gtk_combo_box_set_active(GTK_COMBO_BOX(widget), aSourceValue->asInteger());
         }

         break;
      case CB_INFOTYPE_BARPOSITION:
         if (aSourceValue->asDouble() == 0) {
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widget), 0.0);
         }
         else {
            gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(widget), aSourceValue->asDouble() / 100.0);
         }
         break;
      case CB_INFOTYPE_LISTITEMADD:
         item = static_cast<TCBListControl *>(aMagnet)->getItem(aSourceValue->asInteger());
         if (item != NULL) {
            if (aMagnet->iRemoteType == CB_TYPE_LISTBOX) {
               subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);
               model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));

               g_value_init(&value, G_TYPE_STRING);
               g_value_set_string(&value, item->value.asString()->getValue());
               gtk_list_store_insert(GTK_LIST_STORE(model), &iter, aSourceValue->asInteger());
               gtk_list_store_set_value(GTK_LIST_STORE(model), &iter, 0, &value);
               g_value_unset(&value);
            }
            else if (aMagnet->iRemoteType == CB_TYPE_COMBOBOX) {
               gtk_combo_box_insert_text(GTK_COMBO_BOX(widget), aSourceValue->asInteger(), item->value.asString()->getValue());
            }
         }
         break;
      case CB_INFOTYPE_LISTITEMDEL:
         if (aMagnet->iRemoteType == CB_TYPE_LISTBOX) {
            subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);
            model = gtk_tree_view_get_model(GTK_TREE_VIEW(subWidget));
            path = gtk_tree_path_new_from_indices(aSourceValue->asInteger(), -1);
            if (gtk_tree_model_get_iter(GTK_TREE_MODEL(model), &iter, path)) {
               gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
            }
            gtk_tree_path_free(path);
         }
         else if (aMagnet->iRemoteType == CB_TYPE_COMBOBOX) {
            gtk_combo_box_remove_text(GTK_COMBO_BOX(widget), aSourceValue->asInteger());
         }
         break;
      case CB_INFOTYPE_LISTITEMCLR:
         if (aMagnet->iRemoteType == CB_TYPE_LISTBOX) {
            refreshItemsListbox(static_cast<TCBListControl *>(aMagnet));
         }
         else if (aMagnet->iRemoteType == CB_TYPE_COMBOBOX) {
            refreshItemsCombobox(static_cast<TCBListControl *>(aMagnet));
         }

         break;
      case CB_INFOTYPE_BUTTONSTATE:
         gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget), aSourceValue->asBoolean());
         break;
      case CB_INFOTYPE_ALIGNMENT:
         if (aMagnet->iRemoteType == CB_TYPE_LABEL) {
            switch (aSourceValue->asInteger()) {
            case CB_ALIGN_LEFT:
               gtk_misc_set_alignment(GTK_MISC(widget), 0, 0.5);
               break;
            case CB_ALIGN_CENTER:
               gtk_misc_set_alignment(GTK_MISC(widget), 0.5, 0.5);
               break;
            case CB_ALIGN_RIGHT:
               gtk_misc_set_alignment(GTK_MISC(widget), 1, 0.5);
               break;
            }
         }
         break;
      case CB_INFOTYPE_GRIDFULLUPDATE:
         updateGridHeaders(static_cast<TCBGrid *>(aMagnet), aSourceValue);
         updateGridAllRows(static_cast<TCBGrid *>(aMagnet), aSourceValue);
         break;
      case CB_INFOTYPE_GRIDROWUPDATE:
         updateGridRow(static_cast<TCBGrid *>(aMagnet), aSourceValue);
         break;
      case CB_INFOTYPE_GRIDROWADD:
         insertGridRow(static_cast<TCBGrid *>(aMagnet), aSourceValue);
         break;
      case CB_INFOTYPE_GRIDROWDELETE:
         deleteGridRow(static_cast<TCBGrid *>(aMagnet), aSourceValue);
         break;
      case CB_INFOTYPE_GRIDCLEAR:
         gtk_list_store_clear(GTK_LIST_STORE(getGridStore(static_cast<TCBGrid *>(aMagnet))));
         break;
      case CB_INFOTYPE_GRIDROWSELECT:
         subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);
         if (subWidget != NULL) {
            if (aSourceValue->asInteger() >= 0) {
               path = gtk_tree_path_new_from_indices(aSourceValue->asInteger(), -1);
               gtk_tree_view_set_cursor(GTK_TREE_VIEW(subWidget), path, NULL, false);
               gtk_tree_path_free(path);
            }
         }
         break;
      case CB_INFOTYPE_GRIDHEADERSUPDATE:
         updateGridHeaders(static_cast<TCBGrid *>(aMagnet), aSourceValue);
         break;
      case CB_INFOTYPE_NOTEBOOKTABSELECT:
         gtk_notebook_set_current_page(GTK_NOTEBOOK(widget), aSourceValue->asInteger());
         break;
      case CB_INFOTYPE_MENUCHECKED:
         bTmp = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(aMagnet->iRemoteId));

         if (bTmp != aSourceValue->asBoolean()) {
            gtk_signal_handler_block_by_data(GTK_OBJECT(aMagnet->iRemoteId), aMagnet);
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(aMagnet->iRemoteId), aSourceValue->asBoolean());
            gtk_signal_handler_unblock_by_data(GTK_OBJECT(aMagnet->iRemoteId), aMagnet);
         }
         break;
      case CB_INFOTYPE_SETCONTEXTMENU:
         break;
      case CB_INFOTYPE_TREEFULLUPDATE:
         refreshTree(static_cast<TCBTree *>(aMagnet));
         break;
      case CB_INFOTYPE_REPOSITION:
         iNewX = aMagnet->x;
         iNewY = aMagnet->y;
         gtk_widget_set_size_request(widget, aMagnet->w, aMagnet->h);
         if (aMagnet->iRemoteType == CB_TYPE_WINDOW) {
            if (iNewX == CB_LOCATION_CENTEROFSCREEN) {
               iNewX = aMagnet->getScreen()->w / 2 - aMagnet->w / 2;
            }
            if (iNewY == CB_LOCATION_CENTEROFSCREEN) {
               iNewY = aMagnet->getScreen()->h / 2 - aMagnet->h / 2;
            }

            gtk_window_move(GTK_WINDOW(widget), iNewX, iNewY);
         }
         else {
            w = getGTKWindowObject(aMagnet->parent.internalGet());
            if (w != NULL) {
               gtk_fixed_move(GTK_FIXED(w->widget), widget, iNewX, iNewY);
            }
         }

         break;
      default:
         break;
      }
   }

   lGeneralAccess.unlock();
}

void TCBDriverGTK::getinfo_magnet(TCBMagnet *aMagnet, int iInfoType, BValue *aDestValue) {
   lGeneralAccess.lockWhenAvailable(GFLOCK_INFINITEWAIT);

   //   TCBGTKWindow *w;
   GtkTextBuffer *aBuffer;
   GtkTextIter start;
   GtkTextIter end;
   gchar *sText;
   GtkTreePath *path;
   gint *indices;
   GtkWidget *subWidget;

   if (aMagnet->iRemoteId != NULL) {
      GtkWidget *widget = reinterpret_cast<GtkWidget *>(aMagnet->iRemoteId);

      switch (iInfoType) {
      case CB_INFOTYPE_EDITTEXT:
         if (aMagnet->iRemoteType == CB_TYPE_EDITBOX) {
            aDestValue->setString(gtk_entry_get_text(GTK_ENTRY(widget)));
         }
         else if (aMagnet->iRemoteType == CB_TYPE_MULTILINE) {
            subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);

            aBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(subWidget));
            gtk_text_buffer_get_start_iter(aBuffer, &start);
            gtk_text_buffer_get_end_iter(aBuffer, &end);
            sText = gtk_text_buffer_get_text(aBuffer, &start, &end, true);
            aDestValue->setString(sText);
            g_free(sText);
         }
         break;
      case CB_INFOTYPE_LISTITEMSEL:
         if (aMagnet->iRemoteType == CB_TYPE_LISTBOX) {
            subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);
            gtk_tree_view_get_cursor(GTK_TREE_VIEW(subWidget), &path, NULL);

            if (path != NULL) {
               indices = gtk_tree_path_get_indices(path);
               aDestValue->setInteger(indices[0]);
               gtk_tree_path_free(path);
            }
            else {
               aDestValue->setInteger(-1);
            }
         }
         else if (aMagnet->iRemoteType == CB_TYPE_COMBOBOX) {
            aDestValue->setInteger(gtk_combo_box_get_active(GTK_COMBO_BOX(widget)));
         }

         break;
      case CB_INFOTYPE_BARPOSITION:
         break;
      case CB_INFOTYPE_BUTTONSTATE:
         aDestValue->setBoolean(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)));
         break;
      case CB_INFOTYPE_GRIDROWSELECT:
         subWidget = reinterpret_cast<GtkWidget *>(aMagnet->magic);
         gtk_tree_view_get_cursor(GTK_TREE_VIEW(subWidget), &path, NULL);

         if (path != NULL) {
            indices = gtk_tree_path_get_indices(path);
            aDestValue->setInteger(indices[0]);
            gtk_tree_path_free(path);
         }
         else {
            aDestValue->setInteger(-1);
         }
         break;
      case CB_INFOTYPE_NOTEBOOKTABSELECT:
         aDestValue->setInteger(gtk_notebook_get_current_page(GTK_NOTEBOOK(widget)));
         break;
      case CB_INFOTYPE_TREESELECT:
         static_cast<TCBTree *>(aMagnet)->selected.internalSet(getSelectedItemTreeView(aMagnet));
         break;
      default:
         break;
      }
   }

   lGeneralAccess.unlock();
}

long TCBDriverGTK::messagebox(TCBMagnet *aParent, String *sCaption, String *sText, long iType) {
   GtkWindow *parent = NULL;
   if (aParent != NULL) {
      parent = GTK_WINDOW(reinterpret_cast<GtkWidget *>(aParent->iRemoteId));
   }

   GtkDialog *dialog = NULL;

   switch (iType) {
   case CB_MSGBOX_OK:
      dialog = GTK_DIALOG(gtk_message_dialog_new(parent, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, sText->getValue()));
      break;
   case CB_MSGBOX_OKCANCEL:
      dialog = GTK_DIALOG(gtk_message_dialog_new(parent, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK_CANCEL, sText->getValue()));
      break;
   case CB_MSGBOX_YESNO:
      dialog = GTK_DIALOG(gtk_message_dialog_new(parent, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_YES_NO, sText->getValue()));
      break;
   case CB_MSGBOX_YESNOCANCEL:
      dialog = GTK_DIALOG(gtk_message_dialog_new(parent, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_NONE, sText->getValue()));
      gtk_dialog_add_buttons(dialog, GTK_STOCK_YES, GTK_RESPONSE_YES);
      gtk_dialog_add_buttons(dialog, GTK_STOCK_NO, GTK_RESPONSE_NO);
      gtk_dialog_add_buttons(dialog, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL);
      break;
   }

   gtk_window_set_title(GTK_WINDOW(dialog), sCaption->getValue());

   gint iRet = gtk_dialog_run(dialog);
   gtk_widget_hide(GTK_WIDGET(dialog));
   gtk_widget_destroy(GTK_WIDGET(dialog));
   switch (iRet) {
   case GTK_RESPONSE_OK:
      return CB_MSGBOX_OK;
   case GTK_RESPONSE_CANCEL:
      return CB_MSGBOX_CANCEL;
   case GTK_RESPONSE_YES:
      return CB_MSGBOX_YES;
   case GTK_RESPONSE_NO:
      return CB_MSGBOX_NO;
   }

   return 0;
}

void TCBDriverGTK::mainloop_iteration() {
   gtk_main_iteration_do(false);
}



/*
GtkWidget*          gtk_file_chooser_dialog_new         (const gchar *title,
                                                         GtkWindow *parent,
                                                         GtkFileChooserAction action,
                                                         const gchar *first_button_text,
                                                         ...);
*/


String *TCBDriverGTK::filedialog(TCBMagnet *aParent, String *sCaption, TCBFileDialogFilter *aFilter, String *sDefaultDirectory, String *sDefaultFileName, long iType) {
   String *sSelectedFilename = NULL;

   GtkWidget *dlg = NULL;

   GtkWidget *hParent = NULL;
   if (aParent != NULL) {
      hParent = reinterpret_cast<GtkWidget *>(aParent->iRemoteId);
   }

   char *pCaption = NULL;
   if (sCaption != NULL) {
      pCaption = sCaption->getValue();
   }
   else {
      if (iType == CB_FILEDIALOG_SAVE) {
         pCaption = "Save";
      }
      else {
         pCaption = "Open";
      }
   }

   if (iType == CB_FILEDIALOG_SAVE) {
      dlg = gtk_file_chooser_dialog_new(pCaption, GTK_WINDOW(hParent), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT, NULL);
   }
   else {
      dlg = gtk_file_chooser_dialog_new(pCaption, GTK_WINDOW(hParent), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
   }

   if (sDefaultDirectory != NULL) {
      gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dlg), sDefaultDirectory->getValue());
   }

   if (sDefaultFileName != NULL) {
      if (iType == CB_FILEDIALOG_SAVE) {
         gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dlg), sDefaultFileName->getValue());
      }
   }

   if (aFilter != NULL) {
      unsigned int c = aFilter->size();
      for (unsigned int i = 0; i < c; i++) {
         TCBFileDialogFilterItem *item = aFilter->getItem(i);

         GtkFileFilter* f = gtk_file_filter_new();
         gtk_file_filter_set_name(f, item->caption.getValue());

         unsigned int d = item->size();
         for (unsigned int j = 0; j < d; j++) {
            gtk_file_filter_add_pattern(f, item->getPattern(j)->getValue());
         }

         gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg), f);
      }
   }

   gint response = gtk_dialog_run(GTK_DIALOG(dlg));
   if (response == GTK_RESPONSE_ACCEPT) {
      char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
      sSelectedFilename = new String(filename);
      g_free(filename);
   }
   gtk_widget_destroy(dlg);

   return sSelectedFilename;
}



#endif // GF_OS_MAC
