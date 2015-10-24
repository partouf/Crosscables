
#include "RemoteSquirrel.h"


Groundfloor::RemoteSQL::RemoteSQL( Groundfloor::RemoteSquirrelConnection *pConnection  ) : Groundfloor::AbstractSquirrel( pConnection ) {
   params.resizeVector(10);
}

Groundfloor::RemoteSQL::~RemoteSQL() {
}

void Groundfloor::RemoteSQL::prepareQuery() {
   Groundfloor::String n;

   unsigned int c = params.size();
   for ( unsigned int i = 0; i < c; i++ ) {
      Groundfloor::SimpleStringLookupObject *obj = static_cast<Groundfloor::SimpleStringLookupObject *>( params.elementAt(i) );

      n.setValue(":",1);
      n.append( &obj->name );
      this->replaceInQuery( n.getValue(), static_cast<Groundfloor::BValue *>(obj->object) );
   }
}

Groundfloor::BValue *Groundfloor::RemoteSQL::findOrAddParam( const Groundfloor::String *sName ) {
   Groundfloor::BValue *p = static_cast<Groundfloor::BValue *>( params.getObjectByString( sName ) );
   if ( p == NULL ) {
      p = new Groundfloor::BValue();
      // sorted insert, so we dont have to sort the parameters afterwards
      params.insertObjectSortedByLength( sName, p, false );
   }

   return p;
}

Groundfloor::BValue *Groundfloor::RemoteSQL::findOrAddParam( const char *sName ) {
   Groundfloor::BValue *p = static_cast<Groundfloor::BValue *>( params.getObjectByString( sName ) );
   if ( p == NULL ) {
      p = new Groundfloor::BValue();
      // sorted insert, so we dont have to sort the parameters afterwards
      params.insertObjectSortedByLength( sName, p, false );
   }

   return p;
}

void Groundfloor::RemoteSQL::setQuery( const Groundfloor::String *sQuery, bool bReplaceBackticksWithDoubleQuotes ) {
    sCurrentQuery.setValue( sQuery );
    if ( bReplaceBackticksWithDoubleQuotes ) {
        // \"ID\" is a ANSI-SQL way to make sure the interpreter knows ID is an identifier,
        //  it is however the most annoying character to use in strings and backticks `ID` are way easier
        //  MySQL uses backticks by default, so a MySQL implementation doesn't need this replace command unless in ANSI mode
        sCurrentQuery.replace_ansi( "`", "\"" );
    }
}

Groundfloor::String *Groundfloor::RemoteSQL::getCurrentQuery() {
    return &sCurrentQuery;
}

// ------------

Groundfloor::RemoteSquirrelConnection::RemoteSquirrelConnection() : Groundfloor::AbstractSquirrelConnection() {
}

Groundfloor::RemoteSquirrelConnection::~RemoteSquirrelConnection() {
}

Groundfloor::LocalSquirrelConnection::LocalSquirrelConnection() : Groundfloor::AbstractSquirrelConnection() {
}

Groundfloor::LocalSquirrelConnection::~LocalSquirrelConnection() {
}
