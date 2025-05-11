#ifndef _TEXTURELOADER_H
#define _TEXTURELOADER_H

#include<_common.h>
#include<SOIL.h>

class _textureLoader
{
    public:
        _textureLoader();
        virtual ~_textureLoader();

        int width,height;
        unsigned char* image;
        GLuint texID;

        void loadImage(const char *);
        GLuint loadImages(const char *);
        void textureBinder();

    protected:

    private:
};

#endif // _TEXTURELOADER_H
