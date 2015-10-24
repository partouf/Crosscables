#ifndef _JRBASESOCKET_H_
#define _JRBASESOCKET_H_

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Atoms/Freeable.h>
#include <Groundfloor/Molecules/String.h>
#include <Groundfloor/Molecules/Property.h>
#include <Groundfloor/Molecules/BaseCommunicator.h>

#include "Defines.h"
#include "NetAddress.h"

using namespace Groundfloor;

namespace Jumpropes {

   /** BaseSocket is a base class for both client- and server-oriented native sockets.
    *  Most of this code is based upon POSIX functions.
    */
   class BaseSocket : public BaseCommunicator
   {
   protected:
      int 		iSocketId;

      IPAddress aRemoteAddress;
      IPAddress aLocalAddress;

      bool loadLocalAddressInfo();
      bool loadRemoteAddressInfo();

      int       iConnectionType;
      int       iAddressType;

      void resetVars();

      /// Set the socket in blocking or non-blocking mode.
      /** It is prefered the socket stays in its Default Non-Blocking state,
        *  for connection purposes you may temporarily set this to blocking
        *  while the connection is being established.
        */
      void setBlocking(bool bBlocking);

   public:
      /// Creates a new native socket.
      BaseSocket();
      /// Initializes the socket with the given native socket identifier.
      BaseSocket(int iSocketId, const BaseSocket *aServer = NULL);
      ~BaseSocket();

      /// Default = true, splits data to send in chunks of [autosplit_size] length and chain-sends these chunks.
      /** Please just use JRThreadedSender.
        **/
      Property<bool> autosplit_sendbuffer;

      /// Default = 1024 bytes
      Property<unsigned int> autosplit_size;

      /// Reinitializes the current socket with a given native socket identifier.
      void reinitialize(int iSocketId);

      /// Returns the ip-address that is used on this machine for this socket.
      IPAddress *getLocalAddress() { return &aLocalAddress; }

      /// Returns the ip-address of the remote machine that is connected to this socket.
      IPAddress *getRemoteAddress() { return &aRemoteAddress; }

      Property<int>   remotePort;
      Property<int>   localPort;

      /// Returns when socket is ready to be written whether or not socket is blocking.
      /** This function is - internally - used to determine when to continue send_data(),
        *  therefor send_data() is always "blocking".
        * Don't override (or use for that matter) this function
        *  unless you know what you're doing.
        **/
      bool isReadyToSend();

      /// open socket
      bool connect();
      /// close socket
      bool disconnect();

      /// Send data to the remote machine via the current socket.
      /** Please mind comments on the autosplit variables */
      bool send(const String *sData, CommReturnData *errData = NULL);
      /// Return whatever data that was sent to this socket and that is currently stored in the socket buffer.
      bool receive(String *sData, CommReturnData *errData = NULL);
   };

}

#endif /*_JRBASESOCKET_H_*/
