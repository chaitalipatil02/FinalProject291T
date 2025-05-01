#include "_textureloader.h"

_textureLoader::_textureLoader()
{
    //ctor
}

_textureLoader::~_textureLoader()
{
    //dtor
}

void _textureLoader::loadImage(const char* fileName)
{
    glGenTextures(1,&texID);  // generate handler ID
    glBindTexture(GL_TEXTURE_2D, texID); // bind handler to the buffer

    image= SOIL_load_image(fileName,&width,&height,0,SOIL_LOAD_RGBA);

    if(!image)cout<<"Fail to load image "<<fileName<<endl;

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
    SOIL_free_image_data(image);

    glEnable(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLuint _textureLoader::loadImages(const char* fileName)
{
    glGenTextures(1,&texID);  // generate handler ID
    glBindTexture(GL_TEXTURE_2D, texID); // bind handler to the buffer

    image= SOIL_load_image(fileName,&width,&height,0,SOIL_LOAD_RGBA);

    if(!image)cout<<"Fail to load image "<<fileName<<endl;

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
    SOIL_free_image_data(image);

    glEnable(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

    return texID;
}


void _textureLoader::textureBinder()
{
    glBindTexture(GL_TEXTURE_2D,texID);
}
