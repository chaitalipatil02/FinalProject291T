#include "_parallax.h"

_parallax::_parallax()
{
    //ctor
    xMin =0.0;
    xMax =1.0;
    yMin =0.0;
    yMax =1.0;
}

_parallax::~_parallax()
{
    //dtor
}

void _parallax::drawBkgrnd(float width, float height)
{
    glColor3f(1.0,1.0,1.0);
    bTex->textureBinder();

    glBegin(GL_POLYGON);

       glTexCoord2f(xMin,yMax);
       glVertex3f(-width/height, -1,-10);

       glTexCoord2f(xMax,yMax);
       glVertex3f(width/height, -1,-10);

              glTexCoord2f(xMax,yMin);
       glVertex3f(width/height, 1,-10);

              glTexCoord2f(xMin,yMin);
       glVertex3f(-width/height, 1,-10);

    glEnd();

}

void _parallax::parallaxInit(char* fileName)
{
    bTex->loadImage(fileName);
}

void _parallax::prlxScrollAuto( string dir, float speed)
{
      if(tmr->getTicks()>50)
      {

         if(dir=="left")
         {
             xMin +=speed;
             xMax +=speed;
         }
          if(dir=="right")
         {
             xMin -=speed;
             xMax -=speed;
         }

          if(dir=="up")
         {
             yMin -=speed;
             yMax -=speed;
         }

          if(dir=="down")
         {
             yMin +=speed;
             yMax +=speed;
         }
         tmr->reset();
      } // End Timer
}


