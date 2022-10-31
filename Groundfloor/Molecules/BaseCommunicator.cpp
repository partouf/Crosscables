
#include "BaseCommunicator.h"

Groundfloor::CommReturnData::CommReturnData() : Groundfloor::Freeable() {
   eof = false;
   error = false;
   errorcode = 0;
   affected = 0;
}

Groundfloor::CommReturnData::~CommReturnData() {
}

Groundfloor::BaseCommunicator::BaseCommunicator() : Groundfloor::Freeable() {
   bConnected = false;
}

Groundfloor::BaseCommunicator::~BaseCommunicator() {
}

bool Groundfloor::BaseCommunicator::isConnected() {
   return bConnected;
}
