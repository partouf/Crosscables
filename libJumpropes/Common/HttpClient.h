
#ifndef __JRHttpClient_H
#define __JRHttpClient_H

namespace JumpropesCommon {
   class HttpHeader;
   class HttpClient;
}

#include <Groundfloor/Molecules/Callbacks.h>
#include <Groundfloor/Molecules/Property.h>
#include <Groundfloor/Materials/FileWriter.h>
#include "HttpConnection.h"
#include "../ClientSocket.h"

#include "../URI.h"
#include "../LookupCache.h"

#define JRHTTPSTATUS_CONNECTED      1
#define JRHTTPSTATUS_DISCONNECTED   2
#define JRHTTPSTATUS_ERROR          3
#define JRHTTPSTATUS_HEADER         4
#define JRHTTPSTATUS_DONE           5

using namespace Groundfloor;
using namespace Jumpropes;

namespace JumpropesCommon {

   class HttpHeader : public Freeable {
   protected:
      void resetVars();
   public:
      HttpHeader();
      ~HttpHeader();

      void parse(String *sHeader);

      double httpversion;
      int httpstatus;
      String statusstring;

      String location;

      String contenttype;
      String connection;
      unsigned long contentlength;

      StringVector cookies;

      bool chunked;
   };

   class HttpClient : public Thread {
   protected:
      LookupBase *pLookupBase;

      LookupObject host;
      String path;
      int port;

      bool bHeaderParsed;
      unsigned long iHeaderSize;
      StringVectorRange rHeader;

      StringVectorRange lastChunkRange;
      unsigned long iChunkScanLastSize;
      bool bWaitForChunk;

      int iChunkCount;

      StringVectorRange rLastSend;

      HttpConnection *connection;
      BaseSocket *socket;
      bool bAlternateSocketUsed;

      String parsedHeader;

      void reset();
      void parseReceivedData();
      void sendGet();
      void parseHeader(int iSize);

      bool isEndOfChunkedData();

      void sendPost(const String *sDataType, const String *sData);

      void done();
      void locateAndParseHeader();

   public:
      HttpClient(LookupBase *pLookupBase);
      ~HttpClient();

      HttpHeader header;

      MultiNotify<int> onStatusUpdate;
      Property<double> useHttpVersion;

      MultiNotify<String *> onContent;

      String cookie;
      String referer;
      String useragent;

      void get(const URI *uri, bool bBlocking = false, BaseSocket *anAlternateSocket = NULL);
      void post(const URI *uri, const String *sDataType, const String *sData, bool bBlocking = false, BaseSocket *anAlternateSocket = NULL);

      String *getHeader();

      void execute();
   };

}

#endif // __JRHttpClient_H
