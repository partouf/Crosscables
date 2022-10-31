
#ifndef __BFunctions_H
#define __BFunctions_H

#include "../Materials/CsvSettings.h"
#include "../Molecules/String.h"
#include "../Bookshelfs/BTable.h"
#include "../Bookshelfs/BNode.h"

#include "../Materials/FileWriter.h"

namespace Groundfloor {
   void BCsvWrite_Table(BaseWriter *aWriter, const BTable *aSource, CsvSettings *aSettings);
   void BCsvExport_Table(const String *sDestination, const BTable *aSource, CsvSettings *aSettings);

   void BXmlWrite_Node(BaseWriter *aWriter, BNode *aSource, XmlSettings *aSettings, bool bWriteDefaultHeader);
   void BXmlExport_Node(const String *sDestination, BNode *aSource, XmlSettings *aSettings, bool bWriteDefaultHeader);

   BNode *BXmlImport_FromString(const String *sXML);
   BNode *BXmlImport_FromFile(const String *sSourceFile);

   BNode *BCsvImport_FromFile(const String *sSourceFile, const CsvSettings *aSettings);

   /// lossy function to try and generate a table from Nodes (attributes are lost, contents are translated to recordvalues)
   BTable *BNodeContentsToTable(const BNode *aNode, int iDefaultDatatype = FLDTYPE_STRING);
   /// lossy function to try and generate a table from Nodes (attributes are translated to recordvalues, contents are lost)
   BTable *BNodeAttributesToTable(const BNode *aNode, int iDefaultDatatype = FLDTYPE_STRING);

   /// semi-lossless function to generate Nodes from a table (every recordvalue is a seperate node)
   BNode *BTableToNodeContents(const BTable *aTable, const char *sTableNodeName, const char *sRecordNodeName);
   /// semi-lossless function to generate Nodes from a table (every recordvalue is a seperate attribute)
   BNode *BTableToNodeAttributes(const BTable *aTable, const char *sTableNodeName, const char *sRecordNodeName);
}

#endif // __BFunctions_H

