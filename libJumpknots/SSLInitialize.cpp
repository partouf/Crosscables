
#include "SSLInitialize.h"
#include <Groundfloor/Atoms/Defines.h>

#include <openssl/ssl.h>
#include <openssl/engine.h>
#include <openssl/err.h>

bool Jumpknots::initJumpknots() {
   if ( !SSL_library_init() ) {
      printf( "Error initializing Jumpknots!\n" );
      
      return false;
   }
   SSL_load_error_strings();
   
   // not a mac specific thing, it seems my mac has version 0.9.7l of OpenSSL
   //  which just doesn't have these functions.
   #ifndef GF_OS_MAC
   ERR_load_BIO_strings();

   OpenSSL_add_all_algorithms();
   #endif
   
   return true;
}

void Jumpknots::finiJumpknots() {
   EVP_cleanup();
   ERR_remove_state(0);
   ERR_free_strings();
   ENGINE_cleanup();
   //SSL_free_comp_methods();
   CRYPTO_cleanup_all_ex_data();

//   CONF_modules_unload();
}
