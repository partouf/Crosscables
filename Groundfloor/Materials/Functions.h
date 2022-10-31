
#ifndef _GFFunctions_H_
#define _GFFunctions_H_

#include "../Molecules/Vector.h"
#include "../Molecules/String.h"
#include "../Atoms/Defines.h"

namespace Groundfloor {

   /// returns position of c in string s, or -1 if not found
   int strpos(const char *s, char c);

   /// converts sInput (binary data) to Base64, overwriting sOutput
   void StrToBase64(const String *sInput, String *sOutput);

   /// converts sInput (binary data) to Ascii85, overwriting sOutput
   void StrToAscii85(const String *sInput, String *sOutput);

   /// appends the 2-character-hexadecimal-value of c to sOutput
   void CharToHex(char c, String *sOutput);
   /// overwrites sOutput with full per-byte hexidecimal values, values per byte are not seperated
   void StrToHex(const String *sInput, String *sOutput);

   /// overwrites sOutput with full bit-to-ascii conversion of sInput, set bits are translated into '1', unset bits are '0'
   /// Bits are ordered from MSB to LSB (big endian).
   void StrToBin(const String *sInput, String *sOutput);

   /// prepends the hexadecimal ascii representation of i, length depends on compiler implementation of long (32 bits or 64 bits)
   void LongToHex(unsigned long i, String *sOutput);
   /// returns the integer value represented in the hexadecimal string sInput
   unsigned int HexToInt(const String *sInput);

   /// simple implementation of compressing sInput to binary tree sOutputData and per-node translation sOutputTable.
   /// only compresses single characters/bytes.
   //void StrToCharHuffman( const String *sInput, String *sOutputData, String *sOutputTable );
   void StrToCharHuffman(const String *sInput, String *sOutputData, String *sOutputTable1, String *sOutputTable2);

   /// returns original string from binary tree data and translation table
   void CharHuffmanToStr(const String *sInputData, const String *sInputTable1, const String *sInputTable2, String *sOutputString);

   void EncodeUrlEntities(String *s);

   void EncodeHtmlEntities(String *s);
   void DecodeHtmlEntities(String *s);

   ////////////////////////////////////////////////////////////////////////////////

   /// returns filesize of sFilePath, assuming UTF8 string
   unsigned long GetFileSize(const String *sFilePath);

   /// returns true if file exists, assuming UTF8 string
   bool FileExists(const String *sFilePath);
   /// returns true if path is a directory, assuming UTF8 string
   bool IsDirectory(const String *sFilePath);

   /// creates new directory
   bool CreateDir(const String *sFilePath);

   /// returns current directory represented as UTF8 string
   String *GetCurrentDir();

   String *createHybridString(const void *s, bool bWide);
   void PrependNativeSlash(String *sPath, bool bWide = false);
   void AppendNativeSlash(String *sPath, bool bWide = false);
   void ConvertToNativeSlashes(String *sPath, bool bWide = false);

   void ConvertToURISlashes(String *sPath, bool bWide = false);

   /// returns timestamp representing the current date and time
   __int64 GetTimestamp();

   /// timestamp to string following the given format (http://www.cplusplus.com/reference/ctime/strftime/)
   String *TimestampToStr(const char *format, __int64 timestamp);

   /// translates string into timestamp, acceptable formats are "hh:mm", "hh:mm:ss", "yyyy-mm-dd", "yyyy-mm-dd hh:mm" or "yyyy-mm-dd hh:mm:ss"
   /** bTZCorrection can be optionally set to False to Not include a correction that counters the behaviour - of the functions that are used - to translate the timestamp to the local timezone.
        unless you know what you're doing, you should leave these at their default True values to get proper date translations that are compatible with BValue's dates and timestamps */
   __int64 DateStrToTimestamp(String *sDate, bool bTZCorrection = true, bool bDaylightSavingsCorrection = true);


   //------------------------------------------------------------------------------

   /// Splits given string sHaystack into new strings that are stored in a new Vector,
   ///  manual deletion of returned vector is required.
   Vector *split(const String *sHaystack, const char *sNeedle, int iMax = -1);

   void split_p(Vector *pSlicesVector, const String *sHaystack, const char *sNeedle, int iMax = -1);

   Vector split_m(const String *sHaystack, const char *sNeedle, int iMax = -1);

   /// Determines whether given string sNeedle solely contains numbers between 0 and 9
   bool isNumber(const char *sNeedle, unsigned int len);

   /// returns the length in bytes of given widestring
   unsigned int getWStrByteLen(const wchar_t *s);

}

#endif //_GFFunctions_H_
