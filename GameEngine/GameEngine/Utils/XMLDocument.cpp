
#include <fstream>
#include "XMLDocument.h"
#include "Debug.h"
#include "AssetManager.h"
#include "StringUtils.h"

using namespace DrageEngine;

XMLDocument::XMLDocument()
{
}

XMLDocument::XMLDocument(const std::string &filename)
{
    Load(filename);
}

XMLDocument::~XMLDocument()
{
    DeleteSubElements(&root);
}

bool XMLDocument::Load(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        ERROR("Unable to open XML file: " + filename);
        return false;
    }
    
    file.getline(buffer, sizeof(buffer), '<');
    file.getline(buffer, sizeof(buffer), '>');
    root.name = std::string(buffer);
    ReadSubElements(&root, file);
    return true;
}

void XMLDocument::ReadSubElements(Element *current, std::ifstream &file)
{
    std::string name, value;
    std::string end = "/" + current->name;
    
    bool comment = false;
    
    while (true)
    {
        file.getline(buffer, sizeof(buffer), '<');
        
        if (comment)
        {
            std::string str = std::string(buffer);
            if (str.find("-->") != std::string::npos)
                comment = false;
            else
                continue;
        }
        
        if (file.peek() == '!')
        {
            comment = true;
        }
        else if (file.peek() == '/')
        {
            current->value = std::string(buffer);
            file.getline(buffer, sizeof(buffer), '>');
            break;
        }
        else
        {
            Element* subElement = new Element();
            file.getline(buffer, sizeof(buffer), '>');
            subElement->name = std::string(buffer);
            current->subElements.push_back(subElement);
            ReadSubElements(subElement, file);
        }
    }
}

void XMLDocument::DeleteSubElements(Element *current)
{
    for (ElementList::iterator i = current->subElements.begin(); i != current->subElements.end(); i++)
    {
        DeleteSubElements(*i);
        delete *i;
    }
}

void XMLDocument::Element::ToParamList(ParamList &paramList, bool recursive) const
{
    for (ElementList::const_iterator i = subElements.begin(); i != subElements.end(); i++)
    {
        if (!String::IsNullOrEmpty((*i)->value))
            paramList.Set<std::string>((*i)->name, (*i)->value);
        
        if (recursive)
        {
            if ((*i)->subElements.size() > 0)
                (*i)->ToParamList(paramList, recursive);
        }
    }
}

const XMLDocument::Element* XMLDocument::Element::GetSubElement(const std::string &name) const
{
    for (ElementList::const_iterator i = subElements.begin(); i != subElements.end(); i++)
    {
        if ((*i)->name == name)
            return *i;
    }
    return NULL;
}
