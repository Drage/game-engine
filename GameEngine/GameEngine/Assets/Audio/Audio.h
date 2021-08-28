
#ifndef AUDIO_H
#define AUDIO_H

#include <string>

namespace DrageEngine
{
    class Audio
    {
        public:
            Audio();
            virtual ~Audio();
            virtual bool Load(const std::string &filename);
        
            int GetFormat() const;
            int GetSize() const;
            int GetFrequency() const;
            unsigned char* GetData() const;
            const std::string& GetName() const;
        
        protected:
            std::string name;
            int size;
            int frequency;
            int format;
            unsigned char *data;
    };
}

#endif
