
#include "CallbackThread.h"


Groundfloor::CallbackThread::CallbackThread( Groundfloor::Callable *anObject, int iSleeptime ) : Groundfloor::Thread() {
    this->setInterval( iSleeptime );
    this->aCallableObject   = anObject;
}

Groundfloor::CallbackThread::~CallbackThread() {
}

void Groundfloor::CallbackThread::execute() {
    aCallableObject->execute();
}


Groundfloor::Timer::Timer() {
   setInterval( 1000 );
}

Groundfloor::Timer::~Timer() {
}

void Groundfloor::Timer::execute() {
   onTimerEvent.execute( this );
}
