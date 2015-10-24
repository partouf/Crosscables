
#include "BFunctions.h"

#include "../Materials/FileWriter.h"
#include "../Materials/Functions.h"

//-----------------------------------------------------------------------------


void Groundfloor::BCsvWrite_Table(BaseWriter *aWriter, const BTable *aSource, CsvSettings *aSettings) {
   String sEscapedStr;
   sEscapedStr.setSize(aSettings->sep_string.getLength() * 2 + 1);
   sEscapedStr.append(&aSettings->sep_string);
   sEscapedStr.append(&aSettings->sep_string);

   String sVal;

   BRecords *records = aSource->records.get();
   BFields *fields = aSource->fields.get();

   if (aSettings->show_columnnames.get()) {

      unsigned int s1 = fields->size();
      for (unsigned int i = 0; i < s1; i++) {
         BField *field = fields->getField(i);

         sVal.setValue(&field->name);
         sVal.replace(&aSettings->sep_string, &sEscapedStr);

         aWriter->add(&aSettings->sep_string);
         aWriter->add(&field->name);
         aWriter->add(&aSettings->sep_string);

         if (i == s1 - 1) {
            aWriter->add(&aSettings->sep_record);
         }
         else {
            aWriter->add(&aSettings->sep_column);
         }
      }

   }

   unsigned int s2 = records->size();
   for (unsigned int j = 0; j < s2; j++) {
      BRecord *record = records->getRecord(j);

      unsigned int s3 = record->size();
      for (unsigned int k = 0; k < s3; k++) {
         int iDatatype = fields->getField(k)->datatype.get();

         sVal.setValue(record->getValue(k)->asString());
         sVal.replace(&aSettings->sep_string, &sEscapedStr);

         if (iDatatype == FLDTYPE_STRING) {
            aWriter->add(&aSettings->sep_string);
            aWriter->add(&sVal);
            aWriter->add(&aSettings->sep_string);
         }
         else {
            aWriter->add(&sVal);
         }

         if (k < s3 - 1) {
            aWriter->add(&aSettings->sep_column);
         }

      }

      aWriter->add(&aSettings->sep_record);
   }
}

void Groundfloor::BCsvExport_Table(const String *sDestination, const BTable *aSource, CsvSettings *aSettings) {
   FileWriter fw;
   fw.open(sDestination, false);

   fw.setInterval(0);
   fw.start();

   Groundfloor::BCsvWrite_Table(&fw, aSource, aSettings);
   fw.setStopWhenEmpty(true);

   while (fw.isRunning()) {
      GFMillisleep(10);
   }

   fw.close();
}

//-----------------------------------------------------------------------------

void BXmlExport_NodeIteration(Groundfloor::BaseWriter *fw, Groundfloor::BNode *aSource, Groundfloor::XmlSettings *aSettings) {
   bool bIsTag = (aSource->name.getLength() != 0);

   if (bIsTag) {
      fw->add(&aSettings->node_open);
      fw->add(&aSource->name);

      if (aSource->childCount() != 0) {
         if (aSource->attributeCount() > 0) {
            unsigned int c = aSource->attributeCount();
            for (unsigned int i = 0; i < c; i++) {
               Groundfloor::BAttribute *anAttrib = aSource->getAttribute(i);
               if (anAttrib != NULL) {
                  if (!anAttrib->hideFromXML) {
                     fw->add(&aSettings->node_space);
                     fw->add(&anAttrib->key);
                     fw->add(&aSettings->sep_keyval);
                     fw->add(&aSettings->sep_string);

                     Groundfloor::String tmp(anAttrib->value.asString());
                     Groundfloor::EncodeHtmlEntities(&tmp);
                     fw->add(&tmp);

                     fw->add(&aSettings->sep_string);
                  }
               }
            }
         }
         fw->add(&aSettings->node_close);

         bool bMayWriteSeperator = false;

         unsigned int d = aSource->childCount();
         for (unsigned int j = 0; j < d; j++) {
            Groundfloor::BNode *aChild = aSource->getChild(j);
            if (aChild != NULL) {
               if (aChild->name.getLength() != 0) {
                  bMayWriteSeperator = true;
                  fw->add(&aSettings->sep_node);
               }
               BXmlExport_NodeIteration(fw, aChild, aSettings);
            }
         }

         if (bMayWriteSeperator) {
            fw->add(&aSettings->sep_node);
         }

         fw->add(&aSettings->node_open);
         fw->add(&aSettings->node_end);
         fw->add(&aSource->name);
         fw->add(&aSettings->node_close);
      }
      else {
         if (aSource->attributeCount() > 0) {
            unsigned int c = aSource->attributeCount();
            for (unsigned int i = 0; i < c; i++) {
               Groundfloor::BAttribute *anAttrib = aSource->getAttribute(i);
               if (anAttrib != NULL) {
                  if (!anAttrib->hideFromXML) {
                     fw->add(&aSettings->node_space);
                     fw->add(&anAttrib->key);
                     fw->add(&aSettings->sep_keyval);
                     fw->add(&aSettings->sep_string);

                     Groundfloor::String tmp(anAttrib->value.asString());
                     Groundfloor::EncodeHtmlEntities(&tmp);
                     fw->add(&tmp);

                     fw->add(&aSettings->sep_string);
                  }
               }
            }
         }

         if (aSource->content.getLength() > 0) {
            fw->add(&aSettings->node_close);

            Groundfloor::String tmp(&aSource->content);
            Groundfloor::EncodeHtmlEntities(&tmp);
            fw->add(&tmp);

            fw->add(&aSettings->node_open);
            fw->add(&aSettings->node_end);
            fw->add(&aSource->name);
            fw->add(&aSettings->node_close);
         }
         else {
            fw->add(&aSettings->node_space);
            fw->add(&aSettings->node_end);
            fw->add(&aSettings->node_close);
         }
      }

   }
   else { // not bIsTag

      unsigned int d = aSource->childCount();
      for (unsigned int j = 0; j < d; j++) {
         Groundfloor::BNode *aChild = aSource->getChild(j);
         if (aChild != NULL) {
            BXmlExport_NodeIteration(fw, aChild, aSettings);
         }
      }

      if (aSource->content.getLength() > 0) {
         Groundfloor::String tmp(&aSource->content);
         Groundfloor::EncodeHtmlEntities(&tmp);
         fw->add(&tmp);
      }
   }
}

void Groundfloor::BXmlWrite_Node(BaseWriter *aWriter, BNode *aSource, XmlSettings *aSettings, bool bWriteDefaultHeader) {
   if (bWriteDefaultHeader) {
      String sHeader;
      sHeader.setValue_ansi("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
      aWriter->add(&sHeader);
      aWriter->add(&aSettings->sep_node);
   }

   BXmlExport_NodeIteration(aWriter, aSource, aSettings);
}

void Groundfloor::BXmlExport_Node(const String *sDestination, BNode *aSource, XmlSettings *aSettings, bool bWriteDefaultHeader) {
   FileWriter fw;
   fw.open(sDestination, false);

   fw.setInterval(0);
   fw.start();

   Groundfloor::BXmlWrite_Node(&fw, aSource, aSettings, bWriteDefaultHeader);
   fw.setStopWhenEmpty(true);

   while (fw.isRunning()) {
      GFMillisleep(10);
   }

   fw.close();
}


//-----------------------------------------------------------------------------

#define ctagopen '<'
#define ctagclose '>'
#define ctagslash '/'
#define cspace ' '

#define cattrsep '='
#define cattrquote1 '"'
#define cattrquote2 '\''
#define cattrescape '\\'
#define cheadersign '?'
#define ccommentsign '!'

#include "../Materials/Functions.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))

void parseAttributes(Groundfloor::BNode *node, const Groundfloor::String *s) {
   Groundfloor::String tmp;

   bool bIsQuote1 = false;
   bool bIsQuote2 = false;
   bool bIsEscape = false;

   int iLastAttrSep = -1;
   int iLastQuote1 = -1;
   int iLastQuote2 = -1;

   int iStartNewAttr = 0;

   char *cs = s->getValue();
   unsigned int d = s->getLength();
   for (unsigned int j = 0; j < d; j++) {
      char c = cs[j];

      if (!(bIsQuote1 || bIsQuote2 || bIsEscape)) {

         switch (c) {
         case cattrsep:
            iLastAttrSep = j;
            break;
         case cattrquote1:
            bIsQuote1 = true;
            iLastQuote1 = j;
            break;
         case cattrquote2:
            bIsQuote2 = true;
            iLastQuote2 = j;
            break;
         }

      }
      else {

         int iLastQuotePos = -1;
         bool bEoq = false;
         if (bIsEscape) {
            bIsEscape = false;
         }
         else {
            switch (c) {
            case cattrquote1:
               if (bIsQuote1) {

                  iLastQuotePos = iLastQuote1;
                  bEoq = true;

                  bIsQuote1 = false;
               }
               break;
            case cattrquote2:
               if (bIsQuote2) {
                  iLastQuotePos = iLastQuote2;
                  bEoq = true;

                  bIsQuote2 = false;
               }
               break;
            case cattrescape:
               bIsEscape = true;
               break;
            }
         }

         if (bEoq) {
            // value = iLastQuote1 ... j
            // key = iStartNewAttr ... iLastAttrSep - 1
            Groundfloor::BAttribute *a = new Groundfloor::BAttribute();
            a->key.setValue(s->getPointer(iStartNewAttr), iLastAttrSep - iStartNewAttr);
            a->key.ltrim_ansi();
            a->key.rtrim_ansi();

            Groundfloor::String tmp;
            tmp.setValue(s->getPointer(iLastQuotePos), j - iLastQuotePos + 1);
            tmp.ltrim_ansi();
            tmp.rtrim_ansi();

            if (tmp.startsWith("\"", 1) && tmp.endsWith("\"", 1)) {
               tmp.setValue(tmp.getPointer(1), tmp.getLength() - 2);
            }
            if (tmp.startsWith("'", 1) && tmp.endsWith("'", 1)) {
               tmp.setValue(tmp.getPointer(1), tmp.getLength() - 2);
            }

            Groundfloor::DecodeHtmlEntities(&tmp);
            a->value.setString(&tmp);


            iStartNewAttr = j + 1;

            node->addAttribute(a);
         }
      }

   }
}


Groundfloor::BNode *parseNode(Groundfloor::BNode *parent, const Groundfloor::String *s) {
   Groundfloor::BNode *node = NULL;

   if (s->getLength() != 0) {
      if (s->startsWith("</", 2)) {
         if (s->endsWith(">", 1)) {
            // is closetag
            Groundfloor::String tmp;
            tmp.setValue(s->getPointer(2), s->getLength() - 3);
            if (parent != NULL) {
               if (tmp.match(&parent->name)) {
                  return parent->getParentNode();
               }
               else {
                  // what?
                  printf("s=[%s], parent=[%s], tmp=[%s]\n", s->getValue(), parent->name.getValue(), tmp.getValue());
                  printf("error 1\n");
                  exit(1);
               }
            }
            else {
               // !?!?
               printf("error 2\n");
               exit(1);
            }
         }
         else {
            // invalid?
            printf("error 3\n");
            exit(1);
         }
      }
      else if (s->startsWith("<?", 2)) {
         // header, skip
      }
      else if (s->startsWith("<!", 2)) {
         // comment, skip
      }
      else if (s->startsWith("<", 1)) {
         if (s->endsWith("/>", 2)) {
            // is empty tag
            bool bHasAttr = true;
            int iPos = s->pos(0, " ", 1);
            if (iPos == -1) {
               iPos = s->pos(0, "/", 1);
               bHasAttr = false;
            }

            node = new Groundfloor::BNode();
            node->name.setValue(s->getPointer(1), iPos - 1);

            if (bHasAttr) {
               Groundfloor::String sAttr(s->getPointer(iPos + 1), s->getLength() - iPos - 3);
               parseAttributes(node, &sAttr);
            }

            if (parent != NULL) {
               parent->addChildNode(node);
            }

            return parent;
         }
         else if (s->endsWith(">", 1)) {
            bool bHasAttr = true;
            int iPos = s->pos(0, " ", 1);
            if (iPos == -1) {
               iPos = s->pos(0, ">", 1);
               bHasAttr = false;
            }

            node = new Groundfloor::BNode();
            node->name.setValue(s->getPointer(1), iPos - 1);

            if (bHasAttr) {
               Groundfloor::String sAttr(s->getPointer(iPos + 1), s->getLength() - iPos - 2);
               parseAttributes(node, &sAttr);
            }

            if (parent != NULL) {
               parent->addChildNode(node);
            }

            return node;
         }
         else {
            // invalid?
         }
      }
      else {
         // is content
         node = new Groundfloor::BNode();
         node->content.setValue(s);
         Groundfloor::DecodeHtmlEntities(&node->content);

         if (parent != NULL) {
            parent->addChildNode(node);
         }
         return parent;
      }
   }

   return NULL;
}



Groundfloor::BNode *Groundfloor::BXmlImport_FromString(const Groundfloor::String *sXml) {
   BNode *rootNode = NULL;

   String myTagName;
   BNode *currentNode = NULL;

   bool bIsQuote1 = false;
   bool bIsQuote2 = false;
   bool bIsEscape = false;
   bool bIsTag = false;

   String s;

   unsigned int i = 0;
   unsigned int cLen = sXml->getLength();
   char *cXml = sXml->getValue();
   while (i < cLen) {
      unsigned char c = cXml[i];
      i++;

      if (!(bIsQuote1 || bIsQuote2)) {

         if (bIsTag) {
            s.append(c);

            switch (c) {
            case cattrquote1:
               bIsQuote1 = true;
               break;
            case cattrquote2:
               bIsQuote2 = true;
               break;
            case ctagclose:
               bIsTag = false;

               s.ltrim_ansi();
               if (s.getLength() != 0) {
                  currentNode = parseNode(currentNode, &s);
                  if (rootNode == NULL) {
                     rootNode = currentNode;
                  }

                  s.setLength(0);
               }
               break;
            }

         }
         else {

            switch (c) {
            case ctagopen:
               bIsTag = true;

               s.ltrim_ansi();
               if (s.getLength() != 0) {
                  currentNode = parseNode(currentNode, &s);
                  if (rootNode == NULL) {
                     rootNode = currentNode;
                  }

                  s.setLength(0);
               }
               break;
            }


            s.append(c);
         }

      }
      else {    // if quote

         if (bIsEscape) {
            bIsEscape = false;
         }
         else {
            switch (c) {
            case cattrquote1:
               bIsQuote1 = false;
               break;
            case cattrquote2:
               bIsQuote2 = false;
               break;
            case cattrescape:
               bIsEscape = true;
               break;
            }
         }

         s.append(c);
      }
   }

   s.ltrim_ansi();
   if (s.getLength() != 0) {
      currentNode = parseNode(currentNode, &s);
   }

   return rootNode;
}



Groundfloor::BNode *Groundfloor::BXmlImport_FromFile(const Groundfloor::String *sSourceFile) {
   String s;

   FileCommunicator fc;
   fc.filename.set(sSourceFile->getValue());
   fc.mode.set(GFFILEMODE_READ);
   fc.connect();

   CommReturnData err;

   String buf;
   buf.setSize(1024);
   while (!err.eof) {
      fc.receive(&buf, &err);
      s.append(&buf);
   }

   fc.disconnect();

   BNode *node = Groundfloor::BXmlImport_FromString(&s);

   return node;
}

//-----------------------------------------------------------------------------


Groundfloor::BTable *Groundfloor::BNodeContentsToTable(const Groundfloor::BNode *aNode, int iDefaultDatatype) {
   BTable *table = new BTable();
   BFields *fields = new BFields();
   BRecords *records = new BRecords();

   unsigned int crecords = aNode->childCount();
   for (unsigned int r = 0; r < crecords; r++) {
      BNode *recordnode = aNode->getChild(r);
      BRecord *record = records->newRecord();

      unsigned int c = recordnode->childCount();
      for (unsigned int i = 0; i < c; i++) {
         BNode *subnode = recordnode->getChild(i);
         int ifield = fields->getFieldIndex(&subnode->name);
         if (ifield == -1) {
            ifield = fields->size();
            BField *field = fields->newField();
            field->name.setValue(&subnode->name);
            field->datatype.set(iDefaultDatatype);
         }

         record->setValueCount(ifield + 1);

         BValue *value = record->getValue(ifield);
         value->setString(&subnode->content);
      }
   }

   table->fields.set(fields);
   table->records.set(records);

   return table;
}

Groundfloor::BTable *Groundfloor::BNodeAttributesToTable(const BNode *aNode, int iDefaultDatatype) {
   BTable *table = new BTable();
   BFields *fields = new BFields();
   BRecords *records = new BRecords();

   unsigned int crecords = aNode->childCount();
   for (unsigned int r = 0; r < crecords; r++) {
      BNode *recordnode = aNode->getChild(r);
      BRecord *record = records->newRecord();

      unsigned int c = recordnode->attributeCount();
      for (unsigned int i = 0; i < c; i++) {
         BAttribute *attr = recordnode->getAttribute(i);
         int iattr = fields->getFieldIndex(&attr->key);
         if (iattr == -1) {
            iattr = fields->size();
            BField *field = fields->newField();
            field->name.setValue(&attr->key);
            field->datatype.set(iDefaultDatatype);
         }

         record->setValueCount(iattr + 1);

         BValue *value = record->getValue(iattr);
         value->setValue(&attr->value);
      }
   }

   table->fields.set(fields);
   table->records.set(records);

   return table;
}

//-----------------------------------------------------------------------------

Groundfloor::BNode *Groundfloor::BTableToNodeContents(const BTable *aTable, const char *sTableNodeName, const char *sRecordNodeName) {
   BNode *tablenode = NULL;
   if (aTable != NULL) {
      BFields *fields = aTable->fields.get();
      BRecords *records = aTable->records.get();

      if ((fields != NULL) && (records != NULL)) {
         tablenode = new BNode(sTableNodeName);

         unsigned int cfields = fields->size();
         unsigned int crecords = records->size();
         for (unsigned int r = 0; r < crecords; r++) {
            BRecord *record = records->getRecord(r);
            BNode *recordnode = new BNode(sRecordNodeName);

            for (unsigned int f = 0; f < cfields; f++) {
               BValue *value = record->getValue(f);
               BField *field = fields->getField(f);

               BNode *fieldnode = new BNode(field->name.getValue());
               fieldnode->content.setValue(value->asString());

               recordnode->addChildNode(fieldnode);
            }

            tablenode->addChildNode(recordnode);
         }
      }
   }

   return tablenode;
}

Groundfloor::BNode *Groundfloor::BTableToNodeAttributes(const BTable *aTable, const char *sTableNodeName, const char *sRecordNodeName) {
   BNode *tablenode = NULL;
   if (aTable != NULL) {
      BFields *fields = aTable->fields.get();
      BRecords *records = aTable->records.get();

      if ((fields != NULL) && (records != NULL)) {
         tablenode = new BNode(sTableNodeName);

         unsigned int cfields = fields->size();
         unsigned int crecords = records->size();
         for (unsigned int r = 0; r < crecords; r++) {
            BRecord *record = records->getRecord(r);
            BNode *recordnode = new BNode(sRecordNodeName);

            for (unsigned int f = 0; f < cfields; f++) {
               BValue *value = record->getValue(f);
               BField *field = fields->getField(f);

               BAttribute *attr = new BAttribute(field->name.getValue(), value->asString()->getValue());
               recordnode->addAttribute(attr);
            }

            tablenode->addChildNode(recordnode);
         }
      }
   }

   return tablenode;
}
