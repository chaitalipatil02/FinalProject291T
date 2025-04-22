#ifndef _SKYBOX_H
#define _SKYBOX_H

#include<_common.h>
#include<_textureloader.h>

class _skyBox
{
    public:
        _skyBox();
        virtual ~_skyBox();

        _textureLoader *textures=new _textureLoader();

        GLuint tex[6];

        vec3 pos;   //  for the translations
        vec3 rot;   //  for rotations
        vec3 boxSize;//for scaling

        float xMin,yMin,xMax,yMax;

        void skyBoxInit();
        void drawSkyBox();

    protected:
    private:
};

#endif // _SKYBOX_H
