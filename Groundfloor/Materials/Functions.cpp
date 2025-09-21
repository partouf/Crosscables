

#include "Functions.h"
#include "../Atoms/Defines.h"
#include <math.h>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <cstring>


#if _MSC_VER >= 1400
#define nativepowerfunction_integers(a,b)                           pow(a + 0.0,b)
#else
#define nativepowerfunction_integers(a,b)                           pow(a,b)
#endif

#ifdef GF_OS_LINUX
#include <cstring>
#endif


// statische geindexeerde 'tabellen' voor vertalingsfuncties van en naar hex en base64
const char *sBase64Table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char *sHextable = "0123456789ABCDEF";


int Groundfloor::strpos(const char *s, char c) {
   unsigned int len = strlen(s);
   for (unsigned int i = 0; i < len; i++) {
      if (s[i] == c) {
         return i;
      }
   }
   return -1;
}

void Groundfloor::LongToHex(unsigned long i, Groundfloor::String *sOutput) {
   unsigned int a;
   unsigned int k;
   unsigned long mask;

   int c = sizeof(unsigned long) * 2;
   for (int j = 0; j < c; j++) {
      k = j * 4;
      mask = (unsigned long)15 << k;
      a = (i & mask) >> k;

      sOutput->prepend(sHextable[a]);
   }
}

unsigned int Groundfloor::HexToInt(const Groundfloor::String *sInput) {
   unsigned int len = sInput->getLength() - 1;

   char *s = sInput->getValue();
   unsigned int i, p;
   unsigned int res = 0;

   for (i = 0; i <= len; i++) {
      p = Groundfloor::strpos(sHextable, s[len - i]);
      if (p == -1) {
         return 0;
      }

      res += p << (i * 4);
   }

   return res;
}


void CharToBin(unsigned char c, Groundfloor::String *sOutput) {
   int i;
   unsigned char c2;

   for (i = 0; i < 8; i++) {
      c2 = 1;
      c2 <<= (7 - i);

      if ((c & c2) > 0) {
         sOutput->append_ansi("1");
      }
      else {
         sOutput->append_ansi("0");
      }
   }
}

void StrToBin(Groundfloor::String *sInput, Groundfloor::String *sOutput) {
   char *sI = sInput->getValue();

   sOutput->setSize(static_cast<unsigned int>(sInput->getSize() * 2));
   sOutput->setLength(0);

   unsigned int i;
   for (i = 0; i < sInput->getLength(); i++) {
      CharToBin(sI[i], sOutput);
   }
}


void Groundfloor::CharToHex(char c, Groundfloor::String *sOutput) {
   char i, j;

   i = (c & 0xF0) >> 4;
   j = (c & 0x0F);

   sOutput->append(sHextable[i]);
   sOutput->append(sHextable[j]);
}

void Groundfloor::StrToHex(const Groundfloor::String *sInput, Groundfloor::String *sOutput) {
   char *sI = sInput->getValue();

   sOutput->setSize(static_cast<unsigned int>(sInput->getSize() * 2));
   sOutput->setLength(0);

   unsigned int i;
   for (i = 0; i < sInput->getLength(); i++) {
      Groundfloor::CharToHex(sI[i], sOutput);
   }
}


void CharsToAscii85(char sInput[], Groundfloor::String *sOutput) {
   unsigned int i = 0;
   char a, b, c, d, e;

   i += sInput[0] << 24;
   i += sInput[1] << 16;
   i += sInput[2] << 8;
   i += sInput[3];

   if (i != 0) {
      a = (i / static_cast<int>(nativepowerfunction_integers(85, 4))) % 85;
      b = (i / static_cast<int>(nativepowerfunction_integers(85, 3))) % 85;
      c = (i / static_cast<int>(nativepowerfunction_integers(85, 2))) % 85;
      d = (i / static_cast<int>(nativepowerfunction_integers(85, 1))) % 85;
      e = i % 85;

      sOutput->append(33 + a);
      sOutput->append(33 + b);
      sOutput->append(33 + c);
      sOutput->append(33 + d);
      sOutput->append(33 + e);
   }
   else {
      sOutput->append('z');
   }
}



void StrToAscii85(Groundfloor::String *sInput, Groundfloor::String *sOutput) {
   char *sI = sInput->getValue();
   char ic[4] = { 0,0,0,0 };

   sOutput->setSize(static_cast<unsigned int>(sInput->getSize() / 100 * 125));
   sOutput->setLength(0);

   sOutput->append("<~", 2);

   unsigned int i = 0;
   int j = 0;
   int inl = 0;
   while (i < sInput->getLength()) {
      ic[j] = sI[i];

      j++;
      i++;

      j %= 4;

      if (j == 0) {
         CharsToAscii85(ic, sOutput);
         inl++;
      }
   }


   if (j == 3) {
      ic[3] = 0;
      CharsToAscii85(ic, sOutput);

      sOutput->setLength(sOutput->getLength() - 1);
   }

   if (j == 2) {
      ic[3] = 0;
      ic[2] = 0;
      CharsToAscii85(ic, sOutput);

      sOutput->setLength(sOutput->getLength() - 2);
   }

   if (j == 1) {
      ic[3] = 0;
      ic[2] = 0;
      ic[1] = 0;
      CharsToAscii85(ic, sOutput);

      sOutput->setLength(sOutput->getLength() - 3);
   }


   sOutput->append("~>", 2);
}



void CharsToBase64(char sInput[], Groundfloor::String *sOutput) {
   unsigned char a, b, c, d;

   a = (sInput[0] & 0xFC);
   a >>= 2;

   b = (sInput[0] & 0x03);
   b <<= 4;

   c = (sInput[1] & 0xF0);
   c >>= 4;
   b += c;

   c = (sInput[1] & 0x0F);
   c <<= 2;
   d = (sInput[2] & 0xC0);
   d >>= 6;

   c += d;

   d = (sInput[2] & 0x3F);

   sOutput->append(static_cast<const char *>(sBase64Table + a), 1);
   sOutput->append(static_cast<const char *>(sBase64Table + b), 1);
   sOutput->append(static_cast<const char *>(sBase64Table + c), 1);
   sOutput->append(static_cast<const char *>(sBase64Table + d), 1);
}


void Groundfloor::StrToBase64(const Groundfloor::String *sInput, Groundfloor::String *sOutput) {
   char *sI = sInput->getValue();
   char ic[3] = { 0,0,0 };

   sOutput->setSize(static_cast<unsigned int>(sInput->getSize() / 100 * 135.1));
   sOutput->setLength(0);

   unsigned int i = 0;
   int j = 0;
   int inl = 0;
   while (i < sInput->getLength()) {
      ic[j] = sI[i];

      j++;
      i++;

      j %= 3;

      if (j == 0) {
         CharsToBase64(ic, sOutput);
         inl++;

         if (inl == 19) {
            inl = 0;

            sOutput->append(char(13));
            sOutput->append(char(10));
         }
      }
   }

   if (j == 2) {
      ic[2] = 0;
      CharsToBase64(ic, sOutput);

      sOutput->setLength(sOutput->getLength() - 1);
      sOutput->append("=", 1);
   }
   else if (j == 1) {
      ic[2] = 0;
      CharsToBase64(ic, sOutput);

      sOutput->setLength(sOutput->getLength() - 2);
      sOutput->append("==", 2);
   }
}


int getCharIndexInHufftable(const Groundfloor::String *sHufftable, char ch) {
   char *s = sHufftable->getValue();

   unsigned int c = sHufftable->getLength();
   for (unsigned int i = 0; i < c; i++) {
      if (s[i] == ch) {
         return i;
      }
   }

   return -1;
}


////
void Groundfloor::StrToCharHuffman(const Groundfloor::String *sInput, Groundfloor::String *sOutputData, Groundfloor::String *sOutputTable1, Groundfloor::String *sOutputTable2) {
   __int64 OccurrencyTable[256];
   char *sI = sInput->getValue();
   unsigned int i;

   // reset occurencytable (probably already empty, but still)
   for (i = 0; i < 256; i++) {
      OccurrencyTable[i] = 0;
   }

   // fill occurencytable
   unsigned int c = sInput->getLength();
   for (i = 0; i < c; i++) {
      // 0 + is om compiler warnings te ontwijken en ws. sneller dan een aantal reinterpret_casts
      OccurrencyTable[0 + sI[i]]++;
   }

   // fill huffman table
   Groundfloor::String sOutputTable;
   sOutputTable.setSize(256);
   sOutputTable.setLength(0);

   // minimum occurency should be 1, and maximum is length of string
   //  method is a kind of sorting algo, but easier (not per say faster)
   unsigned int j;
   for (j = 1; j < sInput->getLength(); j++) {
      for (i = 0; i < 256; i++) {
         if (OccurrencyTable[i] == j) {
            sOutputTable.append(static_cast<char>(i));
         }
      }
   }

   // mirror table, so max occurency is first char
   char *sO = sOutputTable.getValue();
   char swap;
   unsigned int l = sOutputTable.getLength();
   unsigned int ld2 = l / 2;
   for (i = 0; i < ld2; i++) {
      swap = sO[i];
      sO[i] = sO[l - i - 1];
      sO[l - i - 1] = swap;
   }

   // split the table into 2 (table1 for 0 tree, table2 for 1 tree)
   sOutputTable1->setSize(ld2);
   sOutputTable1->setLength(0);
   sOutputTable2->setSize(ld2 + 1);
   sOutputTable2->setLength(0);

   for (i = 0; i < ld2; i++) {
      sOutputTable1->append(sO[i]);
      sOutputTable2->append(sO[ld2 + i]);
   }

   // l/2 is truncated, so it might be that 1 byte is missed, add that one to table2
   if (ld2 * 2 < l) {
      sOutputTable2->append(sO[l - 1]);
   }

   // fill compressed data
   sOutputData->setSize(sInput->getLength());
   sOutputData->setLength(0);

   unsigned int iCurrentBit = 0;
   unsigned char cCurrentByte = 0;
   unsigned char cBit = 0;
   bool bFound = false;

   char b;
   unsigned int d = sInput->getLength();
   for (i = 0; i < d; i++) {
      b = sI[i];

      int i1 = getCharIndexInHufftable(sOutputTable1, b);
      int i2 = getCharIndexInHufftable(sOutputTable2, b);

      int iset = 0;
      if (i2 >= 0) {
         iset = i2;

         cBit = 1;
         cCurrentByte |= (cBit << (7 - iCurrentBit));
      }
      else {
         iset = i1;
      }

      iCurrentBit++;
      if (iCurrentBit == 8) {
         iCurrentBit = 0;

         sOutputData->append(cCurrentByte);
         cCurrentByte = 0;
      }

      while (iset > 0) {
         iCurrentBit++;

         if (iCurrentBit == 8) {
            iCurrentBit = 0;

            sOutputData->append(cCurrentByte);
            cCurrentByte = 0;
         }

         iset--;
      }

      cBit = 1;
      cCurrentByte |= (cBit << (7 - iCurrentBit));

      iCurrentBit++;
      if (iCurrentBit == 8) {
         iCurrentBit = 0;

         sOutputData->append(cCurrentByte);
         cCurrentByte = 0;
      }
   }

   if (iCurrentBit > 0) {
      sOutputData->append(cCurrentByte);
   }
}

void Groundfloor::CharHuffmanToStr(const Groundfloor::String *sInputData, const Groundfloor::String *sInputTable1, const Groundfloor::String *sInputTable2, Groundfloor::String *sOutputString) {
   char *sI = sInputData->getValue();
   char *sT1 = sInputTable1->getValue();
   char *sT2 = sInputTable2->getValue();
   char *sT = NULL;
   unsigned int i, j;

   // guess probable output length
   sOutputString->setSize(sInputData->getLength() * 2);
   sOutputString->setLength(0);

   unsigned int iCurrentBit = 0;
   unsigned char cCurrentByte = 0;
   unsigned char c2 = 0;

   unsigned int l = sInputData->getLength();
   for (i = 0; i < l; i++) {
      cCurrentByte = sI[i];

      for (j = 0; j < 8; j++) {
         c2 = 1;
         c2 <<= (7 - j);

         if ((cCurrentByte & c2) != 0) {
            if (iCurrentBit == 0) {
               sT = sT2;
               iCurrentBit++;
            }
            else {
               sOutputString->append(sT[iCurrentBit - 1]);
               iCurrentBit = 0;
            }
         }
         else {
            if (iCurrentBit == 0) {
               sT = sT1;
            }
            iCurrentBit++;
         }
      }
   }
}

void Groundfloor::EncodeUrlEntities(Groundfloor::String *s) {
   Groundfloor::String tmp;

   unsigned int c = s->getLength();

   char *str = s->getPointer(0);

   // minimum size required
   tmp.setSize(c);

   for (unsigned int i = 0; i < c; i++) {
      char ch = str[i];

      if (
         ((ch >= 'a') && (ch <= 'z')) ||
         ((ch >= 'A') && (ch <= 'Z')) ||
         ((ch >= '0') && (ch <= '9')) ||
         (ch == '-') ||
         (ch == '_') ||
         (ch == '.')) {
         // no encoding needed
         tmp.append(ch);
      }
      else {
         // encode
         tmp.append('%');
         CharToHex(ch, &tmp);
      }
   }

   s->setValue(&tmp);
}

void Groundfloor::EncodeHtmlEntities(Groundfloor::String *s) {
   Groundfloor::String tmp(s);

   tmp.replace_ansi("&", "&amp;");
   tmp.replace_ansi("\"", "&quot;");
   tmp.replace_ansi("'", "&apos;");
   tmp.replace_ansi("<", "&lt;");
   tmp.replace_ansi(">", "&gt;");

   s->setValue(&tmp);
}

void Groundfloor::DecodeHtmlEntities(Groundfloor::String *s) {
   Groundfloor::String tmp(s);

   tmp.replace_ansi("&quot;", "\"");
   tmp.replace_ansi("&apos;", "'");
   tmp.replace_ansi("&lt;", "<");
   tmp.replace_ansi("&gt;", ">");
   tmp.replace_ansi("&amp;", "&");

   s->setValue(&tmp);
}


////////////////////////////////////////////////////////////////////////////////



#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <string.h>
#include <limits.h>
#include <wchar.h>

unsigned long Groundfloor::GetFileSize(const Groundfloor::String *sFilePath) {
#ifdef GF_OS_WIN32
   struct _stat rec;

   Groundfloor::String sTmpFile(sFilePath);
   sTmpFile.transformToWidestring(true);

   if (!_wstat(sTmpFile.getValueAsWide(), &rec)) {
      return rec.st_size;
   }
   
   return 0;
#else
   struct stat rec;

   if (!stat(sFilePath->getValue(), &rec)) {
      return rec.st_size;
   }

   return 0;
#endif
}

bool Groundfloor::FileExists(const Groundfloor::String *sFilePath) {
#ifdef GF_OS_WIN32
   Groundfloor::String sTmpFile(sFilePath);
   sTmpFile.transformToWidestring(true);

   FILE *fp = _wfopen(sTmpFile.getValueAsWide(), L"rb");
   
   if (fp != 0) {
      fclose(fp);

      return true;
   }

   return false;
#else
   FILE *fp = fopen(sFilePath->getValue(), "rb");

   if (fp != 0) {
      fclose(fp);

      return true;
   }

   return false;
#endif
}

bool Groundfloor::IsDirectory(const Groundfloor::String *sFilePath) {
#ifdef GF_OS_WIN32
   struct _stat rec;

   Groundfloor::String sTmpPath(sFilePath);
   sTmpPath.transformToWidestring(true);

   if (_wstat(sTmpPath.getValueAsWide(), &rec) != 0) {
      // retry with ansi function
      _stat(sFilePath->getValue(), &rec);
   }
   
   return ((rec.st_mode & _S_IFDIR) > 0);
#else
   struct stat rec;

   stat(sFilePath->getValue(), &rec);

   return S_ISDIR(rec.st_mode);
#endif
}

bool Groundfloor::CreateDir(const Groundfloor::String *sFilePath) {
#ifdef GF_OS_WIN32
   Groundfloor::String sTmpPath(sFilePath);
   sTmpPath.transformToWidestring(true);
   //sTmpPath->prepend_wide(L"\\\\?\\"); // for pathnames > 248 characters

   wprintf(L"%s\n", sTmpPath.getValueAsWide());

   LPSECURITY_ATTRIBUTES attr = NULL;
   BOOL ret = ::CreateDirectoryW(sTmpPath.getValueAsWide(), attr);

   return (ret == TRUE);
#else
   mode_t mode = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH;

   int err = mkdir(sFilePath->getValue(), mode);
   // errno contains error if err == -1

   return (err == 0);
#endif
}

Groundfloor::String *Groundfloor::GetCurrentDir() {
   Groundfloor::String *sDir = new Groundfloor::String();

#ifdef GF_OS_WIN32
   DWORD dwRet;
   unsigned int iLen = ::GetCurrentDirectoryW(0, NULL) * sizeof(wchar_t);

   sDir->setLength(iLen);
   dwRet = ::GetCurrentDirectoryW(iLen, reinterpret_cast<wchar_t *>(sDir->getValue()));

   if (!dwRet) {
      delete sDir;

      return NULL;
   }
   else {
      sDir->setLength(getWStrByteLen(reinterpret_cast<wchar_t *>(sDir->getValue())));
   }

   sDir->transformWidestringToString(true);
#else
   sDir->setLength(PATH_MAX);

   if (getcwd(sDir->getValue(), sDir->getSize()) == NULL) {
      delete sDir;

      return NULL;
   }
   else {
      sDir->setLength(strlen(sDir->getValue()));
   }
#endif

   return sDir;
}

Groundfloor::String *Groundfloor::createHybridString(const void *s, bool bWide) {
   if (bWide) {
      return new Groundfloor::String(reinterpret_cast<const wchar_t *>(s));
   }
   else {
      return new Groundfloor::String(reinterpret_cast<const char *>(s));
   }
}

void Groundfloor::PrependNativeSlash(Groundfloor::String *sPath, bool bWide) {
   Groundfloor::String *a;
   if (bWide) {
      a = createHybridString(S_WIDE_NATIVE_SLASH, bWide);
   }
   else {
      a = createHybridString(S_ANSI_NATIVE_SLASH, bWide);
   }
   if (!sPath->startsWith(a)) {
      sPath->prepend(a);
   }
   delete a;
}

void Groundfloor::AppendNativeSlash(Groundfloor::String *sPath, bool bWide) {
   Groundfloor::String *a;
   if (bWide) {
      a = createHybridString(S_WIDE_NATIVE_SLASH, bWide);
   }
   else {
      a = createHybridString(S_ANSI_NATIVE_SLASH, bWide);
   }
   if (!sPath->endsWith(a)) {
      sPath->append(a);
   }
   delete a;
}

void Groundfloor::ConvertToNativeSlashes(Groundfloor::String *sPath, bool bWide) {
   Groundfloor::String *a;
   Groundfloor::String *b;
   Groundfloor::String *c;
   if (bWide) {
      a = createHybridString(S_WIDE_NATIVE_SLASH, bWide);
      b = createHybridString(S_WIDE_WIN32_SLASH, bWide);
      c = createHybridString(S_WIDE_LINUX_SLASH, bWide);
   }
   else {
      a = createHybridString(S_ANSI_NATIVE_SLASH, bWide);
      b = createHybridString(S_ANSI_WIN32_SLASH, bWide);
      c = createHybridString(S_ANSI_LINUX_SLASH, bWide);
   }
   sPath->replace(b, a);
   sPath->replace(c, a);
   delete c;
   delete b;
   delete a;
}

void Groundfloor::ConvertToURISlashes(Groundfloor::String *sPath, bool bWide) {
   Groundfloor::String *a;
   Groundfloor::String *b;
   if (bWide) {
      a = createHybridString(S_WIDE_WIN32_SLASH, bWide);
      b = createHybridString(S_WIDE_LINUX_SLASH, bWide);
   }
   else {
      a = createHybridString(S_ANSI_WIN32_SLASH, bWide);
      b = createHybridString(S_ANSI_LINUX_SLASH, bWide);
   }
   sPath->replace(a, b);
   delete b;
   delete a;
}

#include <time.h>

__int64 Groundfloor::GetTimestamp() {
   time_t tCurrent;

   time(&tCurrent);

   return tCurrent;
}

// Validate strftime format string
static bool validateStrftimeFormat(const char *format, Groundfloor::String *errorMsg = nullptr) {
   // Valid strftime conversion specifiers according to cppreference.com
   // Including: n t y Y E y C c G g b h B m U W V j d a e A a w u H I O M S p c x X r R T D F z Z p %
   const char* validSpecifiers = "ntyYEyCcGgbhBmUWVjdaeAawuHIOMSpcxXrRTDFzZp%";

   const char* p = format;
   while (*p) {
      if (*p == '%') {
         const char* percentStart = p;  // Remember where the % started
         p++;
         if (*p == '\0') {
            if (errorMsg) {
               errorMsg->setValue("Format string ends with incomplete % specifier");
            }
            return false;
         }

         // Check for optional flag characters (-, _, 0, ^, #)
         if (*p == '-' || *p == '_' || *p == '0' || *p == '^' || *p == '#') {
            p++;
            if (*p == '\0') {
               if (errorMsg) {
                  errorMsg->setValue("Format string ends with incomplete % specifier after flag");
               }
               return false;
            }
         }

         // Check for optional field width (digits)
         while (*p >= '0' && *p <= '9') {
            p++;
            if (*p == '\0') {
               if (errorMsg) {
                  errorMsg->setValue("Format string ends with incomplete % specifier after width");
               }
               return false;
            }
         }

         // Check for E or O modifier
         if (*p == 'E' || *p == 'O') {
            p++;
            if (*p == '\0') {
               if (errorMsg) {
                  errorMsg->setValue("Format string ends with incomplete % specifier after modifier");
               }
               return false;
            }
         }

         // Check if the conversion specifier is valid
         if (strchr(validSpecifiers, *p) == nullptr) {
            if (errorMsg) {
               // Build error message without sprintf
               errorMsg->setValue("Invalid format specifier: ");

               // Extract and append the invalid format sequence (from % to current position)
               int sequenceLength = (int)(p - percentStart + 1);
               errorMsg->append(percentStart, sequenceLength);
            }
            return false;
         }
      }
      p++;
   }

   return true;
}

Groundfloor::String *Groundfloor::TimestampToStr(const char *format, __int64 timestamp) {
   // Validate format string first
   Groundfloor::String errorMsg;
   if (!validateStrftimeFormat(format, &errorMsg)) {
      // Throw exception with details about invalid format
      Groundfloor::String *exceptionMsg = new Groundfloor::String();
      exceptionMsg->setValue("Invalid strftime format string: ");
      exceptionMsg->append(&errorMsg);

      std::string stdError = exceptionMsg->getValue();
      delete exceptionMsg;

      throw std::runtime_error(stdError);
   }

   Groundfloor::String *datetimestr = new Groundfloor::String();

   // reserve memory
   datetimestr->setSize(strlen(format) * 2 + 4);

   time_t t = timestamp;
   struct tm *timeinfo = localtime(&t);

   int len = strftime(datetimestr->getPointer(0), datetimestr->getSize(), format, timeinfo);
   datetimestr->setLength(len);

   return datetimestr;
}

__int64 Groundfloor::DateStrToTimestamp(Groundfloor::String *sDate, bool bTZCorrection, bool bDaylightSavingsCorrection) {
   int iDatLen = sDate->getLength();

   int y = 0;
   int m = 0;
   int d = 0;
   int h = 0;
   int n = 0;
   int s = 0;

   char *p = sDate->getValue();

   switch (iDatLen) {
      // "23:59"
   case 5:
      if (p[2] == ':') {
         h = (p[0] - 48) * 10;
         h += (p[1] - 48);
         n = (p[3] - 48) * 10;
         n += (p[4] - 48);
      }
      else {
         return 0;
      }
      break;

      // "23:59:59"
   case 8:
      if ((p[2] == ':') && (p[5] == ':')) {
         h = (p[0] - 48) * 10;
         h += (p[1] - 48);
         n = (p[3] - 48) * 10;
         n += (p[4] - 48);
         s = (p[6] - 48) * 10;
         s += (p[7] - 48);
      }
      else {
         return 0;
      }

      break;

      // "9999-12-31"
   case 10:
      if ((p[4] == '-') && (p[7] == '-')) {
         y = (p[0] - 48) * 1000;
         y += (p[1] - 48) * 100;
         y += (p[2] - 48) * 10;
         y += (p[3] - 48);
         m = (p[5] - 48) * 10;
         m += (p[6] - 48);
         d = (p[8] - 48) * 10;
         d += (p[9] - 48);
      }
      else {
         return 0;
      }

      break;

      // "9999-12-31 23:59"
   case 16:
      if ((p[4] == '-') && (p[7] == '-') && (p[13] == ':')) {
         y = (p[0] - 48) * 1000;
         y += (p[1] - 48) * 100;
         y += (p[2] - 48) * 10;
         y += (p[3] - 48);
         m = (p[5] - 48) * 10;
         m += (p[6] - 48);
         d = (p[8] - 48) * 10;
         d += (p[9] - 48);

         h = (p[11] - 48) * 10;
         h += (p[12] - 48);
         n = (p[14] - 48) * 10;
         n += (p[15] - 48);
      }
      else {
         return 0;
      }

      break;

      // "9999-12-31 23:59:59"
   case 19:
      if ((p[4] == '-') && (p[7] == '-') && (p[13] == ':') && (p[16] == ':')) {
         y = (p[0] - 48) * 1000;
         y += (p[1] - 48) * 100;
         y += (p[2] - 48) * 10;
         y += (p[3] - 48);
         m = (p[5] - 48) * 10;
         m += (p[6] - 48);
         d = (p[8] - 48) * 10;
         d += (p[9] - 48);

         h = (p[11] - 48) * 10;
         h += (p[12] - 48);
         n = (p[14] - 48) * 10;
         n += (p[15] - 48);
         s = (p[17] - 48) * 10;
         s += (p[18] - 48);
      }
      else {
         return 0;
      }

      break;

   default:
      return 0;
      break;
   }


   struct tm timeinfo;

   // timezone correction for mktime()
   time_t tnow = Groundfloor::GetTimestamp();
   struct tm *tmp;

   if (bDaylightSavingsCorrection) {
      struct tm *tmpDst = localtime(&tnow);
      timeinfo.tm_isdst = tmpDst->tm_isdst;
   }

   tmp = gmtime(&tnow);
   time_t utcnow = mktime(tmp);

   time_t correction = tnow - utcnow;

   timeinfo.tm_year = y - 1900;
   timeinfo.tm_mon = m - 1;
   timeinfo.tm_mday = d;

   timeinfo.tm_hour = h;
   timeinfo.tm_min = n;
   timeinfo.tm_sec = s;

   time_t t;

   if (bTZCorrection) {
      t = mktime(&timeinfo) + correction;
   }
   else {
      t = mktime(&timeinfo);
   }

   return t;
}

//-----------------------------------------------------------------------------

void Groundfloor::split_p(Groundfloor::Vector *pSlicesVector, const Groundfloor::String *sHaystack, const char *sNeedle, int iMax) {
   Groundfloor::String *slice;
   int iCurrentPos = 0;
   int iPreviousPos = 0;
   unsigned int iLen = strlen(sNeedle);

   int iCount = 1;

   iCurrentPos = sHaystack->pos(0, sNeedle, iLen);
   while (iCurrentPos != -1) {
      slice = new Groundfloor::String();
      slice->setValue(sHaystack->getPointer(iPreviousPos), iCurrentPos - iPreviousPos);
      pSlicesVector->addElement(slice);

      iPreviousPos = iCurrentPos + iLen;

      iCurrentPos = sHaystack->pos(iPreviousPos, sNeedle, iLen);

      if (iMax != -1) {
         iCount++;

         if (iCount >= iMax) {
            break;
         }
      }
   }

   slice = new Groundfloor::String();
   slice->setValue(sHaystack->getPointer(iPreviousPos), sHaystack->getLength() - iPreviousPos);
   pSlicesVector->addElement(slice);
}

Groundfloor::Vector *Groundfloor::split(const Groundfloor::String *sHaystack, const char *sNeedle, int iMax) {
   Groundfloor::Vector *slices = new Groundfloor::Vector();

   Groundfloor::split_p(slices, sHaystack, sNeedle, iMax);

   return slices;
}

Groundfloor::Vector Groundfloor::split_m(const Groundfloor::String *sHaystack, const char *sNeedle, int iMax) {
   Groundfloor::Vector slices;

   Groundfloor::split_p(&slices, sHaystack, sNeedle, iMax);

   return slices;
}

bool Groundfloor::isNumber(const char *sNeedle, unsigned int len) {
   bool bOk = false; // empty string ain't a number
   unsigned char c;

   for (unsigned int i = 0; i < len; i++) {
      c = sNeedle[i];
      bOk = ((c <= 57) && (c >= 48));
      if (!bOk) {
         break;
      }
   }

   return bOk;
}

#include <wchar.h>
unsigned int Groundfloor::getWStrByteLen(const wchar_t *s) {
   return wcslen(s) * sizeof(wchar_t);
}
