#ifndef _WALLS_H
#define _WALLS_H

#include <_common.h>
#include <_textureloader.h>

class _walls
{
    public:
        _walls();
        virtual ~_walls();

        float x, y, z;
        float width, height, depth;
        GLuint textureID;
        GLuint wallTexture;
        void wallsInit(char*);

        _textureLoader *bTex = new _textureLoader();
        void draw(float, float, float, float, float, float);

        float moveSpeedX, moveDir, wallPosX, minX, maxX;
        void updateMovement();

    protected:

    private:
};

#endif // _WALLS_H
