
#ifndef __GFFileWriter_H
#define __GFFileWriter_H


#include "ThreadedBuffer.h"
#include "../Atoms/Freeable.h"
#include "../Molecules/String.h"
#include "../Materials/FileCommunicator.h"
#include "../Molecules/StringVector.h"

namespace Groundfloor {

   class BaseWriter : public ThreadedBuffer {
   public:
      BaseWriter();
      ~BaseWriter();

      /// obj is assumed to be a TGFString that is copied to a new TGFString and placed in the buffer
      virtual void add(const Freeable *obj);
   };

   class StringWriter : public BaseWriter {
   public:
      StringWriter();
      ~StringWriter();

      StringVector Output;

      bool processObject(Freeable *obj);
   };

   /** TGFFileWriter is a Threaded Buffer that periodically writes
     *  data from the buffer to a file.
     * Warning: if you prematurely delete this object, data might be lossed.
     *  To prevent loss of data, implement a protection loop by e.g.: while ( !filewriter.isEmpty() ) { GFMilliSleep(100); );
    */
   class FileWriter : public BaseWriter {
   protected:
      FileCommunicator file;
      bool bAppendmode;

   public:
      FileWriter();
      ~FileWriter();

      void setFlushDirectly(bool yesno);

      /// opens file in Write mode (or Append mode if bAppendMode is true), sFile must be in UTF8
      void open(const String *sFile, bool bAppendmode = false);
      /// opens file in Write mode (or Append mode if bAppendMode is true), sFile must be in UTF8
      void open(const char *sFile, bool bAppendmode = false);
      /// closes file (warning: does not automatically flush buffer!)
      void close();

      bool processObject(Freeable *obj);
   };

}

#endif // __GFFileWriter_H

