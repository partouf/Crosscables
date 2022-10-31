
#ifndef __CBXMLInterface_H
#define __CBXMLInterface_H


#include <Groundfloor/Atoms/Freeable.h>
#include <Groundfloor/Materials/StringLookup.h>
#include <Groundfloor/Bookshelfs/BNode.h>
#include "CBBase.h"

using namespace Groundfloor;

class TCBXMLInterface: public Freeable
{
    protected:
        long controlcount;

        TCBMagnet *recursivelyAddNodesAsMagnets( TCBCollector *parent, BNode *node );

    public:
        SimpleStringLookup lstFunctions;

        TCBXMLInterface();
        ~TCBXMLInterface();

        BNode *newMagnetAsNode( TCBMagnet *magnet );
        void magnetsToNodes( BNode *parentnode, TCBMagnet *magnet );

        TCBMagnet *addNodesAsMagnets( TCBScreen *aScreen, BNode *aInterfaceNode );
};


#endif // __InterfaceXml_H
