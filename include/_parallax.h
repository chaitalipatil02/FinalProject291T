#ifndef _PARALLAX_H
#define _PARALLAX_H

#include<_common.h>
#include<_textureloader.h>
#include<_timer.h>

class _parallax
{
    public:
        _parallax();
        virtual ~_parallax();

        _textureLoader *bTex = new _textureLoader();
        _timer *tmr = new _timer();

        void drawBkgrnd(float, float); //width & height
        void parallaxInit(char*);      //load with filename
        void prlxScrollAuto(string,float); //auto,direction,speed

        float xMin,xMax,yMin,yMax;

    protected:

    private:
};

#endif // _PARALLAX_H
