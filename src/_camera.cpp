#include "_camera.h"

_camera::_camera()
{
    //ctor
}

_camera::~_camera()
{
    //dtor
}
void _camera::camInit()
{
    eye.x =4; eye.y =0; eye.z =12;
    des.x =0; des.y =0; des.z =0;
    up.x  =0;  up.y =1;  up.z =0;

    step =0.5;

    distance =  sqrt(pow((des.x-eye.x),2)+
                     pow((des.y-eye.y),2)+
                     pow((des.z-eye.z),2));
}

void _camera::camReset()
{
    eye.x =0; eye.y =0; eye.z =10;
    des.x =0; des.y =0; des.z =0;
    up.x  =0;  up.y =1;  up.z =0;

    step =0.5;
    distance =  sqrt(pow((des.x-eye.x),2)+
                     pow((des.y-eye.y),2)+
                     pow((des.z-eye.z),2));
}

void _camera::rotationXY()
{
    eye.x = des.x +distance* cos(rotAngle.y*PI/180.0) * sin(rotAngle.x*PI/180.0);
    eye.y = des.y +distance * sin(rotAngle.y*PI/180.0);
    eye.z = des.z +distance * cos(rotAngle.y*PI/180.0) * cos(rotAngle.x*PI/180.0);
}

void _camera::rotationUD(vec3)
{

}

void _camera::camMoveFdBd(int dir)
{
    eye.z += dir*step;
    des.z += dir*step;
}

void _camera::camMoveLtRt(int dir)
{
    eye.x += dir*step;
    des.x += dir*step;
}


void _camera::setUPcamera()
{
    float radius = 10.0f;

    float radX = cameraAngleX * PI / 180.0f;
    float radY = cameraAngleY * PI / 180.0f;

    eye.x = des.x + radius * cos(radY) * sin(radX);
    eye.y = des.y + radius * sin(radY);
    eye.z = des.z + radius * cos(radY) * cos(radX);

    gluLookAt(eye.x, eye.y, eye.z,
              des.x, des.y, des.z,
              up.x, up.y, up.z);
}




