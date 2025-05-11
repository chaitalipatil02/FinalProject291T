#include "_skybox.h"

extern bool useLevelTwoTextures;

_skyBox::_skyBox()
{
    //ctor
}

_skyBox::~_skyBox()
{
    //dtor
}
void _skyBox::skyBoxInit()
{
    glGenTextures(6,levelOneTextures);
    levelOneTextures[0] = textures->loadImages("images/squid.jpg");
    levelOneTextures[1] = textures->loadImages("images/squid.jpg");
    levelOneTextures[2] = textures->loadImages("images/squidsky.jpg");
    levelOneTextures[3] = textures->loadImages("images/astro.png");
    levelOneTextures[4] = textures->loadImages("images/squid.jpg");
    levelOneTextures[5] = textures->loadImages("images/squid.jpg");

    xMin = yMin = 0.0;
    xMax = yMax = 1.0;

    rot.x= rot.y = rot.z = 0.0;
    pos.x= pos.y = 0.0;
    pos.z = -9;
    boxSize.x = boxSize.z =30.0;
    boxSize.y = 5.0;

}

void _skyBox::skyBoxInit2()
{
    glGenTextures(6,levelTwoTextures);
    levelTwoTextures[0] = textures->loadImages("images/box/front.jpg");
    levelTwoTextures[1] = textures->loadImages("images/box/back.jpg");
    levelTwoTextures[2] = textures->loadImages("images/box/top.jpg");
    levelTwoTextures[3] = textures->loadImages("images/box/bottom.jpg");
    levelTwoTextures[4] = textures->loadImages("images/box/left.jpg");
    levelTwoTextures[5] = textures->loadImages("images/box/right.jpg");

    xMin = yMin = 0.0;
    xMax = yMax = 1.0;

    rot.x= rot.y = rot.z = 0.0;
    pos.x= pos.y = 0.0;
    pos.z = -9;
    boxSize.x = boxSize.z =30.0;
    boxSize.y = 5.0;

}

void _skyBox::drawSkyBox()
{
   glPushMatrix();
     // handling translation and scaling
      glColor3f(1.0,1.0,1.0);
     glScalef(boxSize.x,boxSize.y,boxSize.z);
  //   glTranslatef(pos.x,pos.y, pos.z);
     //handling rotations
     glRotatef(rot.x,1,0,0);
     glRotatef(rot.y,0,1,0);
     glRotatef(rot.z,0,0,1);

   //Front Wall
   glEnable(GL_TEXTURE_2D);

    GLuint* currentTextures;

    if (useLevelTwoTextures)
        currentTextures = levelTwoTextures;
    else
        currentTextures = levelOneTextures;


     glBindTexture(GL_TEXTURE_2D, currentTextures[0]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,1.0);
      glTexCoord2f(xMax,yMin); glVertex3f( 1.0,1.0,1.0);
      glTexCoord2f(xMax,yMax); glVertex3f( 1.0,-1.0,1.0);
      glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,1.0);
     glEnd();

   //back Wall
     glBindTexture(GL_TEXTURE_2D, currentTextures[1]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMax); glVertex3f(1.0, -1.0,-1.0);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,-1.0);
      glTexCoord2f(xMax,yMin); glVertex3f(-1.0, 1.0,-1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0, 1.0,-1.0);
     glEnd();

    //top wall
     glBindTexture(GL_TEXTURE_2D, currentTextures[2]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMax); glVertex3f( 1.0,1.0,-1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0,1.0,1.0);
      glTexCoord2f(xMax,yMin); glVertex3f( -1.0,1.0,1.0);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,1.0,-1.0);
     glEnd();

      //bottom wall
     glBindTexture(GL_TEXTURE_2D, currentTextures[3]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0, 1.0);
      glTexCoord2f(xMin,yMax); glVertex3f( 1.0,-1.0, 1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0,-1.0,-1.0);
      glTexCoord2f(xMax,yMin); glVertex3f(-1.0,-1.0,-1.0);
     glEnd();

     //left Wall
     glBindTexture(GL_TEXTURE_2D, currentTextures[4]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,-1.0);
      glTexCoord2f(xMax,yMin); glVertex3f(-1.0,1.0,1.0);
      glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,1.0);
      glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,-1.0);
     glEnd();

     //right wall
     glBindTexture(GL_TEXTURE_2D, currentTextures[5]);
     glBegin(GL_QUADS);
      glTexCoord2f(xMax,yMin); glVertex3f( 1.0,1.0,-1.0);
      glTexCoord2f(xMax,yMax); glVertex3f( 1.0,-1.0,-1.0);
      glTexCoord2f(xMin,yMax); glVertex3f( 1.0,-1.0,1.0);
      glTexCoord2f(xMin,yMin); glVertex3f( 1.0,1.0,1.0);
     glEnd();
     glDisable(GL_TEXTURE_2D);
     glDisable(GL_CULL_FACE);


   glPopMatrix();

}
