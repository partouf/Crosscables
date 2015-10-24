
#include "CBXMLInterface.h"

#include "CBDriver.h"
#include "CBControls.h"

#include <cassert>


//-----------------------------------------------------------------------------

TCBXMLInterface::TCBXMLInterface() : Freeable() {
    controlcount = 0;

}

TCBXMLInterface::~TCBXMLInterface() {
}


//-----------------------------------------------------------------------------

BNode *TCBXMLInterface::newMagnetAsNode( TCBMagnet *magnet ) {
    controlcount++;

    BNode *node = new BNode();

    typeToBasicName( magnet->iRemoteType, &node->name );

    BAttribute *id = new BAttribute( "identifier", controlcount );
    id->value.asString()->prepend_ansi( "control" );
    if ( magnet->identifier.getLength() > 0 ) {
       id->value.setString( magnet->identifier.link() );
    }
    node->addAttribute( id );
    node->addAttribute( new BAttribute( "caption", magnet->caption.link() ) );
    node->addAttribute( new BAttribute( "x", magnet->x ) );
    node->addAttribute( new BAttribute( "y", magnet->y ) );
    node->addAttribute( new BAttribute( "w", magnet->w ) );
    node->addAttribute( new BAttribute( "h", magnet->h ) );

    return node;
}

void TCBXMLInterface::magnetsToNodes( BNode *parentnode, TCBMagnet *magnet ) {
    BNode *node = newMagnetAsNode( magnet );
    parentnode->addChildNode( node );

    if ( magnet->iRemoteType >= 100 ) {
        Vector *v = static_cast<TCBCollector *>( magnet )->getVector();
        unsigned int c = v->size();
        for ( unsigned int i = 0; i < c; i++ ) {
            TCBMagnet *child = static_cast<TCBMagnet *>( v->elementAt(i) );
            if ( child != NULL ) {
                if ( child->iRemoteType != CB_TYPE_MENU ) {
                    magnetsToNodes( node, child );
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------

TCBMagnet *TCBXMLInterface::recursivelyAddNodesAsMagnets( TCBCollector *parent, BNode *node ) {
    long t = nameToType( &node->name );

    TCBMagnet *magnet = NULL;

    switch ( t ) {
        case CB_TYPE_LABEL: magnet = new TCBLabel(); break;
        case CB_TYPE_EDITBOX: magnet = new TCBEditbox(); break;
        case CB_TYPE_MULTILINE: magnet = new TCBMultiline(); break;
        case CB_TYPE_LISTBOX: magnet = new TCBListbox(); break;
        case CB_TYPE_COMBOBOX: magnet = new TCBCombobox(); break;
        case CB_TYPE_BUTTON: magnet = new TCBButton(); break;
        case CB_TYPE_CHECKBOX: magnet = new TCBCheckbox(); break;
        case CB_TYPE_RADIOBUTTON: magnet = new TCBRadiobutton(); break;
        case CB_TYPE_PROGRESSBAR: magnet = new TCBProgressbar(); break;
        case CB_TYPE_GRID: magnet = new TCBGrid(); break;
        case CB_TYPE_TREE: magnet = new TCBTree(); break;
        case CB_TYPE_WINDOW: magnet = new TCBWindow(); break;
        case CB_TYPE_GROUPBOX: magnet = new TCBGroupbox(); break;
        case CB_TYPE_PANEL: magnet = new TCBPanel(); break;
        case CB_TYPE_NOTEBOOK: magnet = new TCBNotebook(); break;
        case CB_TYPE_NOTEBOOKTAB: magnet = new TCBNotebookTab(); break;
        case CB_TYPE_SCREEN: magnet = new TCBScreen(); break;
        case CB_TYPE_MENU: magnet = new TCBMenu(); break;
        default: return magnet; break;
    }

    BAttribute *attr;
    attr = node->getAttributeByName( "identifier" );
    if ( attr != NULL ) {
       magnet->identifier.set( attr->value.asString()->getValue() );
    }
    attr = node->getAttributeByName( "caption" );
    if ( attr != NULL ) {
       magnet->caption.set( attr->value.asString()->getValue() );
    }
    attr = node->getAttributeByName( "x" );
    if ( attr != NULL ) {
      magnet->x = attr->value.asInteger();
    }
    attr = node->getAttributeByName( "y" );
    if ( attr != NULL ) {
      magnet->y = attr->value.asInteger();
    }
    attr = node->getAttributeByName( "w" );
    if ( attr != NULL ) {
      magnet->w = attr->value.asInteger();
    }
    attr = node->getAttributeByName( "h" );
    if ( attr != NULL ) {
      magnet->h = attr->value.asInteger();
    }

    attr = node->getAttributeByName( "visible" );
    if ( attr != NULL ) {
       magnet->visible.set( attr->value.asBoolean() );
    }

    attr = node->getAttributeByName( "onHide" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onHide.addNotify( p );
    }
    attr = node->getAttributeByName( "onShow" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onShow.addNotify( p );
    }
    attr = node->getAttributeByName( "onPaint" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onPaint.addNotify( p );
    }
    attr = node->getAttributeByName( "onFocus" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onFocus.addNotify( p );
    }
    attr = node->getAttributeByName( "onChange" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onChange.addNotify( p );
    }
    attr = node->getAttributeByName( "onClick" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onClick.addNotify( p );
    }
    attr = node->getAttributeByName( "onDoubleClick" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onDoubleClick.addNotify( p );
    }
    attr = node->getAttributeByName( "onContextMenu" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        magnet->onContextMenu.addNotify( p );
    }

    attr = node->getAttributeByName( "onMove" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        static_cast<TCBCollector *>( magnet )->onMove.addNotify( p );
    }
    attr = node->getAttributeByName( "onResize" );
    if ( attr != NULL ) {
        CBMagnetEvent *p = static_cast<CBMagnetEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        static_cast<TCBCollector *>( magnet )->onResize.addNotify( p );
    }
    attr = node->getAttributeByName( "onKeyDown" );
    if ( attr != NULL ) {
        CBKeyEvent *p = static_cast<CBKeyEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        static_cast<TCBCollector *>( magnet )->onKeyDown.addNotify( p );
    }
    attr = node->getAttributeByName( "onKeyUp" );
    if ( attr != NULL ) {
        CBKeyEvent *p = static_cast<CBKeyEvent *>( lstFunctions.getObjectByString( attr->value.asString() ) );
        assert( p != NULL );
        static_cast<TCBCollector *>( magnet )->onKeyUp.addNotify( p );
    }

    magnet->parent.set( parent );

    if ( magnet->iRemoteType >= 100 ) {
        unsigned int c = node->childCount();
        for ( unsigned int i = 0; i < c; i++ ) {
            BNode *child = node->getChild(i);
            if ( child != NULL ) {
                recursivelyAddNodesAsMagnets( static_cast<TCBCollector *>( magnet ), child );
            }
        }
    }

	return magnet;
}


TCBMagnet *TCBXMLInterface::addNodesAsMagnets( TCBScreen *aScreen, BNode *aInterfaceNode ) {
	TCBMagnet *mainmagnet = NULL;

    BNode *screennode = aInterfaceNode->getChildByName( "screen" );
    if ( screennode != NULL ) {
        unsigned int c = screennode->childCount();
        for ( unsigned int i = 0; i < c; i++ ) {
            BNode *child = screennode->getChild(i);
            if ( child != NULL ) {
                mainmagnet = recursivelyAddNodesAsMagnets( aScreen, child );
            }
        }
    }

	return mainmagnet;
}

//-----------------------------------------------------------------------------


