
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
            std::string m_name;
            int m_size;
            int m_frequency;
            int m_format;
            unsigned char *m_data;
    };
}

#endif
