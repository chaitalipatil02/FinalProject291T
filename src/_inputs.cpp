#include "_inputs.h"

_inputs::_inputs()
{
    //ctor

    mouse_translate=false;
    mouse_rotate = false;

    cameraAngleX = 0.0;
    cameraAngleY = 20.0;
}

_inputs::~_inputs()
{
    //dtor
}
void _inputs::keyPressed(_model* mdl)
{
    switch(wParam)
    {
        case VK_LEFT:
              mdl->rot.y +=1.0;
             break;
        case VK_RIGHT:
              mdl->rot.y -=1.0;
             break;
        case VK_UP:
              mdl->rot.x +=1.0;
             break;
        case VK_DOWN:
              mdl->rot.x -=1.0;
             break;
    }

}

void _inputs::keyPressed(_parallax* prlx)
{
     switch(wParam)
    {
        case VK_LEFT:
            //  prlx->xMin -=0.005;
            //  prlx->xMax -=0.005;
             prlx->prlxScrollAuto("left",0.005);

             break;
        case VK_RIGHT:

             break;
        case VK_UP:

             break;
        case VK_DOWN:

             break;
    }

}
void _inputs::keyPressed(_skyBox* sky)
{
     switch(wParam)
    {
        case VK_LEFT:
              sky->rot.y -=1.0;

             break;
        case VK_RIGHT:
               sky->rot.y +=1.0;
             break;
        case VK_UP:

               sky->rot.x +=1.0;
             break;
        case VK_DOWN:
               sky->rot.x -=1.0;
             break;

        case VK_F2:
            sky->pos.z +=1.0;
            break;

        case VK_F3:
            sky->pos.z -=1.0;
            break;
    }

}
void _inputs::keyPressed(_2DPlyer* ply)
{
     switch(wParam)
    {
        case VK_LEFT:
              ply->actiontrigger=ply->WALKLEFT;

             break;
        case VK_RIGHT:
              ply->actiontrigger=ply->WALKRIGHT;
             break;
        case VK_UP:
              ply->actiontrigger=ply->WALKFRONT;

             break;
        case VK_DOWN:
              ply->actiontrigger=ply->WALKBACK;
             break;
    }
}

void _inputs::keyPressed(_3dmodelloader* ply,_3dmodelloader* W )
{
     switch(wParam)
    {
        case VK_LEFT:
              W->actionTrigger=ply->actionTrigger=ply->RUN;


             break;
        case VK_RIGHT:
             W->actionTrigger= ply->actionTrigger=ply->JUMP;
             break;
        case VK_UP:
             W->actionTrigger= ply->actionTrigger=ply->ATTACK;

             break;
        case VK_DOWN:
             W->actionTrigger= ply->actionTrigger=ply->WALKLEFT;
             break;

        default:
            W->actionTrigger=ply->actionTrigger=ply->STAND;  break;
    }


}





void _inputs::keyPressed(_camera* cm)
{
      switch(wParam)
    {
        case 0x57: // w
              cm->camMoveFdBd(-1);
             break;
        case 0x53: //s
              cm->camMoveFdBd(1);
             break;
        case  65: //a
            cm->camMoveLtRt(-1);
            break;
        case  68://d
            cm->camMoveLtRt(1);
             break;
        case VK_SPACE:
              cm->camReset();
              break;

        case 73: cm->rotAngle.x +=1;cm->rotationXY(); break;
        case 74: cm->rotAngle.y -=1;cm->rotationXY(); break;
        case 75: cm->rotAngle.x +=1;cm->rotationXY(); break;
        case 76: cm->rotAngle.y +=1;cm->rotationXY(); break;
    }
}

void _inputs::keyUp()
{
   switch(wParam)
   {
       default:
           cout<< "came here";
            break;
   }
}

void _inputs::mouseEventDown(_model* mdl, double x, double y)
{
       switch(wParam)
       {
       case MK_LBUTTON:
              mouse_rotate = true;
             break;
       case MK_RBUTTON:
               mouse_translate =true;
             break;
       case MK_MBUTTON:
             break;
       }
       prev_mouseX = x;
       prev_mousey = y;
}

void _inputs::mouseEventUp()
{
     mouse_rotate = false;
     mouse_translate = false;
}

void _inputs::mouseWheel(_model* mdl, double delta)
{
     mdl->pos.z +=delta/100.0;
}

void _inputs::mouseMove(_camera* cam, double x, double y)
{
    //if(mouse_translate)
    //{
    cam->des.y += (x-prev_mouseX)/3.0;
    cam->des.x += (y-prev_mousey)/3.0;
    //}
    if(mouse_rotate)
    {
       cam->des.x += (x-prev_mouseX)/100.0;
       cam->des.y -= (y-prev_mousey)/100.0;
    }
    prev_mouseX =x;
    prev_mousey =y;
}


void _inputs::mouseMove(_skyBox* mdl, double x, double y)
{
    if(mouse_rotate)
    {
        mdl->rot.y += (x-prev_mouseX)/3.0;
        mdl->rot.x += (y-prev_mousey)/3.0;
    }
    if(mouse_translate)
    {
       mdl->pos.x += (x-prev_mouseX)/100.0;
       mdl->pos.y -= (y-prev_mousey)/100.0;
    }
    prev_mouseX =x;
    prev_mousey =y;
}
