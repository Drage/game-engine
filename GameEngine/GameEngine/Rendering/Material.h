
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
            Material(const Material &other);
            bool Load(const std::string &filename);

            const Shader* GetShader() const;
            void ApplyUniforms() const;
        
            unsigned GetID() const;
        
            bool IsTransparent() const;
            bool IsUIOverlay() const;
            unsigned GetRenderPriority() const;
        
            template<typename T>
            void SetAttributeValue(const std::string &name, T value)
            {
                int location = shader->GetUniformLocation(name);
                for (int i = 0; i < attributes.size(); i++)
                {
                    if (attributes[i].location == location)
                    {
                        *(T*)(attributes[i].value) = value;
                        break;
                    }
                }
            }
        
        private:
            std::string name;
            Shader *shader;
            bool isTransparent;
            bool isUIOverlay;
            unsigned renderPriority;
        
            typedef struct Attribute
            {
                Shader::Uniform::Type type;
                int location;
                void *value;
            } UniformAttribute;
        
            typedef std::vector<Attribute> AttributeList;
            AttributeList attributes;
        
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
        
            static unsigned nextMaterialId;
            unsigned id;
    };
}

#endif
