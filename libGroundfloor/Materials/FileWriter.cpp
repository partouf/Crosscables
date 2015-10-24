
#include "FileWriter.h"



//-----------------------------------------------------------------------------

Groundfloor::BaseWriter::BaseWriter() : Groundfloor::ThreadedBuffer() {
   this->aBuffer.resizeVector(100);
}

Groundfloor::BaseWriter::~BaseWriter() {
}

void Groundfloor::BaseWriter::add(const Groundfloor::Freeable *obj) {
   Groundfloor::String *sDataCopy = new Groundfloor::String(static_cast<const Groundfloor::String *>(obj));

   Groundfloor::ThreadedBuffer::add(sDataCopy);
}

//-----------------------------------------------------------------------------

Groundfloor::StringWriter::StringWriter() : Groundfloor::BaseWriter() {
}

Groundfloor::StringWriter::~StringWriter() {
}

bool Groundfloor::StringWriter::processObject(Groundfloor::Freeable *obj) {
   Groundfloor::String *str = static_cast<Groundfloor::String *>(obj);

   Output.addChunk(str);

   return true;
}

//-----------------------------------------------------------------------------

Groundfloor::FileWriter::FileWriter() : Groundfloor::BaseWriter() {
   bAppendmode = false;
}

Groundfloor::FileWriter::~FileWriter() {
}

void Groundfloor::FileWriter::open(const Groundfloor::String *sFile, bool bAppendmode) {
   open(sFile->getValue(), bAppendmode);
}

void Groundfloor::FileWriter::open(const char *sFile, bool bAppendmode) {
   this->bAppendmode = bAppendmode;
   file.filename.set(sFile);

   if (bAppendmode) {
      file.mode.set(GFFILEMODE_WRITE | GFFILEMODE_APPEND);
   }
   else {
      file.mode.set(GFFILEMODE_WRITE);
      file.connect();
   }
}

void Groundfloor::FileWriter::close() {
   if (!bAppendmode) {
      file.disconnect();
   }
}

void Groundfloor::FileWriter::setFlushDirectly(bool yesno) {
   file.flushdirectly.set(yesno);
}

bool Groundfloor::FileWriter::processObject(Groundfloor::Freeable *obj) {
   Groundfloor::String *str = static_cast<Groundfloor::String *>(obj);

   if (bAppendmode) {
      file.connect();
      file.send(str);
      file.disconnect();
   }
   else {
      file.send(str);
   }

   delete str;

   return true;
}

