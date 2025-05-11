#ifndef _2DPLYER_H
#define _2DPLYER_H

#include <_common.h>
#include <_timer.h>
#include <_textureloader.h>

class _2DPlyer
{
    public:
        _2DPlyer();
        virtual ~_2DPlyer();

        _timer *myTime = new _timer();
        _textureLoader *myTex = new _textureLoader();

        vec3 pos; // player position
        vec3 rot; // player rotations
        vec2 pSize;//Player Scale

        vec2 frames;

        vec3 vertx[4]; // for drawing Quad

        enum{STAND,WALKLEFT,WALKRIGHT,WALKFRONT,WALKBACK};
        float xMin,xMax,yMin,yMax;

        int actiontrigger;

        void ply2Dinit(char *,int,int);
        void drawPly2D();
        void ply2DActions();
        float walkspeed;    // Speed of player movement

        int numCols, numRows; // number of frames horizontally and vertically
        int currentFrame;     // which frame is active
        int startFrame;       // start frame for this animation (like for WALKLEFT etc.)
        int endFrame;         // end frame for this animation
        int animRow;          // which row (Y) to use: WALKLEFT is row 1, WALKRIGHT is row 2, etc.

        float u0, v0, u1, v1;

    protected:

    private:
};

#endif // _2DPLYER_H
