
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

            const Shader* GetShader() const;
            void ApplyUniforms() const;
        
            unsigned GetID() const;
        
            bool IsTransparent() const;
            unsigned GetRenderPriority() const;
        
        private:
            std::string name;
            Shader *shader;
            bool transparent;
            unsigned renderPriority;
        
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
                attributes.push_back(attribute);
            }
        
            AttributeList attributes;
        
            static unsigned nextMaterialId;
            unsigned id;
    };
}

#endif
