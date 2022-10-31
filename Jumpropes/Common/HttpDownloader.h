
#ifndef __JRHttpDownloader_H
#define __JRHttpDownloader_H

#include <Groundfloor/Atoms/Freeable.h>
#include <Groundfloor/Materials/FileWriter.h>
#include "HttpClient.h"

using namespace Groundfloor;
using namespace Jumpropes;

namespace JumpropesCommon {

   class HttpDownloader : Freeable {
   protected:
      FileWriter fw;
      HttpClient *client;
      URI *uri;
   public:
      HttpDownloader(LookupBase *aLookup, double dHttpVersion, const char *sUseragent);
      ~HttpDownloader();

      void download(const char *sUri, const char *sOutputFile, BaseSocket *anAlternateSocket = NULL);
      void waitToFinish();

      void onData(String *s);
   };

}

#endif //__JRHttpDownloader_H
