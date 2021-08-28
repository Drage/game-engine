
#ifndef AUDIOSOURCE_H
#define AUDIOSOURCE_H

#include "AudioClip.h"
#include "Vector3.h"

namespace DrageEngine
{
    class AudioSource
    {
        public:
            AudioSource();
            ~AudioSource();
        
            void SetClip(const AudioClip *clip);
            void Play(const AudioClip *clip);
            void Play();
            void Pause();
            void Stop();
            bool IsPlaying() const;
        
            float GetVolume() const;
            void SetVolume(float volume);
            
            float GetPitch() const;
            void SetPitch(float pitch);
        
            Vector3 GetPosition() const;
            void SetPosition(const Vector3 &position);
        
            Vector3 GetVelocity() const;
            void SetVelocity(const Vector3 &velocity);
        
            bool IsLooping() const;
            void SetLooping(bool loop);
        
        private:
            unsigned m_id;
            float m_volume;
            float m_pitch;
            Vector3 m_position;
            Vector3 m_velocity;
            bool m_looping;
    };
}

#endif
