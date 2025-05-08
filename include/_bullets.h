#ifndef _BULLETS_H
#define _BULLETS_H

#include <_common.h>
#include<_textureloader.h>
#include<_timer.h>

class _bullets
{
    public:
        _bullets();
        virtual ~_bullets();

        _textureLoader *tex = new _textureLoader();
        _timer *myTime = new _timer();

        vec3 pos;
        vec3 des;
        vec3 src;

        vec3 scale;
        float t=0; // parameter for the bullet path
        bool live = true;

        enum {READY,DEAD,SHOOT,HIT};

        void initBullet(char*);
        void drawBullet();
        void bulletAction();

        int actionTrigger;

    protected:

    private:
};

#endif // _BULLETS_H
