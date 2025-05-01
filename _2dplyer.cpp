#include "_2dplyer.h"

_2DPlyer::_2DPlyer()
{
    //ctor
    xMin=yMin =0.0;
    xMax=yMax =1.0;

    pos.x =0;
    pos.y =0;
    pos.z =-8;

    vertx[0].x = -0.5; vertx[0].y = -0.5; vertx[0].z=-1.0;
    vertx[1].x =  0.5; vertx[1].y = -0.5; vertx[1].z=-1.0;
    vertx[2].x =  0.5; vertx[2].y =  0.5; vertx[2].z=-1.0;
    vertx[3].x = -0.5; vertx[3].y =  0.5; vertx[3].z=-1.0;

    actiontrigger=0;
}

_2DPlyer::~_2DPlyer()
{
    //dtor
}
void _2DPlyer::ply2Dinit(char* fileName, int x, int y)
{
    myTex->loadImage(fileName);
    numCols = x;  // not frames.x
    numRows = y;
    currentFrame = 0;
    animRow = 0;
    numCols = 6;
    numRows = 4;
}


void _2DPlyer::drawPly2D()
{
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);
    myTex->textureBinder();

    // Calculate the correct frame size
    float frameWidth = 1.0f / numCols;
    float frameHeight = 1.0f / numRows;

    // Find which column and row the frame is
    int col = currentFrame % numCols;
    int row = animRow;  // Row is based on action (walk left, right etc.)

    // Now calculate texture UVs
    float xMin = col * frameWidth;
    float xMax = xMin + frameWidth;
    float yMin = row * frameHeight;
    float yMax = yMin + frameHeight;

    glBegin(GL_QUADS);
        glTexCoord2f(xMin, yMax); glVertex3f(vertx[0].x, vertx[0].y, vertx[0].z); // Top Left
        glTexCoord2f(xMax, yMax); glVertex3f(vertx[1].x, vertx[1].y, vertx[1].z); // Top Right
        glTexCoord2f(xMax, yMin); glVertex3f(vertx[2].x, vertx[2].y, vertx[2].z); // Bottom Right
        glTexCoord2f(xMin, yMin); glVertex3f(vertx[3].x, vertx[3].y, vertx[3].z); // Bottom Left
    glEnd();

    glPopMatrix();
}


void _2DPlyer::ply2DActions()
{
    switch(actiontrigger)
    {
        case STAND:
            animRow = 0;   // 1st row for STAND
            break;

        case WALKLEFT:
            animRow = 1;   // 2nd row for WALK LEFT
            break;

        case WALKRIGHT:
            animRow = 2;   // 3rd row for WALK RIGHT
            break;

        case WALKFRONT:
            animRow = 3;   // 4th row for WALK FRONT
            break;

        case WALKBACK:
            animRow = 4;   // 5th row for WALK BACK
            break;
    }

    // Animation: Move to next frame every 60 ms
    if (myTime->getTicks() > 60)
    {
        currentFrame++;
        if (currentFrame >= numCols)
            currentFrame = 0; // Loop back to first frame

        myTime->reset();
    }
}

