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

        int winMsg(HWND	hWnd,			// Handle For This Window
                   UINT	uMsg,			// Message For This Window
                   WPARAM wParam,			// Additional Message Information
                   LPARAM	lParam);

         vec2 dim;
         vec3 playerPositions[15];
         Bullet bullets[10]; // max 10 bullets at once


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

    protected:

    private:
};

#endif // _SCENE_H
