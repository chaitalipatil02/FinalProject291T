#include "_sounds.h"

_sounds::_sounds()
{
    //ctor
}

_sounds::~_sounds()
{
    //dtor
    eng->drop();
}
void _sounds::playMusic(const char* fileName)
{
    if (music) {
        music->stop(); // Stop any existing music
    }
    music = eng->play2D(fileName, true, false, true); // play in loop, paused = false, trackable = true
}

void _sounds::stopMusic()
{
    if (music) {
        music->setIsPaused(true);
    }
}

void _sounds::resumeMusic()
{
    if (music) {
        music->setIsPaused(false);
    }
}

void _sounds::playGunshot(const char* fileName)
{
    stopMusic();
    eng->play2D(fileName, false, false);  // play once
    resumeMusic(); // (Note: this resumes immediately — may overlap)
}


void _sounds::PlaySoundA(const char* fileName)
{
    //if(!eng->isCurrentlyPlaying(fileName))
        eng->play2D(fileName, false, false);
}

void _sounds::pauseSound(const char* fileName)
{
    eng->play2D(fileName, true, false);
}

int _sounds::initSound()
{
    if(!eng)
    {
        cout<<"ERROR: *** the sound engine could not Load"<<endl;

    }
    return 1;
}
