#ifndef _SOUNDS_
#define _SOUNDS_

#include<_common.h>
#include<SNDS/irrKlang.h>

using namespace irrklang;

class _sounds
{
    public:
        _sounds();
        virtual ~_sounds();

        ISoundEngine *eng = createIrrKlangDevice();

        void playMusic(const char*);
        void playGunshot(const char*);
        void stopMusic();
        void resumeMusic();
        void PlaySoundA(const char*);
        void pauseSound(const char*);
        int initSound();
        bool gunshotActive = false;
        float gunshotTimer = 0.0; // track time since gunshot

        ISound* gunshotSound = nullptr;

        void StopAllSounds();

    protected:

    private:
        ISound* music = nullptr; // add this to manage background music
};

#endif // _SOUNDS_
