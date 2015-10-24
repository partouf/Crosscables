#import "CocoaIntf.h"
#import <Cocoa/Cocoa.h>
#import <vector>

//------------------------------------------------------------------------

@interface CBCocoaWindow: NSWindow <NSWindowDelegate> {
   unsigned long customid;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (BOOL) windowShouldClose:(id)window;
   - (BOOL) acceptsFirstResponder;
   - (BOOL) becomeFirstResponder;
   - (BOOL) canBecomeKeyWindow;
   - (BOOL) canBecomeMainWindow;
   - (void) windowDidResize:(NSNotification *)notification;
@end

@interface CBCocoaTextField: NSTextField {
   bool visible;
   unsigned long customid;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (void)setVisible:(bool)bVisible;
@end

@interface CBCocoaButton: NSButton {
   bool visible;
   unsigned long customid;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (void)setVisible:(bool)bVisible;
   - (void)drawRect:(NSRect)aRect;
@end

@interface CBCocoaCombobox: NSPopUpButton {
   bool visible;
   unsigned long customid;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (void)setVisible:(bool)bVisible;
@end

@interface CBCocoaScrollView: NSScrollView {
}
@end

@interface CBCocoaTextView: NSTextView {
   bool visible;
   unsigned long customid;
   NSMenu *contextMenu;
   CBCocoaScrollView *scroll;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (void)setScroll:(CBCocoaScrollView *)theScroll;
   - (CBCocoaScrollView *)getScroll;
   - (void)setContextMenu:(NSMenu *)theMenu;
   - (void)mouseDown:(NSEvent *)theEvent;
   - (void)setVisible:(bool)bVisible;
   - (BOOL)performKeyEquivalent:(NSEvent *)theEvent;
@end

@interface CBCocoaTable: NSTableView {
   bool visible;
   unsigned long customid;
   NSMenu *contextMenu;
   CBCocoaScrollView *scroll;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (void)setScroll:(CBCocoaScrollView *)theScroll;
   - (CBCocoaScrollView *)getScroll;
   - (void)setContextMenu:(NSMenu *)theMenu;
   - (void)mouseDown:(NSEvent *)theEvent;
   - (void)setVisible:(bool)bVisible;
@end

@interface CBCocoaTree: NSOutlineView <NSOutlineViewDelegate> {
   bool visible;
   unsigned long customid;
   NSMenu *contextMenu;
   CBCocoaScrollView *scroll;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (void)setScroll:(CBCocoaScrollView *)theScroll;
   - (CBCocoaScrollView *)getScroll;
   - (void)setContextMenu:(NSMenu *)theMenu;
   - (void)mouseDown:(NSEvent *)theEvent;
   - (void)setVisible:(bool)bVisible;
   - (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(NSTableColumn *)tableColumn item:(id)item;
   - (BOOL)outlineView:(NSOutlineView *)outlineView shouldExpandItem:(id)item;
   - (void)outlineViewItemWillExpand:(NSNotification *)notification;
@end

@interface CBCocoaTabView: NSTabView {
   unsigned long customid;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
@end

@interface CBCocoaTabViewItem: NSTabViewItem {
   unsigned long customid;
}
   - (void)setCustomId:(unsigned long)iId;
   - (unsigned long)getCustomId;
   - (id)contentView;
@end


//------------------------------------------------------------------------

@interface AppController : NSObject <NSApplicationDelegate>
   - (void) buttonClick:(id)sender;
   - (void) radiobuttonClick:(id)sender;
   - (void) comboClick:(id)sender;
   - (void) menuClick:(id)sender;
   - (void) applicationWillFinishLaunching: (NSNotification *)notif;
   - (void) tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem;
@end

typedef struct __radiogroup {
   id parent;
   std::vector<id> buttons;
} radiogroup;

@interface GridDataSource: NSObject <NSTableViewDelegate> {
   unsigned long thecustomid;
   CINTF_GRIDEDITABLEFUNC functionIsEditable;
   CINTF_GRIDNUMBEROFROWSFUNC functionNumberOfRows;
   CINTF_GRIDGETVALUEFUNC functionGetValue;
   CINTF_GRIDSETVALUEFUNC functionSetValue;
   NSAutoreleasePool *pool;
}
   - (id)init;
   - (void)setthecustomid:(unsigned long)customid;
   - (void)setfunceditable:(CINTF_GRIDEDITABLEFUNC)func;
   - (void)setfuncnumberofrows:(CINTF_GRIDNUMBEROFROWSFUNC)func;
   - (void)setfuncgetvalue:(CINTF_GRIDGETVALUEFUNC)func;
   - (void)setfuncsetvalue:(CINTF_GRIDSETVALUEFUNC)func;

   - (BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
   - (int)numberOfRowsInTableView:(NSTableView *)aTableView;
   - (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
   - (void)tableView:(NSTableView *)aTableView setObjectValue:anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex;
@end

@interface CBCocoaTreeNode: NSObject {
   unsigned long remoteid;
   NSString *lastValue;
}
   - (unsigned long)getRemoteId;
   - (NSString *)getLastValue;
   - (void)setRemoteId:(unsigned long)id;
   - (void)setLastValue:(NSString *)s;
@end

@interface TreeDataSource: NSObject {
   unsigned long thecustomid;
   CINTF_TREEGETCHILDCOUNTFUNC functionGetChildCount;
   CINTF_TREEGETCHILDIDFUNC functionGetChildId;
   CINTF_TREEGETVALUEFUNC functionGetValue;
   CINTF_TREEGETCUSTOMVALUEFUNC functionGetCustomValue;
   CINTF_TREESETCUSTOMVALUEFUNC functionSetCustomValue;
}
   - (id)init;
   - (void)setthecustomid:(unsigned long)customid;

   - (void)setfuncgetchildcount:(CINTF_TREEGETCHILDCOUNTFUNC)func;
   - (void)setfuncgetchildid:(CINTF_TREEGETCHILDIDFUNC)func;
   - (void)setfuncgetvalue:(CINTF_TREEGETVALUEFUNC)func;

   - (void)setfuncgetcustomvalue:(CINTF_TREEGETCUSTOMVALUEFUNC)func;
   - (void)setfuncsetcustomvalue:(CINTF_TREESETCUSTOMVALUEFUNC)func;

   - (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item;
   - (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item;
   - (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item;
   - (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;
//   - (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;
@end

//------------------------------------------------------------------------

static long iScreenWidth;
static long iScreenHeight;

AppController *delegate;
NSAutoreleasePool *pool;
NSApplication *app;
unsigned long mainWin;

static std::vector<radiogroup *> radiogroups;

static CINTF_EVENTFUNC clickeventfunction = 0;
static CINTF_EVENTFUNC closeeventfunction = 0;
static CINTF_EVENTFUNC menuclickeventfunction = 0;
static CINTF_TABSELECTEVENTFUNC tabselecteventfunction = 0;
static CINTF_SIZEEVENTFUNC windowresizeeventfunction = 0;


//------------------------------------------------------------------------

void setCocoaClickEventMethod( CINTF_EVENTFUNC func ) {
   clickeventfunction = func;
}

void setCocoaMenuClickEventMethod( CINTF_EVENTFUNC func ) {
   menuclickeventfunction = func;
}

void setCocoaCloseEventMethod( CINTF_EVENTFUNC func ) {
   closeeventfunction = func;
}

void setCocoaTabSelectEventMethod( CINTF_TABSELECTEVENTFUNC func ) {
   tabselecteventfunction = func;
}

void setCocoaWindowResizeEventMethod( CINTF_SIZEEVENTFUNC func ) {
   windowresizeeventfunction = func;
}

radiogroup *findRadiogroup( id parent ) {
   unsigned int c = radiogroups.size();
   for ( unsigned int i = 0; i < c; i++ ) {
      radiogroup *grp = radiogroups[i];
      if ( grp != NULL ) {
         if ( grp->parent == parent ) {
            return grp;
         }
      }
   }

   return NULL;
}

void addToRadiogroup( id parent, id button ) {
   radiogroup *grp = findRadiogroup( parent );
   if ( grp == NULL ) {
      grp = new radiogroup;
      grp->parent = parent;
      radiogroups.push_back( grp );
   }

   grp->buttons.push_back( button );
}

//------------------------------------------------------------------------
@implementation AppController
   - (void) menuClick:(id)sender {
      if ( menuclickeventfunction != 0 ) {
         menuclickeventfunction( (unsigned long)sender, 0 );
      } else {
         NSLog( @"menuclick %d", (int)sender );
      }
   }
   - (void) buttonClick:(id)sender {
      if ( clickeventfunction != 0 ) {
         CBCocoaButton *button = (CBCocoaButton *)sender;
         clickeventfunction( (unsigned long)sender, [button getCustomId] );
      } else {
         NSLog( @"buttonclick %d", (int)sender );
      }
   }
   - (void) comboClick:(id)sender {
      if ( clickeventfunction != 0 ) {
         CBCocoaCombobox *combo = (CBCocoaCombobox *)sender;
         clickeventfunction( (unsigned long)sender, [combo getCustomId] );
      } else {
         NSLog( @"comboclick %d", (int)sender );
      }
   }
   - (void) radiobuttonClick:(id)sender {
      NSView *parent = [sender superview];
      radiogroup *grp = findRadiogroup( parent );
      if ( grp != NULL ) {
         unsigned int c = grp->buttons.size();
         for ( unsigned int i = 0; i < c; i++ ) {
            id obj = grp->buttons[i];
            if ( obj != sender ) {
               [obj setIntValue: 0];
            }
         }
      }

      if ( clickeventfunction != 0 ) {
         CBCocoaButton *button = (CBCocoaButton *)sender;

         clickeventfunction( (unsigned long)sender, [button getCustomId] );
      } else {
         NSLog( @"radiobuttonclick %d", (int)sender );
      }
   }
   - (void) applicationWillFinishLaunching: (NSNotification *)notif {
      NSWindow *keyWindow = [app keyWindow];

      NSWindow *win = (NSWindow *)mainWin;
      [win makeMainWindow];
      [win makeKeyAndOrderFront: nil];
   }
   - (void)tabView:(NSTabView *)tabView didSelectTabViewItem:(NSTabViewItem *)tabViewItem {
      if ( tabselecteventfunction != 0 ) {
         CBCocoaTabView *view = (CBCocoaTabView *)tabView;
         CBCocoaTabViewItem *tab = (CBCocoaTabViewItem *)tabViewItem;
         tabselecteventfunction( [view getCustomId], [tab getCustomId] );
      } else {
         NSLog( @"tabselectevent %d", (int)tabView );
      }
   }
@end

@implementation CBCocoaWindow
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (BOOL) canBecomeKeyWindow {
      return YES;
   }
   - (BOOL) canBecomeMainWindow {
      return YES;
   }
   - (BOOL) acceptsFirstResponder {
      return YES;
   }
   - (BOOL) becomeFirstResponder {
      return YES;
   }
   - (BOOL) windowShouldClose:(id)window {
      if ( closeeventfunction != 0 ) {
         if ( closeeventfunction( reinterpret_cast<unsigned long>(self), customid ) ) {
            return YES;
         } else {
            return NO;
         }
      } else {
         return YES;
      }
   }
   - (void) windowDidResize:(NSNotification *)notification {
      if ( windowresizeeventfunction != 0 ) {
         windowresizeeventfunction( reinterpret_cast<unsigned long>(self), customid, [[self contentView] frame].size.width, [[self contentView] frame].size.height );
      }
   }
@end

@implementation CBCocoaTextField
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (void)setVisible:(bool)bVisible {
      visible = bVisible;
   }
@end

@implementation CBCocoaScrollView
@end

@implementation CBCocoaTextView
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (void)setScroll:(CBCocoaScrollView *)theScroll {
      scroll = theScroll;
   }
   - (CBCocoaScrollView *)getScroll {
      return scroll;
   }
   - (void)setContextMenu:(NSMenu *)theMenu {
      contextMenu = theMenu;
   }
   - (void)setVisible:(bool)bVisible {
   }
   - (void)mouseDown:(NSEvent *)theEvent {
      if ( [theEvent modifierFlags] & (NSCommandKeyMask != 0) ) {
         [super mouseDown: theEvent];

         if ( contextMenu != nil ) {
            [NSMenu popUpContextMenu:contextMenu withEvent:theEvent forView:self];
         }
      } else {
         [super mouseDown: theEvent];
      }
   }
   - (BOOL)performKeyEquivalent:(NSEvent *)theEvent {
      return NO;
   }
@end

@implementation CBCocoaTable
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (void)setScroll:(CBCocoaScrollView *)theScroll {
      scroll = theScroll;
   }
   - (CBCocoaScrollView *)getScroll {
      return scroll;
   }
   - (void)setContextMenu:(NSMenu *)theMenu {
      contextMenu = theMenu;
   }
   - (void)setVisible:(bool)bVisible {
   }
   - (void)mouseDown:(NSEvent *)theEvent {
      if ( [theEvent modifierFlags] & (NSCommandKeyMask != 0) ) {
         [super mouseDown: theEvent];

         if ( contextMenu != nil ) {
            [NSMenu popUpContextMenu:contextMenu withEvent:theEvent forView:self];
         }
      } else {
         [super mouseDown: theEvent];
      }
   }
@end

@implementation CBCocoaTreeNode
   - (unsigned long)getRemoteId {
      return remoteid;
   }
   - (NSString *)getLastValue {
      return lastValue;
   }
   - (void)setRemoteId:(unsigned long)id {
      remoteid = id;
   }
   - (void)setLastValue:(NSString *)s {
      lastValue = s;
   }
@end

@implementation CBCocoaTree
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (void)setScroll:(CBCocoaScrollView *)theScroll {
      scroll = theScroll;
   }
   - (CBCocoaScrollView *)getScroll {
      return scroll;
   }
   - (void)setContextMenu:(NSMenu *)theMenu {
      contextMenu = theMenu;
   }
   - (void)setVisible:(bool)bVisible {
   }
   - (void)mouseDown:(NSEvent *)theEvent {
      if ( [theEvent modifierFlags] & (NSCommandKeyMask != 0) ) {
         [super mouseDown: theEvent];

         if ( contextMenu != nil ) {
            [NSMenu popUpContextMenu:contextMenu withEvent:theEvent forView:self];
         }
      } else {
         [super mouseDown: theEvent];
      }
   }
   - (BOOL)outlineView:(NSOutlineView *)outlineView shouldEditTableColumn:(NSTableColumn *)tableColumn item:(id)item {
      return NO;
   }
   - (BOOL)outlineView:(NSOutlineView *)outlineView shouldExpandItem:(id)item {
      return YES;
   }
   - (void)outlineViewItemWillExpand:(NSNotification *)notification {
   }
@end

@implementation CBCocoaTabView
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
@end

@implementation CBCocoaTabViewItem
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (id)contentView {
      return [self view];
   }
@end

@implementation CBCocoaButton
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (void)setVisible:(bool)bVisible {
      visible = bVisible;
   }
   - (void)drawRect:(NSRect)aRect {
      if (visible) {
         [super drawRect:aRect];
      }
   }
@end

@implementation CBCocoaCombobox
   - (void)setCustomId:(unsigned long)iId {
      customid = iId;
   }
   - (unsigned long)getCustomId {
      return customid;
   }
   - (void)setVisible:(bool)bVisible {
      visible = bVisible;
   }
@end

@implementation GridDataSource
   - (id)init {
      self = [super init];
      pool = [[NSAutoreleasePool alloc] init];

      return self;
   }
   - (void)setthecustomid:(unsigned long)customid {
      thecustomid = customid;
   }
   - (void)setfunceditable:(CINTF_GRIDEDITABLEFUNC)func {
      functionIsEditable = func;
   }
   - (void)setfuncnumberofrows:(CINTF_GRIDNUMBEROFROWSFUNC)func {
      functionNumberOfRows = func;
   }
   - (void)setfuncgetvalue:(CINTF_GRIDGETVALUEFUNC)func {
      functionGetValue = func;
   }
   - (void)setfuncsetvalue:(CINTF_GRIDSETVALUEFUNC)func {
      functionSetValue = func;
   }

   - (BOOL)tableView:(NSTableView *)aTableView shouldEditTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
      NSArray *tableColumns = [aTableView tableColumns];
      if ( functionIsEditable( thecustomid, (long)[tableColumns indexOfObject: aTableColumn], rowIndex ) ) {
         return YES;
      }
      return NO;
   }
   - (int)numberOfRowsInTableView:(NSTableView *)aTableView {
      int i = functionNumberOfRows( thecustomid );
      return i;
   }
   - (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
      NSArray *tableColumns = [aTableView tableColumns];
      char *caption = functionGetValue( thecustomid, (long)[tableColumns indexOfObject: aTableColumn], rowIndex );

      NSString *s = [[[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding] autorelease];
      return s;
   }
   - (void)tableView:(NSTableView *)aTableView setObjectValue:anObject forTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
      NSArray *tableColumns = [aTableView tableColumns];
      functionSetValue( thecustomid, (long)[tableColumns indexOfObject: aTableColumn], rowIndex, [anObject UTF8String] );
   }
@end

@implementation TreeDataSource
   - (id)init {
      thecustomid = 0;
      functionGetChildCount = nil;
      functionGetChildId = nil;
      functionGetValue = nil;
      functionGetCustomValue = nil;
      functionSetCustomValue = nil;

	  return self;
   }
   - (void)setthecustomid:(unsigned long)customid {
      thecustomid = customid;
   }

   - (void)setfuncgetchildcount:(CINTF_TREEGETCHILDCOUNTFUNC)func {
      functionGetChildCount = func;
   }
   - (void)setfuncgetchildid:(CINTF_TREEGETCHILDIDFUNC)func {
      functionGetChildId = func;
   }
   - (void)setfuncgetvalue:(CINTF_TREEGETVALUEFUNC)func {
      functionGetValue = func;
   }
   - (void)setfuncgetcustomvalue:(CINTF_TREEGETCUSTOMVALUEFUNC)func {
      functionGetCustomValue = func;
   }
   - (void)setfuncsetcustomvalue:(CINTF_TREESETCUSTOMVALUEFUNC)func {
      functionSetCustomValue = func;
   }
   - (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item {
      unsigned long childid;
      CBCocoaTreeNode *node;
      if ( item != nil ) {
         node = (CBCocoaTreeNode *)item;
         childid = functionGetChildId( thecustomid, [node getRemoteId], index );
      } else {
         childid = functionGetChildId( thecustomid, 0, index );
      }

      node = (CBCocoaTreeNode *)functionGetCustomValue( thecustomid, childid );
      if ( node == nil ) {
         node = [[CBCocoaTreeNode alloc] init];
         [node setRemoteId: childid];
         [node setLastValue: nil];

         functionSetCustomValue( thecustomid, childid, (unsigned long)node );
      }

      return node;
   }
   - (BOOL)outlineView:(NSOutlineView *)outlineView isItemExpandable:(id)item {
      CBCocoaTreeNode *node = (CBCocoaTreeNode *)item;
      long ret = functionGetChildCount( thecustomid, [node getRemoteId] );

      if ( ret != 0 ) {
         return YES;
      }
      return NO;
   }
   - (NSInteger)outlineView:(NSOutlineView *)outlineView numberOfChildrenOfItem:(id)item {
      CBCocoaTreeNode *node = (CBCocoaTreeNode *)item;
      return functionGetChildCount( thecustomid, [node getRemoteId] );
   }
   - (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn *)tableColumn byItem:(id)item {
      CBCocoaTreeNode *node = (CBCocoaTreeNode *)item;
      char *caption = functionGetValue( thecustomid, [node getRemoteId] );

      NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];

      NSString *oldstr = [node getLastValue];
      if ( oldstr != nil ) {
         if ( [s isEqualToString: oldstr] ) {
            [s release];
            return oldstr;
         }
      } else {
         [node setLastValue: s];
      }

      return s;
   }

//   - (void)outlineView:(NSOutlineView *)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn byItem:(id)item;
@end

//------------------------------------------------------------------------

unsigned long addWindow( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid ) {
   CGFloat wh = iScreenHeight - h;
   int style = (NSTitledWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask | NSClosableWindowMask);
   CBCocoaWindow *win = [[CBCocoaWindow alloc] initWithContentRect: NSMakeRect(x, wh - y, w, h)
                                        styleMask: style
                                          backing: NSBackingStoreBuffered
                                            defer: YES];
   [win setCustomId: customid];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [win setTitle: s];
   [s release];
   [win setDelegate: win];

   [win setReleasedWhenClosed: NO];

   return (unsigned long)win;
}

unsigned long addBox( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   int style = (NSTitledWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask | NSClosableWindowMask);
   NSBox *panel = [[NSBox alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [panel setTitle: s];
   [s release];
   [vw addSubview:panel];

   return (unsigned long)panel;
}

unsigned long addPanel( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   int style = (NSTitledWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask | NSClosableWindowMask);
   NSBox *panel = [[NSBox alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [panel setTitle: s];
   [panel setBorderType: NSNoBorder];
   [s release];
   [vw addSubview:panel];

   return (unsigned long)panel;
}

unsigned long addTabView( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaTabView *tabview = [[CBCocoaTabView alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];
   [tabview setCustomId: customid];
   [tabview setDelegate: delegate];
   [vw addSubview: tabview];

   return (unsigned long)tabview;
}

unsigned long addTabViewItem( int index, const char *caption, unsigned long parent, unsigned long customid ) {
   CBCocoaTabView *tabview = (CBCocoaTabView *)parent;

   CBCocoaTabViewItem *tab = [[CBCocoaTabViewItem alloc] initWithIdentifier: nil];
   [tab setCustomId: customid];
   if ( index >= 0 ) {
      [tabview insertTabViewItem: tab
                         atIndex: index];
   } else {
      [tabview addTabViewItem: tab];
   }

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [tab setLabel: s];
   [s release];

   return (unsigned long)tab;
}

//------------------------------------------------------------------------


unsigned long addTextFieldLabel( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaTextField *lbl = [[CBCocoaTextField alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];
   [lbl setVisible: true];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [lbl setStringValue: s];
   [s release];

   [vw addSubview:lbl];
   [lbl setBordered:NO];
   [lbl setDrawsBackground:NO];
   [lbl setEnabled:YES];
   [lbl setEditable:NO];

   return (unsigned long)lbl;
}

unsigned long addTextField( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaTextField *lbl = [[CBCocoaTextField alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [lbl setStringValue: s];
   [s release];

   [vw addSubview:lbl];
   [lbl setBordered:NO];
   [lbl setBezelStyle: NSTextFieldSquareBezel];
   [lbl setBezeled:YES];
   [lbl setDrawsBackground:YES];
   [lbl setEnabled:YES];
   [lbl setEditable:YES];

   return (unsigned long)lbl;
}

unsigned long addProgressIndicator( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   NSProgressIndicator *obj = [[NSProgressIndicator alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];

   [obj setIndeterminate: NO];
   [obj startAnimation:obj];

   [vw addSubview:obj];

   return (unsigned long)obj;
}

unsigned long addButton( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaButton *btn = [[CBCocoaButton alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];
   [btn setCustomId: customid];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [btn setTitle: s];
   [s release];

   [btn setTarget: delegate];
   [btn setAction:@selector(buttonClick:)];

   [btn setBezelStyle: NSRoundedBezelStyle];
   [vw addSubview:btn];

   [btn setVisible:true];

   return (unsigned long)btn;
}

unsigned long addCheckbox( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaButton *btn = [[CBCocoaButton alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];
   [btn setCustomId: customid];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [btn setTitle: s];
   [s release];

   [btn setTarget: delegate];
   [btn setAction:@selector(buttonClick:)];

   [btn setButtonType: NSSwitchButton];
   [vw addSubview:btn];

   [btn setVisible:true];

   return (unsigned long)btn;
}

unsigned long addRadioButton( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaButton *btn = [[CBCocoaButton alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];
   [btn setCustomId: customid];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [btn setTitle: s];
   [s release];

   [btn setTarget: delegate];
   [btn setAction:@selector(radiobuttonClick:)];

   [btn setButtonType: NSRadioButton];
   [vw addSubview:btn];

   [btn setVisible:true];

   addToRadiogroup( vw, btn );

   return (unsigned long)btn;
}

unsigned long addCombobox( int x, int y, int w, int h, const char *caption, unsigned long parent, unsigned long customid ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaCombobox *combo = [[CBCocoaCombobox alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];
   [combo setCustomId: customid];

   [combo setTarget: delegate];
   [combo setAction:@selector(comboClick:)];

   [vw addSubview:combo];

   return (unsigned long)combo;
}

unsigned long addScrollView( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;
   CBCocoaScrollView *scroll = [[CBCocoaScrollView alloc] initWithFrame: NSMakeRect(x, wh - y, w, h)];

   [scroll setBorderType: NSBezelBorder];
   [scroll setHasVerticalScroller: YES];
   [scroll setRulersVisible: NO];

   [vw addSubview: scroll];

   return (unsigned long)scroll;
}

unsigned long addTextView( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   CBCocoaScrollView *scroll = (CBCocoaScrollView *)addScrollView( x, y, w, h, caption, parent );
   [scroll setHasHorizontalScroller: YES];
   [scroll setAutohidesScrollers: YES];

   CBCocoaTextView *ed = [[CBCocoaTextView alloc] initWithFrame: NSMakeRect(0, 0, 10, 10)];
   [ed setScroll: scroll];
   [ed setRichText: YES];
   [ed setEditable: YES];
   [ed setHorizontallyResizable: YES];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [ed setString: s];
   [s release];

   [scroll setDocumentView: ed];

   return (unsigned long)ed;
}

unsigned long addListbox( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   CBCocoaScrollView *scroll = (CBCocoaScrollView *)addScrollView( x, y, w, h, caption, parent );
   CBCocoaTable *control = [[CBCocoaTable alloc] initWithFrame: NSMakeRect(0, 0, w - 17, 10)];
   [control setScroll: scroll];
   [scroll setDocumentView: control];
   [control setAllowsMultipleSelection: NO];
   [control setHeaderView: nil];

   return (unsigned long)control;
}

unsigned long addTableView( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   CBCocoaScrollView *scroll = (CBCocoaScrollView *)addScrollView( x, y, w, h, caption, parent );
   [scroll setHasHorizontalScroller: YES];

   CBCocoaTable *control = [[[CBCocoaTable alloc] initWithFrame: NSMakeRect(0, 0, w - 17, 10)] autorelease];
   [control setScroll: scroll];
   [scroll setDocumentView: control];
   [control setAllowsMultipleSelection: NO];
   [control setAllowsColumnReordering: NO];
   [control setAllowsColumnResizing: NO];

   return (unsigned long)control;
}

void setupTableViewDataSource( unsigned long obj, unsigned long customid, int columncount, CINTF_GRIDEDITABLEFUNC a, CINTF_GRIDNUMBEROFROWSFUNC b, CINTF_GRIDGETVALUEFUNC c, CINTF_GRIDSETVALUEFUNC d ) {
   CBCocoaTable *control = (CBCocoaTable *)obj;

   for ( int i = 0; i < columncount; i++ ) {
      NSTableColumn *col = [[[NSTableColumn alloc] initWithIdentifier: nil] autorelease];
      [control addTableColumn: col];
   }

   GridDataSource *data = [[GridDataSource alloc] autorelease];
   [data setthecustomid: customid];
   [data setfunceditable: a];
   [data setfuncnumberofrows: b];
   [data setfuncgetvalue: c];
   [data setfuncsetvalue: d];

   [control setDataSource: data];
   [control setDelegate: data];

   [control reloadData];
}

void setTableViewColumnCount( unsigned long obj, unsigned long newsize ) {
   CBCocoaTable *control = (CBCocoaTable *)obj;

   unsigned long c = [control numberOfColumns];
   for ( unsigned long i = c; i < newsize; i++ ) {
      NSTableColumn *col = [[[NSTableColumn alloc] initWithIdentifier: nil] autorelease];
      [control addTableColumn: col];
   }

   NSArray *tableColumns = [control tableColumns];
   for ( unsigned long j = newsize; j < c; j++ ) {
      NSTableColumn *col = [tableColumns objectAtIndex: j];
      [control removeTableColumn: col];
      [col release];
   }
}

void setTableViewColumnTitle( unsigned long obj, unsigned long index, const char *caption ) {
   CBCocoaTable *control = (CBCocoaTable *)obj;

   NSArray *tableColumns = [control tableColumns];
   NSTableColumn *col = [tableColumns objectAtIndex: index];
   NSTableHeaderCell *cell = [col headerCell];

   NSAutoreleasePool *tmpPool = [[NSAutoreleasePool alloc] init];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [cell setStringValue: s];
   [s release];

   [tmpPool release];
}

void setTableViewColumnWidth( unsigned long obj, unsigned long index, int width ) {
   CBCocoaTable *control = (CBCocoaTable *)obj;
   NSArray *tableColumns = [control tableColumns];
   NSTableColumn *col = [tableColumns objectAtIndex: index];

   [col setWidth: width];
}

void signalTableViewRowUpdate( unsigned long obj, long index ) {
   CBCocoaTable *control = (CBCocoaTable *)obj;

   if ( index == -1 ) {
      [control reloadData];
   } else {
      [control setNeedsDisplayInRect:[control rectOfRow:index]];
   }
}

void signalTableViewRowCountChanged( unsigned long obj, long start, long newlen ) {
   CBCocoaTable *control = (CBCocoaTable *)obj;

   NSAutoreleasePool *tmpPool = [[NSAutoreleasePool alloc] init];

   [control noteNumberOfRowsChanged];

   for ( long index = start; index < newlen; index++ ) {
      [control setNeedsDisplayInRect:[control rectOfRow:index]];
   }

   [tmpPool release];
}


//------------------------------------------------------------------------

unsigned long addTreeView( int x, int y, int w, int h, const char *caption, unsigned long parent ) {
   CBCocoaScrollView *scroll = (CBCocoaScrollView *)addScrollView( x, y, w, h, caption, parent );
   [scroll setHasHorizontalScroller: YES];

   CBCocoaTree *control = [[[CBCocoaTree alloc] initWithFrame: NSMakeRect(0, 0, w - 17, 10)] autorelease];
   [control setScroll: scroll];
   [scroll setDocumentView: control];
   [control setAllowsMultipleSelection: NO];
   [control setAllowsColumnReordering: NO];
   [control setAllowsColumnResizing: NO];
   [control setHeaderView: nil];

   return (unsigned long)control;
}

void setupTreeDataSource( unsigned long obj, unsigned long customid, CINTF_TREEGETCHILDCOUNTFUNC a, CINTF_TREEGETCHILDIDFUNC b, CINTF_TREEGETVALUEFUNC c, CINTF_TREEGETCUSTOMVALUEFUNC d, CINTF_TREESETCUSTOMVALUEFUNC e ) {
   CBCocoaTree *control = (CBCocoaTree *)obj;

   NSTableColumn *col = [[[NSTableColumn alloc] initWithIdentifier: nil] autorelease];
   [control addTableColumn: col];
   [col release];
   [control setOutlineTableColumn: col];
   [control setAutoresizesOutlineColumn: YES];

   TreeDataSource *data = [[[TreeDataSource alloc] init]autorelease];
   [data setthecustomid: customid];
   [data setfuncgetchildcount: a];
   [data setfuncgetchildid: b];
   [data setfuncgetvalue: c];
   [data setfuncgetcustomvalue: d];
   [data setfuncsetcustomvalue: e];

   [control setDataSource: data];
   [control setDelegate: control];

   [control reloadData];
}

void signalTreeFullUpdate( unsigned long obj ) {
   CBCocoaTree *control = (CBCocoaTree *)obj;

   [control reloadData];
}

//------------------------------------------------------------------------

unsigned long newMenu( const char *caption ) {
   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   NSMenu *menu = [[NSMenu alloc] initWithTitle: s];
   [s release];

   return (unsigned long)menu;
}

unsigned long addMenuItemSeparator( unsigned long parent ) {
   NSMenu *menu = (NSMenu *)parent;

   NSMenuItem *item;

   item = [NSMenuItem separatorItem];
   [menu addItem: item];

   return (unsigned long)item;
}

unsigned long addMenuItem( const char *caption, unsigned long parent, char controlkey ) {
   NSMenu *menu = (NSMenu *)parent;
   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];

   NSMenuItem *item;
   if ( controlkey != 0 ) {
      NSString *key = [[NSString alloc] initWithFormat: @"%c", controlkey];

      item = [menu addItemWithTitle: s
                      action: @selector(menuClick:)
                      keyEquivalent: key];

      [key release];
   } else {
      item = [menu addItemWithTitle: s
                      action: @selector(menuClick:)
                      keyEquivalent: @""];
   }

   [item setTarget: delegate];
   [s release];

   return (unsigned long)item;
}

void linkSubMenu( unsigned long parentmenu, unsigned long menuitem, unsigned long newmenu ) {
   NSMenu *m0     = (NSMenu *)parentmenu;
   NSMenuItem *m1 = (NSMenuItem *)menuitem;
   NSMenu *m2     = (NSMenu *)newmenu;

   [m0 setSubmenu: m2
          forItem: m1];
}

void setApplicationMenuBar( unsigned long obj ) {
   NSMenu *menu = (NSMenu *)obj;

   [app setMainMenu: menu];
}

//------------------------------------------------------------------------

void setContextMenu_NSTableView( unsigned long obj, unsigned long menu ) {
   CBCocoaTable *table = (CBCocoaTable *)obj;

   [table setContextMenu: (NSMenu *)menu];
}

const char *getText_NSTextView( unsigned long obj ) {
   NSTextView *tv = (NSTextView *)obj;

   NSString *s = [tv string];
   return [s UTF8String];
}

void setText_NSTextView( unsigned long obj, const char *caption ) {
   NSTextView *tv = (NSTextView *)obj;

   NSAutoreleasePool *tmpPool = [[NSAutoreleasePool alloc] init];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [tv setString: s];
   [s release];

   [tmpPool release];
}

long addItem_NSPopUpButton( unsigned long obj, const char *caption ) {
   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];

   NSPopUpButton *combo = (NSPopUpButton *)obj;
   [combo addItemWithTitle: s];

   [s release];

   return [combo numberOfItems];
}

void removeItem_NSPopUpButton( unsigned long obj, long index ) {
   NSPopUpButton *combo = (NSPopUpButton *)obj;

   [combo removeItemAtIndex: index];
}

void clearItems_NSPopUpButton( unsigned long obj ) {
   NSPopUpButton *combo = (NSPopUpButton *)obj;

   [combo removeAllItems];
}

long getSelected_NSPopUpButton( unsigned long obj ) {
   NSPopUpButton *combo = (NSPopUpButton *)obj;
   return [combo indexOfSelectedItem];
}

void setSelected_NSPopUpButton( unsigned long obj, long index ) {
   NSPopUpButton *combo = (NSPopUpButton *)obj;
   [combo selectItemAtIndex: index];
}

long getSelected_NSTableView( unsigned long obj ) {
   NSTableView *control = (NSTableView *)obj;
   return [control selectedRow];
}

void setSelected_NSTableView( unsigned long obj, long index ) {
   NSTableView *control = (NSTableView *)obj;

   NSIndexSet *indexset = [[NSIndexSet alloc] initWithIndex: index];
   [control selectRowIndexes: indexset
        byExtendingSelection: NO];
   [indexset release];
}

bool getState_NSButton( unsigned long obj ) {
   NSButton *check = (NSButton *)obj;

   if ([check intValue] == 1 ) {
      return true;
   } else {
      return false;
   }
}

void setState_NSButton( unsigned long obj, bool state, bool isRadio ) {
   NSButton *check = (NSButton *)obj;

   if ( state ) {
      [check setIntValue: 1];

      if ( isRadio ) {
         NSView *parent = [check superview];
         radiogroup *grp = findRadiogroup( parent );
         if ( grp != NULL ) {
            unsigned int c = grp->buttons.size();
            for ( unsigned int i = 0; i < c; i++ ) {
               id obj = grp->buttons[i];
               if ( obj != check ) {
                  [obj setIntValue: 0];
               }
            }
         }
      }
   } else {
      [check setIntValue: 0];
   }
}

void setProgressIndicatorValue( unsigned long obj, double value ) {
   NSProgressIndicator *pbar = (NSProgressIndicator *)obj;
   [pbar setDoubleValue: value];
}

void setProgressIndicatorIndetermined( unsigned long obj, bool indetermined ) {
   NSProgressIndicator *pbar = (NSProgressIndicator *)obj;
   if ( indetermined ) {
      [pbar setIndeterminate: YES];
   } else {
      [pbar setIndeterminate: NO];
   }
}

void setText_NSControl( unsigned long obj, const char *text ) {
   NSControl *control = (NSControl *)obj;

   NSAutoreleasePool *tmpPool = [[NSAutoreleasePool alloc] init];

   NSString *s = [[NSString alloc] initWithCString: text
                                          encoding: NSUTF8StringEncoding];
   [control setStringValue: s];
   [s release];

   [tmpPool release];

}

const char *getText_NSControl( unsigned long obj ) {
   NSControl *control = (NSControl *)obj;
   NSString *s = [control stringValue];

   return [s UTF8String];
}

void setText_NSButton( unsigned long obj, const char *text ) {
   NSButton *control = (NSButton *)obj;

   NSAutoreleasePool *tmpPool = [[NSAutoreleasePool alloc] init];

   NSString *s = [[NSString alloc] initWithCString: text
                                          encoding: NSUTF8StringEncoding];
   [control setTitle: s];
   [s release];

   [tmpPool release];

}

const char *getText_NSButton( unsigned long obj ) {
   NSButton *control = (NSButton *)obj;
   NSString *s = [control title];

   return [s UTF8String];
}

void setTitle_Container( unsigned long obj, const char *caption ) {
   NSWindow *control = (NSWindow *)obj;

   NSAutoreleasePool *tmpPool = [[NSAutoreleasePool alloc] init];

   NSString *s = [[NSString alloc] initWithCString: caption
                                          encoding: NSUTF8StringEncoding];
   [control setTitle: s];
   [s release];

   [tmpPool release];
}

unsigned long createFont( const char *name, int size ) {
   NSString *s = [[NSString alloc] initWithCString: name
                                          encoding: NSUTF8StringEncoding];
   NSFont *font = [NSFont fontWithName: s
                                  size: size];
   [s release];

   return (unsigned long)font;
}

void setFont_NSText( unsigned long obj, unsigned long fontid ) {
   NSText *control = (NSText *)obj;
   NSFont *font = (NSFont *)fontid;

   NSAutoreleasePool *tmpPool = [[NSAutoreleasePool alloc] init];
   [control setFont: font];
   [tmpPool release];
}


void cut_NSText( unsigned long obj ) {
   [(NSText *)obj cut: nil];
}

void copy_NSText( unsigned long obj ) {
   [(NSText *)obj copy: nil];
}

void paste_NSText( unsigned long obj ) {
   [(NSText *)obj paste: nil];
}

void selectAll_NSText( unsigned long obj ) {
   [(NSText *)obj selectAll: nil];
}

void getSize_NSView( unsigned long obj, long *w, long *h ) {
   if ( obj != 0 ) {
      NSWindow *win = (NSWindow *)obj;
      NSView *vw = [win contentView];

      w[0] = (long)[vw frame].size.width;
      h[0] = (long)[vw frame].size.height;
   } else {
      w[0] = iScreenWidth;
      h[0] = iScreenHeight;
   }
}

void reposition_NSView( unsigned long obj, unsigned long parent, int x, int y, int w, int h ) {
   NSInteger xx = x;
   NSInteger yy = y;
   NSInteger ww = w;
   NSInteger hh = h;

   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;

   NSInteger newh = wh - y;

   NSView *control = (NSView *)obj;
   [[control superview] setNeedsDisplayInRect: [control frame]];
   [control setFrame: NSMakeRect(xx, newh, ww, hh)];
   [control setNeedsDisplay: YES];
}

void reposition_CBCocoaTextView( unsigned long obj, unsigned long parent, int x, int y, int w, int h ) {
   NSInteger xx = x;
   NSInteger yy = y;
   NSInteger ww = w;
   NSInteger hh = h;

   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;

   NSInteger newh = wh - y;

   CBCocoaTextView *control = (CBCocoaTextView *)obj;
   CBCocoaScrollView *scroll = [control getScroll];
   [[scroll superview] setNeedsDisplayInRect: [scroll frame]];
   [scroll setFrame: NSMakeRect(xx, newh, ww, hh)];
   [scroll setNeedsDisplay: YES];

   // todo: control width resize?
}

void reposition_CBCocoaTable( unsigned long obj, unsigned long parent, int x, int y, int w, int h ) {
   NSInteger xx = x;
   NSInteger yy = y;
   NSInteger ww = w;
   NSInteger hh = h;

   NSWindow *win = (NSWindow *)parent;
   NSView *vw = [win contentView];
   CGFloat wh = [vw frame].size.height - h;

   NSInteger newh = wh - y;

   CBCocoaTable *control = (CBCocoaTable *)obj;
   CBCocoaScrollView *scroll = [control getScroll];
   [[scroll superview] setNeedsDisplayInRect: [scroll frame]];
   [scroll setFrame: NSMakeRect(xx, newh, ww, hh)];
   [scroll setNeedsDisplay: YES];
}

//------------------------------------------------------------------------

void showWindow( unsigned long obj ) {
   NSWindow *control = (NSWindow *)obj;

   [control makeKeyAndOrderFront: nil];
}

void closeWindow( unsigned long obj ) {
   NSWindow *control = (NSWindow *)obj;

   [control close];
}


//------------------------------------------------------------------------

int cocoaMessageBox( const char *caption, const char *text, const char *button1, const char *button2, const char *button3 ) {
   NSString *cap = nil;
   NSString *txt = nil;
   NSString *btn1 = nil;
   NSString *btn2 = nil;
   NSString *btn3 = nil;

   cap = [[NSString alloc] initWithCString: caption
                                 encoding: NSUTF8StringEncoding];
   txt = [[NSString alloc] initWithCString: text
                                 encoding: NSUTF8StringEncoding];
   if ( button1 != NULL ) {
      btn1 = [[NSString alloc] initWithCString: button1
                                    encoding: NSUTF8StringEncoding];
   }
   if ( button2 != NULL ) {
      btn2 = [[NSString alloc] initWithCString: button2
                                    encoding: NSUTF8StringEncoding];
   }
   if ( button3 != NULL ) {
      btn3 = [[NSString alloc] initWithCString: button3
                                    encoding: NSUTF8StringEncoding];
   }

   int iResponse = NSRunAlertPanel( cap, txt, btn1, btn2, btn3 );

   [btn3 release];
   [btn2 release];
   [btn1 release];
   [txt release];
   [cap release];

   switch( iResponse ) {
      case NSAlertDefaultReturn:
         return 1;
         break;
      case NSAlertAlternateReturn:
         return 2;
         break;
      case NSAlertOtherReturn:
         return 3;
         break;
   }

   return -1;
}

//------------------------------------------------------------------------

void initCocoa() {
   // very nasty hack to get the attention of the OS to our application
   CGCaptureAllDisplaysWithOptions( kCGCaptureNoFill );
   CGReleaseAllDisplays();

   CGDirectDisplayID iMainDsp = CGMainDisplayID();
   iScreenWidth = CGDisplayPixelsWide( iMainDsp );
   iScreenHeight = CGDisplayPixelsHigh( iMainDsp );

   pool = [[NSAutoreleasePool alloc] init];
   app = [NSApplication sharedApplication];
   delegate = [[AppController alloc] init];
   [app setDelegate: delegate];
   [app activateIgnoringOtherApps:true];
}

void stopCocoa() {
   [app stop: nil];
}

void finiCocoa() {
   [pool release];
}

void runCocoa( unsigned long mainwindow ) {
//   [NSBundle loadNibNamed:@"myMain" owner:app];

   mainWin = mainwindow;

   [app run];
}

