
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "Transform.h"
#include "ParamList.h"

namespace DrageEngine
{
    class Shader
    {
        public:
            typedef struct Uniform
            {
                enum Type
                {
                    Float = 5126,
                    Int = 5124,
                    Vec2 = 35664,
                    Vec3 = 35665,
                    Vec4 = 35666,
                    Texture = 35678,
                    Cubemap = 35680,
                } type;
                int location;
                std::string name;
            } Uniform;
            typedef std::vector<Uniform> UniformList;
        
            Shader();
            ~Shader();
        
            bool Load(const std::string &filename);
            bool CompileFromSource(const std::string &vertexShaderCode, const std::string &fragmentShaderCode);
            unsigned CompileShader(int shaderType, const std::string& shaderCode);
            unsigned CompileProgram(unsigned vertexShaderID, unsigned fragmentShaderID);
        
            unsigned GetID() const;
            unsigned GetVertexShaderId() const;
            unsigned GetFragmentShaderId() const;
            const std::string& GetName() const;
        
            int GetAttributeLocation(const std::string &name) const;
            int GetUniformLocation(const std::string &name) const;
        
            void SetUniform(int location, const Matrix4x4 &m) const;
            void SetUniform(int location, const Vector2 &v) const;
            void SetUniform(int location, const Vector3 &v) const;
            void SetUniform(int location, const Vector4 &v) const;
            void SetUniform(int location, const Color &c) const;
            void SetUniform(int location, float f) const;
            void SetUniform(int location, int i) const;
            void SetUniform(const std::string &name, const Matrix4x4 &m) const;
            void SetUniform(const std::string &name, const Vector2 &v) const;
            void SetUniform(const std::string &name, const Vector3 &v) const;
            void SetUniform(const std::string &name, const Vector4 &v) const;
            void SetUniform(const std::string &name, const Color &c) const;
            void SetUniform(const std::string &name, float f) const;
            void SetUniform(const std::string &name, int i) const;
            void SetUniform(const std::string &array, int index, const std::string &name, const Matrix4x4 &m) const;
            void SetUniform(const std::string &array, int index, const std::string &name, const Vector2 &v) const;
            void SetUniform(const std::string &array, int index, const std::string &name, const Vector3 &v) const;
            void SetUniform(const std::string &array, int index, const std::string &name, const Vector4 &v) const;
            void SetUniform(const std::string &array, int index, const std::string &name, const Color &c) const;
            void SetUniform(const std::string &array, int index, const std::string &name, float f) const;
            void SetUniform(const std::string &array, int index, const std::string &name, int i) const;
        
            const UniformList& GetActiveUniforms() const;
            const ParamList& GetDefaultUniformValues() const;
        
        private:
            void InsertIncludes(std::string &shaderSource) const;
            void CheckAndLogStatus(unsigned shaderID, int statusType);
        
            std::string GetArrayUniformName(const std::string &arrayName, int index, const std::string &propertyName) const;
        
            static const std::string BUILT_IN_UNIFORMS[];
            bool IsUniformBuiltIn(const std::string &uniform) const;
            void LoadUniforms();
        
            std::string name;
            unsigned vertexShaderId;
            unsigned fragmentShaderId;
            unsigned id;
            UniformList uniforms;
            ParamList defaults;
    };
}

#endif
