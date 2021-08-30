
#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include <vector>
#include <string>
#include "ParamList.h"

namespace DrageEngine
{
    class XMLDocument
        {
        public:
            typedef struct Element
            {
                std::string name;
                std::string value;
                std::vector<Element*> subElements;
                
                void ToParamList(ParamList &paramList, bool recursive = false) const;
                const Element* GetSubElement(const std::string &name) const;
            } Element;
            
            typedef std::vector<Element*> ElementList;
            
            Element root;
            
            XMLDocument();
            XMLDocument(const std::string &filename);
            ~XMLDocument();
            bool Load(const std::string &filename);
            
        private:
            char buffer[256];
            
            void ReadSubElements(Element *current, std::ifstream &file);
            void DeleteSubElements(Element *current);
    };
}

#endif
