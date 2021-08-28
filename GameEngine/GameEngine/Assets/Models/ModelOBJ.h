
#ifndef MODELOBJ_H
#define MODELOBJ_H

#include <fstream>
#include "Model.h"

namespace DrageEngine
{
    class ModelOBJ : public Model
    {
        public:
            ModelOBJ();
            ModelOBJ(const std::string &filename);
        
            bool Load(const std::string &filename);
        
        private:
            void ParseVertexCoord(std::ifstream &file, std::vector<float> &vertexCoords);
            void ParseTextureCoord(std::ifstream &file, std::vector<float> &textureCoords);
            void ParseNormal(std::ifstream &file, std::vector<float> &normals);
    };
}

#endif
