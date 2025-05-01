#include "_model.h"

_model::_model()
{
    //ctor
    rot.x=rot.y=rot.z=0;      //init rotation
    scale.x=scale.y=scale.z=1;
    pos.x=pos.y =0; pos.z = -8.0;
}

_model::~_model()
{
    //dtor
}
void _model::drawModel()
{
   glPushMatrix();
    myTex->textureBinder();
    glColor3f(1.0,0.7,0.7);

    glTranslatef(pos.x,pos.y,pos.z);
    glRotatef(rot.x,1,0,0);
    glRotatef(rot.y,0,1,0);
    glRotatef(rot.z,0,0,1);
    glScalef(scale.x,scale.y,scale.z);

    glutSolidTeapot(1.5);// ToDo: change depend on the scene
    glPopMatrix();
}
void _model::initModel(const char* fileName)
{
    myTex->loadImage(fileName);

}
