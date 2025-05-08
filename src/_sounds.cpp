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
void _sounds::playMusic(char* fileName)
{
    eng->play2D(fileName,true);
}

void _sounds::PlaySoundA(char* fileName)
{
    //if(!eng->isCurrentlyPlaying(fileName))
        eng->play2D(fileName, false, false);
}

void _sounds::pauseSound(char* fileName)
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
