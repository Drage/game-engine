
#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include "Shader.h"
#include "ParamList.h"

namespace DrageEngine
{
    class Material
    {
        public:
            Material();
            bool Load(const std::string &filename);

            Shader* GetShader() const;
            void ApplyUniforms() const;
        
        private:
            std::string m_name;
            Shader *m_shader;
        
            typedef struct Attribute
            {
                Shader::Uniform::Type type;
                int location;
                void *value;
            } UniformAttribute;
            typedef std::vector<Attribute> AttributeList;
        
            template<typename T>
            void AddAttribute(int location, Shader::Uniform::Type type, T value)
            {
                Attribute attribute;
                attribute.location = location;
                attribute.type = type;
                attribute.value = malloc(sizeof(T));
                *(T*)(attribute.value) = value;
                m_attributes.push_back(attribute);
            }
        
            AttributeList m_attributes;
    };
}

#endif
