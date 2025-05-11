#ifndef _MODEL_H
#define _MODEL_H
#include<_common.h>
#include<_textureloader.h>

class _model
{
    public:
        _model();
        virtual ~_model();

        _textureLoader *myTex = new _textureLoader();

        vec3 pos;//position the model;
        vec3 rot;//angels for rotate the model;
        vec3 scale;//to scale the model

        void initModel(const char *); //initialize the model
        void drawModel();
    protected:

    private:
};

#endif // _MODEL_H
