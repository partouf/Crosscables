
#ifndef __GFCALLABLE_H
#define __GFCALLABLE_H

#include "Freeable.h"

namespace Groundfloor {

/** TGFCallable defines an abstruct function execute() that
  * other objects may call upon.
  */
class Callable: public Groundfloor::Freeable
{
    public:
         /// abstract function to implement
        virtual void execute() = 0;
};

}

#endif // __GFCALLABLE_H
