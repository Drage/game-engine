
#define GL_GLEXT_PROTOTYPES
#include <SDL_opengl.h>
#include <OpenGL/gl.h>
#include <fstream>
#include <sstream>
#include "Shader.h"
#include "FileSystem.h"
#include "ParamList.h"
#include "AssetManager.h"
#include "Debug.h"
#include "XMLDocument.h"
#include "ArrayUtils.h"
#include "Application.h"

using namespace DrageEngine;

Shader::Shader()
{
    id = 0;
}

Shader::~Shader()
{
    if(id != 0)
        glDeleteProgram(id);
}

bool Shader::Load(const std::string &filename)
{
    name = GetFileName(filename);
    
    XMLDocument xml;
    if (!xml.Load(filename))
        return false;
    
    ParamList params;
    xml.root.ToParamList(params, true);
    
    std::string vertexShaderFile = params.Get<std::string>("vert");
    std::string fragmentShaderFile = params.Get<std::string>("frag");
    
    std::string vertexShaderPath = app->assets->GetAssetPath(vertexShaderFile);
    std::string fragmentShaderPath = app->assets->GetAssetPath(fragmentShaderFile);
    
    std::string vertexShaderCode = ReadFileContents(vertexShaderPath);
    std::string fragmentShaderCode = ReadFileContents(fragmentShaderPath);
    
    InsertIncludes(vertexShaderCode);
    InsertIncludes(fragmentShaderCode);
    
    bool compileSuccess = Compile(vertexShaderCode, fragmentShaderCode);
    
    const XMLDocument::Element *defaults = xml.root.GetSubElement("defaults");
    if (defaults != NULL)
        defaults->ToParamList(this->defaults);
    
    return compileSuccess;
}

bool Shader::Compile(const std::string &vertexShaderCode, const std::string &fragmentShaderCode)
{
    unsigned vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
    unsigned fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    
    id = CompileProgram(vertexShaderID, fragmentShaderID);

    LoadUniforms();
    
    return id != 0;
}

unsigned Shader::GetID() const
{
    return id;
}

const std::string& Shader::GetName() const
{
    return name;
}

int Shader::GetAttributeLocation(const std::string &name) const
{
    return glGetAttribLocation(id, name.c_str());
}

int Shader::GetUniformLocation(const std::string &name) const
{
    int loc = glGetUniformLocation(id, name.c_str());
    return loc;
}

void Shader::SetUniform(int location, const Matrix4x4 &m) const
{
    glUniformMatrix4fv(location, 1, false, m.ToArray());
}

void Shader::SetUniform(int location, const Vector2 &v) const
{
    glUniform2fv(location, 1, v.ToArray());
}

void Shader::SetUniform(int location, const Vector3 &v) const
{
    glUniform3fv(location, 1, v.ToArray());
}

void Shader::SetUniform(int location, const Vector4 &v) const
{
    glUniform4fv(location, 1, v.ToArray());
}

void Shader::SetUniform(int location, const Color &c) const
{
    glUniform3fv(location, 1, c.ToArray());
}

void Shader::SetUniform(int location, float f) const
{
    glUniform1f(location, f);
}

void Shader::SetUniform(int location, int i) const
{
    glUniform1i(location, i);
}

void Shader::SetUniform(const std::string &name, const Matrix4x4 &m) const
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, false, m.ToArray());
}

void Shader::SetUniform(const std::string &name, const Vector2 &v) const
{
    glUniform2fv(GetUniformLocation(name), 1, v.ToArray());
}

void Shader::SetUniform(const std::string &name, const Vector3 &v) const
{
    glUniform3fv(GetUniformLocation(name), 1, v.ToArray());
}

void Shader::SetUniform(const std::string &name, const Vector4 &v) const
{
    glUniform4fv(GetUniformLocation(name), 1, v.ToArray());
}

void Shader::SetUniform(const std::string &name, const Color &c) const
{
    glUniform3fv(GetUniformLocation(name), 1, c.ToArray());
}

void Shader::SetUniform(const std::string &name, float f) const
{
    glUniform1f(GetUniformLocation(name), f);
}

void Shader::SetUniform(const std::string &name, int i) const
{
    glUniform1i(GetUniformLocation(name), i);
}

void Shader::SetUniform(const std::string &array, int index, const std::string &name, const Matrix4x4 &m) const
{
    SetUniform(GetArrayUniformName(array, index, name), m);
}

void Shader::SetUniform(const std::string &array, int index, const std::string &name, const Vector2 &v) const
{
    SetUniform(GetArrayUniformName(array, index, name), v);
}

void Shader::SetUniform(const std::string &array, int index, const std::string &name, const Vector3 &v) const
{
    SetUniform(GetArrayUniformName(array, index, name), v);
}

void Shader::SetUniform(const std::string &array, int index, const std::string &name, const Vector4 &v) const
{
    SetUniform(GetArrayUniformName(array, index, name), v);
}

void Shader::SetUniform(const std::string &array, int index, const std::string &name, const Color &c) const
{
    SetUniform(GetArrayUniformName(array, index, name), c);
}

void Shader::SetUniform(const std::string &array, int index, const std::string &name, float f) const
{
    SetUniform(GetArrayUniformName(array, index, name), f);
}

void Shader::SetUniform(const std::string &array, int index, const std::string &name, int i) const
{
    SetUniform(GetArrayUniformName(array, index, name), i);
}

void Shader::InsertIncludes(std::string &shaderSource) const
{
    std::size_t start = shaderSource.find("#include \"");
    while (start != std::string::npos)
    {
        std::size_t end = shaderSource.find('"', start + 10);
        std::string includeFilename = shaderSource.substr(start + 10, end - start - 10);
    
        std::string includeShaderPath = app->assets->GetAssetPath(includeFilename);
        std::string includeShaderCode = ReadFileContents(includeShaderPath);
        
        // Remove #version from included code
        std::size_t versionStart = includeShaderCode.find("#version");
        if (versionStart != std::string::npos)
        {
            std::size_t versionEnd = shaderSource.find("\n", versionStart);
            includeShaderCode.erase(versionStart, versionEnd - versionStart + 1);
        }
        
        // Replace #include with the included source
        shaderSource = shaderSource.erase(start, end - start + 1);
        shaderSource.insert(start, includeShaderCode.c_str());
        
        start = shaderSource.find("#include \"", start);
    }
}

unsigned Shader::CompileShader(int shaderType, const std::string& shaderCode)
{
    unsigned shaderID = glCreateShader(shaderType);
    
    const char *source = shaderCode.c_str();
    glShaderSource(shaderID, 1, &source, NULL);
    
    glCompileShader(shaderID);
    
    CheckAndLogStatus(shaderID, GL_COMPILE_STATUS);

    return shaderID;
}

unsigned Shader::CompileProgram(unsigned vertexShaderID, unsigned fragmentShaderID)
{
    unsigned programID = glCreateProgram();
    
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    
    glLinkProgram(programID);
    
    CheckAndLogStatus(programID, GL_LINK_STATUS);
    
    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    
    return programID;
}

void Shader::CheckAndLogStatus(unsigned shaderID, int statusType)
{
    void (*GetInfo)(GLuint, GLenum, GLint*);
    GetInfo = statusType == GL_LINK_STATUS ? &glGetProgramiv : &glGetShaderiv;
    
    int result = GL_FALSE;
    GetInfo(shaderID, statusType, &result);
    
    if (result == GL_FALSE)
    {
        int infoLogLength;
        GetInfo(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        std::vector<char> errorMessage(infoLogLength+1);
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, &errorMessage[0]);
        ERROR(name + ": " + &errorMessage[0]);
    }
}

std::string Shader::GetArrayUniformName(const std::string &arrayName, int index, const std::string &propertyName) const
{
    std::ostringstream ss;
    ss << arrayName << "[" << index << "]." << propertyName;
    return ss.str();
}

const std::string Shader::BUILT_IN_UNIFORMS[] =
{
    "time",
    "modelMatrix",
    "viewMatrix",
    "projectionMatrix",
    "modelViewMatrix",
    "viewProjectionMatrix",
    "modelViewProjectionMatrix",
    "cameraPosition",
    "numLights",
    "lights[].position",
    "lights[].intensities",
    "lights[].attenuation",
    "lights[].ambientCoefficient",
    "lights[].coneAngle",
    "lights[].coneDirection",
};

bool Shader::IsUniformBuiltIn(const std::string &uniform) const
{
    std::string uniformName = std::string(uniform);
    
    // Remove array index
    size_t arrayStartIndex = uniformName.find('[');
    if (arrayStartIndex != std::string::npos)
    {
        size_t arrayEndIndex = uniformName.find(']');
        uniformName.replace(arrayStartIndex, arrayEndIndex - arrayStartIndex + 1, "[]");
    }
    
    for (int i = 0; i < Array::Length(BUILT_IN_UNIFORMS); i++)
    {
        if (uniformName.compare(BUILT_IN_UNIFORMS[i]) == 0)
            return true;
    }
    return false;
}

void Shader::LoadUniforms()
{
    GLint count;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);
    for (GLint i = 0; i < count; i++)
    {
        const GLsizei bufSize = 64;
        GLchar name[bufSize];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(id, (GLuint)i, bufSize, &length, &size, &type, name);
        
        std::string uniformName = std::string(name);
        if (!IsUniformBuiltIn(uniformName))
        {
            Uniform uniform;
            uniform.location = GetUniformLocation(uniformName);
            uniform.name = uniformName;
            uniform.type = (Uniform::Type)type;
            uniforms.push_back(uniform);
        }
    }
}

const Shader::UniformList& Shader::GetActiveUniforms() const
{
    return uniforms;
}

const ParamList& Shader::GetDefaultUniformValues() const
{
    return defaults;
}

