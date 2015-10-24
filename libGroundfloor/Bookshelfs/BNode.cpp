
#include "BNode.h"

//------------------------------------------------------------------------------

Groundfloor::BNodePath::BNodePath() : Groundfloor::Freeable() {
   current = -1;
}

Groundfloor::BNodePath::~BNodePath() {
}

void Groundfloor::BNodePath::append( unsigned int i ) {
   path.push_back( i );
}

void Groundfloor::BNodePath::clear() {
   path.clear();
   current = -1;
}

unsigned int Groundfloor::BNodePath::traverse_reset() {
   current = -1;

   return current;
}

unsigned int Groundfloor::BNodePath::traverse_previous() {
   current--;

   return path[current];
}

unsigned int Groundfloor::BNodePath::traverse_next() {
   current++;

   return path[current];
}

bool Groundfloor::BNodePath::traverse_isBoF() {
   return (current <= -1);
}

bool Groundfloor::BNodePath::traverse_isEoF() {
   int c = path.size() - 1;
   return (current >= c);
}

void Groundfloor::BNodePath::readFromString( const char *sPath ) {
    this->clear();

    unsigned int len = strlen( sPath );
    Groundfloor::String nextItem;
    for ( unsigned int i = 0; i < len; i++ ) {
        char c = sPath[i];
        if ( (c >= '0') && (c <= '9') ) {
            nextItem.append( c );
        } else {
            unsigned int p = atoi( nextItem.getValue() );
            this->append( p );
            nextItem.setLength( 0 );
        }
    }
    if ( nextItem.getLength() != 0 ) {
        unsigned int p = atoi( nextItem.getValue() );
        this->append( p );
    }
}

//------------------------------------------------------------------------------

Groundfloor::BNode::BNode() : Groundfloor::Freeable() {
   arrAttributes = NULL;
   arrChildren = NULL;
}

Groundfloor::BNode::BNode( const char *sName ) : Groundfloor::Freeable() {
   arrAttributes = NULL;
   arrChildren = NULL;

   parent = NULL;

   name.setValue_ansi( sName );
}

Groundfloor::BNode::~BNode() {
   if ( arrAttributes != NULL ) {
      delete arrAttributes;
   }
   if ( arrChildren != NULL ) {
      delete arrChildren;
   }
}

Groundfloor::BNode *Groundfloor::BNode::getParentNode() {
   return parent;
}

void Groundfloor::BNode::addAttribute( Groundfloor::BAttribute *anAttrib ) {
   if ( arrAttributes == NULL ) {
      arrAttributes = new Groundfloor::Vector();
   }

   arrAttributes->addElement( anAttrib );
}

void Groundfloor::BNode::addChildNode( Groundfloor::BNode *aNode ) {
   if ( arrChildren == NULL ) {
      arrChildren = new Groundfloor::Vector();
   }

   arrChildren->addElement( aNode );
   aNode->parent = this;
}

unsigned int Groundfloor::BNode::childCount() const {
   if ( arrChildren != NULL ) {
      return arrChildren->size();
   }

   return 0;
}

unsigned int Groundfloor::BNode::attributeCount() const {
   if ( arrAttributes != NULL ) {
      return arrAttributes->size();
   }

   return 0;
}

Groundfloor::BAttribute *Groundfloor::BNode::getAttribute( unsigned int i ) const {
   if ( arrAttributes != NULL ) {
      return static_cast<Groundfloor::BAttribute *>( arrAttributes->elementAt( i ) );
   }

   return NULL;
}

Groundfloor::BAttribute *Groundfloor::BNode::getAttributeByName( const char *sName ) const {
   if ( arrAttributes != NULL ) {
      unsigned int c = arrAttributes->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         Groundfloor::BAttribute *attrib = static_cast<Groundfloor::BAttribute *>( arrAttributes->elementAt( i ) );
         if ( attrib->key.match_ansi( sName ) ) {
            return attrib;
         }
      }
   }

   return NULL;
}

void Groundfloor::BNode::copyAttributes( Groundfloor::BNode *aNode ) {
    unsigned int c = aNode->attributeCount();
    for ( unsigned int i = 0; i < c; i++ ) {
        Groundfloor::BAttribute *attrib = new Groundfloor::BAttribute( static_cast<Groundfloor::BAttribute *>( aNode->arrAttributes->elementAt( i ) ) );
        this->addAttribute( attrib );
    }
}


Groundfloor::BNode *Groundfloor::BNode::getChildByName( const char *sName ) const {
   if ( arrChildren != NULL ) {
      unsigned int c = arrChildren->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         Groundfloor::BNode *child = static_cast<Groundfloor::BNode *>( arrChildren->elementAt( i ) );
         if ( child->name.match_ansi( sName ) ) {
            return child;
         }
      }
   }

   return NULL;
}

Groundfloor::BNode *Groundfloor::BNode::getChildByNameAndAttribute( const char *sName, const char *sAttributeName, const Groundfloor::BValue *sAttributeValue ) const {
   if ( arrChildren != NULL ) {
      unsigned int c = arrChildren->size();
      for ( unsigned int i = 0; i < c; i++ ) {
         Groundfloor::BNode *child = static_cast<Groundfloor::BNode *>( arrChildren->elementAt( i ) );
         if ( child->name.match_ansi( sName ) ) {
            Groundfloor::BAttribute *attrib = child->getAttributeByName( sAttributeName );
            if ( attrib != NULL ) {
               if ( attrib->value.equals( sAttributeValue ) ) {
                  return child;
               }
            } else {
               if ( sAttributeValue == NULL ) {
                  return child;
               }
            }
         }
      }
   }

   return NULL;
}

Groundfloor::BNode *Groundfloor::BNode::findAttributeThroughRecursiveSearch( const char *sAttributeName, const Groundfloor::BValue *sAttributeValue ) {
    Groundfloor::BAttribute *attrib = this->getAttributeByName( sAttributeName );
    if ( attrib != NULL ) {
        if ( attrib->value.equals( sAttributeValue ) ) {
            return this;
        }
    }

    if ( arrChildren != NULL ) {
        unsigned int c = arrChildren->size();
        for ( unsigned int i = 0; i < c; i++ ) {
            Groundfloor::BNode *child = static_cast<Groundfloor::BNode *>( arrChildren->elementAt( i ) );

            if ( child != NULL ) {
                Groundfloor::BNode *foundNode = child->findAttributeThroughRecursiveSearch( sAttributeName, sAttributeValue );
                if ( foundNode != NULL ) {
                    return foundNode;
                }
            }
        }
    }

    return NULL;
}

Groundfloor::BNode *Groundfloor::BNode::getChild( unsigned int i ) const {
   if ( arrChildren != NULL ) {
      return static_cast<Groundfloor::BNode *>( arrChildren->elementAt( i ) );
   }

   return NULL;
}

Groundfloor::BAttribute *Groundfloor::BNode::findOrCreateAttribute( const char *sName ) {
   Groundfloor::BAttribute *attr = getAttributeByName( sName );
   if ( attr == NULL ) {
      attr = new Groundfloor::BAttribute( sName );
      addAttribute( attr );
   }

   return attr;
}

Groundfloor::String *Groundfloor::BNode::getFirstChildsContent() {
    if ( arrChildren != NULL ) {
        if ( arrChildren->size() != 0 ) {
            Groundfloor::BNode *child = static_cast<Groundfloor::BNode *>( arrChildren->elementAt(0) );
            if ( child != NULL ) {
                return &child->content;
            }
        }
    }

    return &content;
}

Groundfloor::String *Groundfloor::BNode::flattenChildContent() {
    Groundfloor::String *sNewContent = new Groundfloor::String();
    sNewContent->append( &content );

    if ( arrChildren != NULL ) {
        unsigned int c = arrChildren->size();
        for ( unsigned int i = 0; i < c; i++ ) {
            Groundfloor::BNode *child = static_cast<Groundfloor::BNode *>( arrChildren->elementAt(i) );
            if ( child != NULL ) {
                Groundfloor::String *tmp = child->flattenChildContent();
                sNewContent->append( tmp );
                delete tmp;
                if ( (i+1) != c ) {
                    if ( sNewContent->getLength() != 0 ) {
                        sNewContent->append_ansi( " " );
                    }
                }
            }
        }
    }

    return sNewContent;
}

//--------------------------------------------

Groundfloor::BNode *Groundfloor::BNode::getChildAndTraverse( Groundfloor::BNodePath *aPath ) const {
   if ( !aPath->traverse_isEoF() ) {
      unsigned int i = aPath->traverse_next();

      return getChild( i );
   }

   return NULL;
}

Groundfloor::BNode *Groundfloor::BNode::getChildByPath( Groundfloor::BNodePath *aPath ) {
   Groundfloor::BNode *newChild = this->getChildAndTraverse( aPath );
   if ( newChild != NULL ) {
      return newChild->getChildByPath( aPath );
   } else {
      return this;
   }
}

