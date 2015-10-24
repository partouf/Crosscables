
#include "HttpDownloader.h"

JumpropesCommon::HttpDownloader::HttpDownloader( LookupBase *aLookup, double dHttpVersion, const char *sUseragent ) : Freeable() {
   client = new HttpClient( aLookup );
   uri = new URI( aLookup );

   client->useHttpVersion.set( dHttpVersion );
   client->useragent.setValue_ansi( sUseragent );

   client->onContent.addNotify( GFCreateNotify( String *, HttpDownloader, this, &JumpropesCommon::HttpDownloader::onData ) );
}

JumpropesCommon::HttpDownloader::~HttpDownloader() {
   delete uri;
   delete client;
}

void JumpropesCommon::HttpDownloader::download( const char *sUri, const char *sOutputFile, BaseSocket *anAlternateSocket ) {
   uri->parseUri( sUri, true );

   // empty file if it already exists
   fw.open( sOutputFile, false );
   fw.close();

   fw.open( sOutputFile, true );
   fw.start();

   client->get( uri, true, anAlternateSocket );
}

void JumpropesCommon::HttpDownloader::waitToFinish() {
   while ( client->isRunning() ) {
      GFMillisleep(10);
   }

   while ( !fw.isEmpty() ) {
      GFMillisleep(10);
   }

   fw.stopAndWait();
}

void JumpropesCommon::HttpDownloader::onData( String *s ) {
   fw.add( s );
}
