#include "_walls.h"

_walls::_walls()
{
    //ctor
    moveSpeedX = 0.05;
    moveDir = 1.0;
    wallPosX = 1.0;
    minX = 2.0;
    maxX = 10.0;
}

_walls::~_walls()
{
    //dtor
}
void _walls::draw(float x, float y, float z, float width, float height, float depth)
{
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(x, y, z);
    bTex->textureBinder();  // Bind the wall texture

    glBegin(GL_QUADS);

    // Front
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(width, height, 0);
    glTexCoord2f(0, 1); glVertex3f(0, height, 0);

    // Back
    glTexCoord2f(0, 0); glVertex3f(0, 0, -depth);
    glTexCoord2f(1, 0); glVertex3f(width, 0, -depth);
    glTexCoord2f(1, 1); glVertex3f(width, height, -depth);
    glTexCoord2f(0, 1); glVertex3f(0, height, -depth);

    // Left
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(0, 0, -depth);
    glTexCoord2f(1, 1); glVertex3f(0, height, -depth);
    glTexCoord2f(0, 1); glVertex3f(0, height, 0);

    // Right
    glTexCoord2f(0, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(width, 0, -depth);
    glTexCoord2f(1, 1); glVertex3f(width, height, -depth);
    glTexCoord2f(0, 1); glVertex3f(width, height, 0);

    // Top
    glTexCoord2f(0, 0); glVertex3f(0, height, 0);
    glTexCoord2f(1, 0); glVertex3f(width, height, 0);
    glTexCoord2f(1, 1); glVertex3f(width, height, -depth);
    glTexCoord2f(0, 1); glVertex3f(0, height, -depth);

    // Bottom
    glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
    glTexCoord2f(1, 0); glVertex3f(width, 0, 0);
    glTexCoord2f(1, 1); glVertex3f(width, 0, -depth);
    glTexCoord2f(0, 1); glVertex3f(0, 0, -depth);

    glEnd();
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}


void _walls::wallsInit(char* fileName)
{
    bTex->loadImage(fileName);
    wallPosX = 0.0f;
    moveSpeedX = 0.05f;   // Movement speed
    moveDir = 1.0f;       // Start going right
    minX = -20.0f;
    maxX = 20.0f;
}

void _walls::updateMovement()
{
    wallPosX += moveSpeedX * moveDir;

    if (wallPosX > maxX || wallPosX < minX) {
        moveDir *= -1;  // Reverse direction
    }
}

