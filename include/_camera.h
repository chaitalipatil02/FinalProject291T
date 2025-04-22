#ifndef _CAMERA_H
#define _CAMERA_H
#include<_common.h>

class _camera
{
    public:
        _camera();
        virtual ~_camera();

        vec3 eye;       // for eye position
        vec3 des;       // looking at
        vec3 up;        // head direction


        float step;     // movement
        vec2 rotAngle;  // rotation angles
        float distance; // distance between eye and des

        void camInit(); // initialize the camera
        void camReset();// Reset camera

        void rotationXY(); // rotation on XY
        void rotationUD(vec3); // rotation on Up Down
        void camMoveFdBd(int dir);
        void camMoveLtRt(int dir);

        float cameraAngleX = 0.0;
        float cameraAngleY = 20.0;

        void setUPcamera();

    protected:

    private:
};

#endif // _CAMERA_H
