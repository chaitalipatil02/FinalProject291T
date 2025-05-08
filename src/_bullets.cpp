#include "_bullets.h"

_bullets::_bullets()
{
    //ctor
}

_bullets::~_bullets()
{
    //dtor
}

void _bullets::initBullet(char* fileName)
{
   pos.x =0;
   pos.y =0;
   pos.z =0;
   t=0;

   des.x =0;
   des.y = 20;
   des.z =-50;

   src.x =0;
   src.y =0;
   src.z =0;

   live =false;
}

void _bullets::drawBullet()
{
     glDisable(GL_TEXTURE_2D);
     glPushMatrix();

     if(live)
     {
     glTranslatef(pos.x,pos.y,pos.z);
     glutSolidSphere(10,20,20);
     }
     glPopMatrix();
     glEnable(GL_TEXTURE_2D);
}

void _bullets::bulletAction()
{

    switch(actionTrigger)
    {
        case READY:
            pos.x =0;
            pos.y=0;
            pos.z=0;

            src.x =0;
            src.y= 0;
            src.z =0;

            live = false;
            t=0;
            break;


        case SHOOT:

         if(live)
            {
          if(myTime->getTicks()<50)
              {
                  pos.x = src.x +t*(des.x - src.x);
                  pos.y = src.y +t*(des.y - src.y);
                  pos.z = src.z +t*(des.z - src.z);

                  src.x = pos.x;
                  src.y = pos.y;
                  src.z = pos.z;


              if(t<1)t+=0.0003;
              if(t>=1) actionTrigger = READY;
              }
                myTime->reset();
              }
            break;
        case HIT:
            // use this to show explosion any other action
            break;
    }
}
