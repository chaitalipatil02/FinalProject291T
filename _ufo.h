#ifndef _UFO_H
#define _UFO_H
#include<_common.h>
#include<_timer.h>
#include<_textureloader.h>

struct UFOObject{
    float x,y,z;
    float speed;
    bool active;
};

class _ufo
{
    public:
        _ufo();
        virtual ~_ufo();

        _timer *myTime = new _timer();
        _textureLoader *myTex = new _textureLoader();

        UFOObject ufos[10];

        GLuint ufoTexID;

        void initUFO(const char *);
        void update(float);
        void draw();
        void spawnRandom(int);


    protected:

    private:
};

#endif // _UFO_H
