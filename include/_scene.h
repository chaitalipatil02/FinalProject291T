#ifndef _SCENE_H
#define _SCENE_H

#include<_common.h>

struct Bullet {
    float x, y, z;
    float dirX, dirY, dirZ;
    bool active;
};

class _scene
{
    public:
        _scene();
        virtual ~_scene();

        GLvoid resizeWindow(GLsizei width, GLsizei height);// resizing window
        GLint IniGL();                                    //initialization
        GLvoid renderScene();                              // Draw Scene
        GLvoid renderLevelOne();
        GLvoid renderLevelTwo();
        GLvoid renderLevelThree();

        int winMsg(HWND	hWnd,			// Handle For This Window
                   UINT	uMsg,			// Message For This Window
                   WPARAM wParam,			// Additional Message Information
                   LPARAM	lParam);

         vec2 dim;
         vec3 playerPositions[15];
         Bullet bullets[10]; // max 10 bullets at once


         bool isFalling[15];     // Add in _scene.h
            int fallFramesLeft[15]; // Frames left to play fall animation


         float playerPosX, playerPosZ, playerAngleY;

          // Functions to draw individual screen scenes
         void drawLandingScreen();
         void drawMenuScreen();
         void drawHelpScreen();
         void drawPausePopup();
         void drawExitConfirmPopup();
         void drawText(float, float, const char *, void *, float, float, float);
         void resetAllScenes();

         void drawRoundedRect(float x, float y, float width, float height, float radius, float r, float g, float b, float borderThickness);

         // UI element positions and sizes
         float btnX, btnY, btnWidth, btnHeight;
         float menuX, menuY, menuBtnW, menuBtnH;
         float startX, startY, startW, startH;
         float helpX, helpY, helpW, helpH;
         float infoX, infoY, infoW, infoH;
         float creditsX, creditsY, creditsW, creditsH;
         float exitX, exitY, exitW, exitH;
         float yesX, yesY, yesW, yesH;
         float noX, noY, noW, noH;
         float newGameX, newGameY, newGameW, newGameH;
         float crossX, crossY, crossW, crossH;
         float moveTimer, moveInterval;
         float cameraPitch;
         int columns;
         float spacingX, spacingZ, startplyX, startplyZ;


         int lastMouseX, lastMouseY;


         float shootDirX, shootDirZ;
         int shootTimer;

         // Flags to control UI and models
         bool isInGameScene();
         bool showLaser = false;
         bool allInactive;
         bool mouseDragging = false;
         bool firstPerson = true;
         bool inGameScene, exitInit;
         bool inLandingScene, inMenuScene, inHelpScene, inInfoScene, inCreditScene, inExitScene, inNewGame, inCross, isShooting;
         bool isLevelTwo, useLevelTwoTextures;      // Skybox for level two
         bool playerFacingLeft[15];  // true = facing left, false = facing right
         bool playerVisible[15];
         bool isLevelThree;

         bool q1pop = false;
         bool q1answer = false;
         int correctOption = 2;         // C is correct (0=A, 1=B, 2=C, 3=
         int attempts = 0;

         int killCount;
         float killTextTimer = 0.0;
         float gameTimer = 45.0;
         bool timerStarted = false;
         float lastTime = 0.0;
         bool levelComplete = false;





    protected:

    private:
};

#endif // _SCENE_H
