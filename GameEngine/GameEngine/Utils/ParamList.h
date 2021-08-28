
#ifndef PARAMLIST_H
#define PARAMLIST_H

#include <string>
#include <sstream>
#include <map>
#include <typeinfo>

namespace DrageEngine
{
    typedef std::map<std::string, std::string> ParamMapType;
    
    class ParamList
    {
        public:
            template<typename T>
            T Get(const std::string &name, T defaultValue = T()) const
            {
                // Find param in map
                ParamMapType::const_iterator i = params.find(name);
                if (i == params.end())
                    return defaultValue;
                
                // Ugly hack to make strings return the whole string including spaces
                if (typeid(T) == typeid(std::string))
                    return *((T*)((void*)&i->second));
                
                // Convert param value to desired type
                T result;
                std::stringstream(i->second) >> result;
                return result;
            }
        
            template<typename T>
            void Set(const std::string &name, T value)
            {
                // Convert value type to string
                std::string s;
                std::stringstream out;
                out << value;
                s = out.str();
                
                // Update map
                params[name] = s;
            }
        
            std::string Value(const std::string &name) const;
            void Merge(const ParamList &other, bool otherOverride = true);
            bool Remove(const std::string &name);
            const ParamMapType& GetParamMap() const;
            void Clear();
            static bool Load(const std::string &filename, ParamList &params);
        
        private:
            ParamMapType params;
    };
}

#endif
