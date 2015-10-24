
#ifndef __GFBNode_H
#define __GFBNode_H

namespace Groundfloor {
   class BNode;
   class BNodePath;
}

#include "../Atoms/Freeable.h"
#include "../Molecules/Vector.h"
#include "BAttribute.h"
#include <vector>

namespace Groundfloor {
   class BNodePath : public Freeable {
   protected:
      std::vector<unsigned int> path;

      int current;
   public:
      /// path initialization to length zero
      BNodePath();
      ~BNodePath();

      /// appends i to path
      void append(unsigned int i);
      /// clears entire path vector
      void clear();

      /// resets path pointer
      unsigned int traverse_reset();

      /// returns previous value
      unsigned int traverse_previous();
      /// returns first or next value
      unsigned int traverse_next();

      /// returns true if path pointer is at the beginning of the path
      bool traverse_isBoF();
      /// returns true if path pointer is at the end of the path, or when the path length is zero
      bool traverse_isEoF();

      void readFromString(const char *sPath);
   };

   /// BNode is a class tailored to represent simple XML Nodes containing attributes in key=value format, and nesting of child-nodes
   class BNode : public Freeable {
   protected:
      Vector *arrAttributes;
      Vector *arrChildren;
      BNode *parent;

   public:
      /// initialize node with no name
      BNode();
      /// initialize node with given name
      BNode(const char *sName);
      /// automatically deletes all attributes and child-nodes
      ~BNode();

      String name;
      String content;

      BNode *getParentNode();

      /// adds attribute to the node, given attribute is automatically deleted
      void addAttribute(BAttribute *anAttrib);
      /// adds child-node to the node, given node is automatically deleted
      void addChildNode(BNode *aNode);

      /// number of child-nodes
      unsigned int childCount() const;
      /// number of attributes
      unsigned int attributeCount() const;

      /// copy attributes from given node
      void copyAttributes(BNode *aNode);

      /// returns attribute at index i, returns NULL when out of bounds
      BAttribute *getAttribute(unsigned int i) const;
      /// returns attribute if found by name
      BAttribute *getAttributeByName(const char *sName) const;
      /// returns node at index i, returns NULL when out of bounds
      BNode *getChild(unsigned int i) const;

      /// returns node that has the given name, or NULL
      BNode *getChildByName(const char *sName) const;

      /// returns node that has the given name and where the attribute with given name has the given value
      BNode *getChildByNameAndAttribute(const char *sName, const char *sAttributeName, const BValue *sAttributeValue) const;

      /// returns node that contains the given attribute with the given value, searches the complete tree
      BNode *findAttributeThroughRecursiveSearch(const char *sAttributeName, const BValue *sAttributeValue);

      /// find or create and add an attribute with given name
      BAttribute *findOrCreateAttribute(const char *sName);

      /// traverses 1 position in given NodePath and uses current path reference as index, returns NULL if anything went wrong
      BNode *getChildAndTraverse(BNodePath *aPath) const;

      /// traverses the whole path and returns the last valid child-node in the path, if no valid nodes are found in the path; "this" is returned
      BNode *getChildByPath(BNodePath *aPath);

      /// links pointer to content from either the first child, or itself if no children
      String *getFirstChildsContent();

      /// flatten all child contents into a newly allocated string
      String *flattenChildContent();

   };
}

#endif // __GFBNode_H
