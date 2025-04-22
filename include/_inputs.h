#ifndef _INPUTS_H
#define _INPUTS_H

#include<_common.h>
#include<_model.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_2dplyer.h>
#include <_camera.h>
#include<_3dmodelloader.h>
#include<_scene.h>



class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        void keyPressed(_model *);
        void keyUp(); // ToDo: address accordingly

        void keyPressed(_parallax *);
        void keyPressed(_skyBox *);
        void keyPressed(_2DPlyer *);
        void keyPressed(_3dmodelloader *,_3dmodelloader * );
        void keyPressed(_camera *);
        void keyPressed(_scene *);


        void mouseEventDown(_model*, double,double);
        void mouseEventUp();// ToDo: address accordingly

        void mouseWheel(_model*,double);
        void mouseMove(_skyBox*,double,double);

        double prev_mouseX;
        double prev_mousey;

        bool mouse_translate; // flag to indicate mouse move
        bool mouse_rotate; //flag to indicate mouse rotation

        WPARAM wParam;
        float cameraAngleX;
        float cameraAngleY;

    protected:

    private:
};

#endif // _INPUTS_H
