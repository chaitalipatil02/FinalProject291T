#include "_scene.h"
#include <_lighting.h>
#include <_model.h>
#include <_inputs.h>
#include <_parallax.h>
#include <_skybox.h>
#include <_2dplyer.h>
#include <_camera.h>
#include <_3dmodelloader.h>
#include <_ufo.h>
#include <_sounds.h>
#include <_walls.h>

bool useLevelTwoTextures = false;

_lighting *myLight = new _lighting();
_model *myModel = new _model();
_inputs *myKbMs = new _inputs();
_parallax *prlx = new _parallax();
_skyBox *sky = new _skyBox();
_2DPlyer *ply2D = new _2DPlyer();
_camera *cam = new _camera();
_3dmodelloader * mdl3D = new _3dmodelloader();
_3dmodelloader * mdl3DW = new _3dmodelloader(); // weapon
_ufo *ufo = new _ufo();     // Initializing meteors
// Initializing all menu options
_parallax *menu = new _parallax();
_parallax *help = new _parallax();
_parallax *info = new _parallax();
_parallax *credits = new _parallax();
_parallax *exits = new _parallax();
_sounds *snds = new _sounds();
_walls *wall1 = new _walls();
_walls *wall2 = new _walls();
_walls *wall3 = new _walls();
_parallax *q1 = new _parallax();
_parallax *gameOverScreen = new _parallax();

typedef struct{
    float time;
    float rot;
    bool fwd;
}plyDir;

plyDir playerRot = {120,0,true};

enum Scenes
{
    Landing = 0,
    NewGame = 1,
    Help = 2,
    Info = 3,
    Credit = 4,
    Exit = 5,
    LevelOne = 6,
    LevelTwo = 7
};

Scenes CurrentScene = Scenes::Landing;

_scene::_scene()
{
    //ctor
    playerPosX = 10.0;
    playerPosZ = 120.0;
    playerAngleY = -110.0;
    allInactive = true;
    inLandingScene = true;
    inMenuScene = false;
    inHelpScene = false;
    inCreditScene = false;
    inExitScene = false;
    inNewGame = false;
    inCross = false;
    shootDirX = 0.0f;
    shootDirZ = -1.0f;
    isShooting = false;
    isLevelTwo = false;
    useLevelTwoTextures = false;
    cameraPitch = 0.0;
    moveTimer = 0.0;
    columns = 5;
    spacingX = 8.0;
    spacingZ = 20.0;
    startplyX = -spacingX * (columns - 1) / 2.0 + 10; // center around 0
    startplyZ = 60.0;
    isLevelThree = false;
    killCount = 0;
    gameOver = false;
    gameOverTimer = 0.0;

    for (int i = 0; i < 15; i++) {
        int row = i / columns;
        int col = i % columns;
        playerPositions[i].x = startplyX + col * spacingX;
        playerPositions[i].y = 0.0f;
        playerPositions[i].z = startplyZ + row * spacingZ;
    }

    for (int i = 0; i < 10; i++) {
        bullets[i].active = false;
    }

    for (int i = 0; i < 15; i++) {
    playerFacingLeft[i] = rand() % 2; // Randomly true or false
}


    for (int i = 0; i < 15; i++) {
        playerVisible[i] = true;  // Initially all players are visible
    }

    for (int i = 0; i < 15; i++) {
        isFalling[i] = false;
        fallFramesLeft[i] = 0;
    }
}

_scene::~_scene()
{
    //dtor
}
GLvoid _scene::resizeWindow(GLsizei width, GLsizei height)
{
    GLfloat Ratio = (GLfloat)width/(GLfloat)height;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,Ratio,0.1,1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);
}

GLint _scene::IniGL()
{
    glClearColor(0.0,0.0,1.0,1.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    myLight->setupLight(GL_LIGHT0);
    myModel->initModel("images/sun.jpg");
    prlx->parallaxInit("images/landing.png");
    ply2D->ply2Dinit("images/char.png",8,9);
    mdl3D->initModel("images/models/aliensoldier/tris.md2");
    mdl3DW->initModel("images/models/aliensoldier/weapon.md2");
    ufo->initUFO("images/sun.jpg");
    wall1->wallsInit("images/wall.jpg");
    wall2->wallsInit("images/wall.jpg");
    wall3->wallsInit("images/wall.jpg");

    q1->parallaxInit("images/q1.png");
    gameOverScreen->parallaxInit("images/gameover.jpg");

    // Set custom movement bounds and positions
    wall1->wallPosX = -17.0;
    wall1->minX = -22.0;
    wall1->maxX = -12.0;
    wall1->moveSpeedX = 0.005;
    wall1->moveDir = 1.0;

    wall2->wallPosX = 0.0;
    wall2->minX = -5.0;
    wall2->maxX = 5.0;
    wall2->moveSpeedX = 0.003;
    wall2->moveDir = -1.0;

    wall3->wallPosX = 15.0;
    wall3->minX = 8.0;
    wall3->maxX = 18.0;
    wall3->moveSpeedX = 0.003;
    wall3->moveDir = 1.0;


    menu->parallaxInit("images/newgamefp.png");
    help->parallaxInit("images/helpfp.png");
    info->parallaxInit("images/infofp.png");
    credits->parallaxInit("images/creditsfp.png");
    exits->parallaxInit("images/exitfp.png");

    sky->skyBoxInit();
    sky->skyBoxInit2();

    cam->camInit();

    snds->initSound();
    snds->playMusic("sounds/main music.mp3");

    dim.x = GetSystemMetrics(SM_CXSCREEN);
    dim.y = GetSystemMetrics(SM_CYSCREEN);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

GLvoid _scene::renderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    static bool menuInitialized = false;

    if (inLandingScene)     // Draw landing scene
    {
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            prlx->drawBkgrnd(dim.x, dim.y); // Landing image
            glEnable(GL_LIGHTING);
        glPopMatrix();

        drawLandingScreen();
    }
    else if (inExitScene)       // Draw exit screen
    {
        exitInit = false;
        if (!exitInit)       // Pop-up exit - esc key pressed
        {
            exits->parallaxInit("images/exitfp.png");
            exitInit = true;
        }
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            exits->drawBkgrnd(dim.x, dim.y);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        drawMenuScreen();
    }
    else if (inMenuScene)       // Draw menu screen
    {
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            menu->drawBkgrnd(dim.x, dim.y); // menu parallax
            glEnable(GL_LIGHTING);
        glPopMatrix();

        drawMenuScreen();
    }
    else if (inHelpScene)       // Draw help screen
    {
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            help->drawBkgrnd(dim.x, dim.y);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }
    else if (inInfoScene)       // Draw info screen
    {
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            info->drawBkgrnd(dim.x, dim.y);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        drawMenuScreen();
    }
    else if (inCreditScene)     // Draw credit screen
    {
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            credits->drawBkgrnd(dim.x, dim.y);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        drawMenuScreen();
    }
    else if (inNewGame)     // Draw new game screen
    {
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            menu->drawBkgrnd(dim.x, dim.y); // menu parallax
            glEnable(GL_LIGHTING);
        glPopMatrix();

        drawMenuScreen();
    }
    else if (inCross)       // Draw exit icon on top left corner
    {
        glPushMatrix();
            glScalef(4.25, 4.25, 1.0);
            glDisable(GL_LIGHTING);
            exits->drawBkgrnd(dim.x, dim.y); // menu parallax
            glEnable(GL_LIGHTING);
        glPopMatrix();
        drawMenuScreen();
    }
    // Draw game when pressed enter - complete setup of player, weapon, camera
    else if (!inLandingScene && !inMenuScene && !inHelpScene && !inInfoScene &&
         !inCreditScene && !inExitScene && !inNewGame && !inCross)
    {
        if (isLevelThree)
            renderLevelThree();
        else if (isLevelTwo)
            renderLevelTwo();
        else
            renderLevelOne();
    }
    else if (!inLandingScene && !inMenuScene && !inHelpScene && !inInfoScene &&
    !inCreditScene && !inExitScene && !inNewGame && !inCross && !isInGameScene()) {
        inLandingScene = true;  // Force return to landing screen
    }

}

GLvoid _scene::renderLevelOne()
{
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Convert ms to seconds
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if(!gameOver){
        if (currentTime - lastAutoTurnTime > 4.0) {
            if (!playerRot.fwd && playerRot.time > 5) {
                playerRot.rot = -180;
                playerRot.fwd = true;  // Turn forward automatically
                lastAutoTurnTime = currentTime;
            }
        }
    }


    int movementDirection[15] = {1, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, 1, 1, -1, 1};

    if (!levelComplete && !q1pop) {
        gameTimer -= deltaTime;
        if (gameTimer <= 0 && killCount < 10) {
            gameOver = true;
            gameOverTimer = 5.0;
        }

        if (killCount >= 10 && gameTimer > 0) {
            // Advance to next level
            isLevelThree = false;
            isLevelTwo = true;
            useLevelTwoTextures = true;
            levelComplete = true;
        }
    }

    if(!gameOver)
    {
        if (playerRot.rot > 0) {
            playerAngleY += (playerRot.fwd ? 0.1 : -0.1);
            playerRot.rot -= 0.1;
            playerRot.time += 0.1;
        } else {
            if (playerRot.fwd){
                if (playerRot.time <= 0){
                    playerRot.rot = 180;
                    playerRot.fwd = false;
                } else {
                    playerRot.time -= 0.1;
                }
            } else {
                if (playerRot.time < 20)
                    playerRot.time += 0.1;
            }
        }
    }

    // Calculate forward direction based on player angle
        float angleRad = playerAngleY * PI / 180.0f;
        float lookX = cos(angleRad);
        float lookZ = sin(angleRad);

        // Set camera (eyes at player's position)
        gluLookAt(
            playerPosX, 1.7, playerPosZ,                       // Player's eye position
            playerPosX + lookX, 1.7, playerPosZ + lookZ,       // Look direction
            0.0, 1.0, 0.0                                   // Up vector
        );

        // Skybox, player
        glPushMatrix();                 // Draw skybox
            glTranslatef(0.0, 15.0, 0.0);
            glScalef(4.33,4.33,4.33);
            glDisable(GL_LIGHTING);
            sky->drawSkyBox();
            glEnable(GL_LIGHTING);
        glPopMatrix();

        // Timer to move players
        moveTimer += 0.0000000000000016;

        if (!gameOver){
            if (moveTimer >= moveInterval) {
                for (int i = 0; i < 15; i++) {
                    playerPositions[i].x += 0.001 * movementDirection[i];
                }
                moveTimer = 0.0f;
            }
        }


        // Draw players
        for (int i = 0; i < 15; i++) {
            if (!playerVisible[i]) continue;

            glPushMatrix();
            glTranslatef(playerPositions[i].x, playerPositions[i].y, playerPositions[i].z);
            glScalef(0.1, 0.1, 0.1);
            // Common X-axis rotation to stand upright
            glRotatef(-90, 1, 0, 0);

            // Rotate around Z to face left or right
            if (playerFacingLeft[i]) {
                glRotatef(180, 0, 0, 1); // Flip to face left
            } else {
                glRotatef(0, 0, 0, 1);   // Default (face right)
            }

            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);

            float speed = 0.002;  // Adjust speed as needed

                if (isFalling[i]) {
                    mdl3D->actionTrigger = _3dmodelloader::FALL;
                    fallFramesLeft[i]--;
                    if (fallFramesLeft[i] <= 0) {
                        playerVisible[i] = false;
                        isFalling[i] = false;
                    }
                } else {
                    // Move left or right
                    if (playerFacingLeft[i]) {
                        playerPositions[i].x -= speed;
                    } else {
                        playerPositions[i].x += speed;
                    }

                    // Occasionally change direction randomly
                    if (rand() % 200 == 0) {  // 1 in 200 frames (roughly)
                        playerFacingLeft[i] = !playerFacingLeft[i];
                    }

                    mdl3D->actionTrigger = _3dmodelloader::RUN;
                }


                mdl3D->actions();
                if(!gameOver) mdl3D->Draw();

                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        glPushMatrix();         // Draw weapon model at player's side
            // Calculate right vector based on player angle
            angleRad = playerAngleY * PI / 180.0;
            float rightX = cos(angleRad);
            float rightZ = -sin(angleRad);

            float weaponOffset = -1.0;

            glTranslatef(
                playerPosX + rightX * weaponOffset,
                1.0,
                playerPosZ + rightZ * weaponOffset
            );
            glRotatef(-playerAngleY, 0.0, 1.0, 0.0);  // Rotate gun with mouse-dragged camera
            glScalef(0.1, 0.1, 0.1);
            if(!gameOver) mdl3DW->actions();
            mdl3DW->Draw();
        glPopMatrix();

        for (int i = 0; i < 10; i++) {
            if (bullets[i].active) {
                // Move bullet
                bullets[i].x += bullets[i].dirX * 1.0;
                bullets[i].y += bullets[i].dirY * 1.0;
                bullets[i].z += bullets[i].dirZ * 1.0;

                // Draw bullet
                glPushMatrix();
                    glDisable(GL_LIGHTING);
                    glColor3f(1, 0, 0);
                    glTranslatef(bullets[i].x, bullets[i].y, bullets[i].z);
                    glutSolidSphere(0.1, 20, 20);
                    glEnable(GL_LIGHTING);
                glPopMatrix();
                glColor3f(1.0,1.0,1.0);

                // Check bullet collision with players
                for (int j = 0; j < 15; j++) {
                    if (!playerVisible[j]) continue;

                    float dx = bullets[i].x - playerPositions[j].x;
                    float dy = bullets[i].y - playerPositions[j].y;
                    float dz = bullets[i].z - playerPositions[j].z;
                    float distance = sqrt(dx*dx + dy*dy + dz*dz);

                    if (distance < 2.0) {
                        isFalling[j] = true;
                        fallFramesLeft[j] = 30; // Let’s say ~30 frames for fall animation
                        bullets[i].active = false;
                        killCount++;
                        killTextTimer = 2.0;
                        break;
                    }
                }

                // Deactivate if out of bounds
                if (bullets[i].x > 200 || bullets[i].x < -200 ||
                    bullets[i].z > 200 || bullets[i].z < -200 ||
                    bullets[i].y > 200 || bullets[i].y < -10) {
                    bullets[i].active = false;
                }
            }
        }

        // Freeze everything during Game Over popup
        if (showGameOverScreen) {
            gameOverDisplayTimer -= deltaTime;
            gameOver = true;

            glPushMatrix();
                glTranslatef(9.0,3.0,110.0);
                glScalef(3.25, 3.25, 1.0);
                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);
                gameOverScreen->drawBkgrnd(dim.x, dim.y);  // shows gameover.jpg
                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();

            if (gameOverDisplayTimer <= 0.0) {
                showGameOverScreen = false;
                gameOver = false;
                gameOverDisplayTimer = 0.0;

                if (gameOverDisplayTimer <= 0.0) {
                    showGameOverScreen = false;
                    gameOver = false;
                    gameOverDisplayTimer = 0.0;

                    // Reset scene flags
                    inLandingScene = true;
                    prlx->parallaxInit("images/landing.png");
                    inMenuScene = false;
                    inHelpScene = false;
                    inInfoScene = false;
                    inCreditScene = false;
                    inExitScene = false;
                    inNewGame = false;
                    inCross = false;
                    inGameScene = false;

                    // Reset level states
                    isLevelOne = false;
                    isLevelTwo = false;
                    isLevelThree = false;
                    levelComplete = false;
                    levelTwoStarted = false;
                    levelThreeStarted = false;

                    // Reset game variables
                    killCount = 0;
                    gameTimer = 45.0f;
                    q1pop = false;
                    q1answer = false;

                    for (int i = 0; i < 15; i++) {
                        playerVisible[i] = true;
                        isFalling[i] = false;
                        fallFramesLeft[i] = 0;
                    }

                    for (int i = 0; i < 10; i++) {
                        bullets[i].active = false;
                    }

                    // Reset player position
                    playerPosX = 10.0;
                    playerPosZ = 120.0;
                    playerAngleY = -110.0;
                }

            }

            return;  // <-- prevent everything else from running
        }

        if (killTextTimer > 0.0) {
            glPushMatrix();
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, dim.x, dim.y, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                glDisable(GL_LIGHTING);

                // Background box
                int boxX = dim.x / 2 - 120 + 11;
                int boxY = dim.y / 2 - 30;
                int boxWidth = 240;
                int boxHeight = 50;

                glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // Semi-transparent black
                glBegin(GL_QUADS);
                    glVertex2i(boxX, boxY);
                    glVertex2i(boxX + boxWidth, boxY);
                    glVertex2i(boxX + boxWidth, boxY + boxHeight);
                    glVertex2i(boxX, boxY + boxHeight);
                glEnd();

                glColor3f(1.0, 1.0, 1.0);
                glRasterPos2i(dim.x / 2 - 50, dim.y / 2);
                string message = to_string(killCount) + " Players Killed!";
                for (int i = 0; message[i] != '\0'; i++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
                }

                glEnable(GL_LIGHTING);
                glPopMatrix();
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
            glPopMatrix();

            killTextTimer -= 0.005;
        }

        if (killCount >= 10 && gameTimer > 0.0f) {
            isLevelTwo = true;
            isLevelThree = false;
            // Optional: reset killCount if needed
        }

        // Show countdown timer on screen
        glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, dim.x, dim.y, 0, -1, 1);  // Set 2D projection

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 1.0f, 1.0f);  // White text
            glRasterPos2i(20, 40);  // Top-left corner

            string timerText = "Time Left: " + to_string((int)gameTimer) + "s";
            for (char c : timerText) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }

            glEnable(GL_LIGHTING);

            glPopMatrix(); // ModelView
            glMatrixMode(GL_PROJECTION);
            glPopMatrix(); // Projection
            glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        if (gameTimer <= 0 && killCount < 10) {
            showGameOverScreen = true;
            gameOverDisplayTimer = 10.0f;
            //snds->PlaySoundA("sounds/gameover.mp3");
        }
}

//==============================================

GLvoid _scene::renderLevelTwo()
{
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Convert ms to seconds
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if(!gameOver){
        if (currentTime - lastAutoTurnTime > 4.0) {
            if (!playerRot.fwd && playerRot.time > 5) {
                playerRot.rot = -180;
                playerRot.fwd = true;  // Turn forward automatically
                lastAutoTurnTime = currentTime;
            }
        }
    }

    int movementDirection[15] = {1, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, 1, 1, -1, 1};

    if (!levelComplete && !q1pop) {
        gameTimer -= deltaTime;
        if (gameTimer <= 0 && killCount < 10) {
            gameOver = true;
            gameOverTimer = 10.0;
        }

        if (killCount >= 10 && gameTimer > 0) {
            // Advance to next level
            isLevelThree = false;
            isLevelTwo = true;
            useLevelTwoTextures = true;
            levelComplete = true;
        }
    }

    if (!levelTwoStarted) {
        killCount = 0;
        killTextTimer = 0.0f;
        gameTimer = 45.0; // reset the timer
        levelTwoStarted = true;

        // Reload new players for level two
        for (int i = 0; i < 15; i++) {
            playerVisible[i] = true;
            isFalling[i] = false;
            fallFramesLeft[i] = 0;

            int row = i / columns;
            int col = i % columns;
            playerPositions[i].x = startplyX + col * spacingX;
            playerPositions[i].y = 0.0f;
            playerPositions[i].z = startplyZ + row * spacingZ;
            playerFacingLeft[i] = rand() % 2;
        }
    }

    if(!gameOver){
        if (playerRot.rot > 0) {
            playerAngleY += (playerRot.fwd ? 0.1 : -0.1);
            playerRot.rot -= 0.1;
            playerRot.time += 0.1;
        } else {
            if (playerRot.fwd){
                if (playerRot.time <= 0){
                    playerRot.rot = 180;
                    playerRot.fwd = false;
                } else {
                    playerRot.time -= 0.1;
                }
            } else {
                if (playerRot.time < 20)
                    playerRot.time += 0.1;
            }
        }
    }

    // Calculate forward direction based on player angle
        float angleRad = playerAngleY * PI / 180.0f;
        float lookX = cos(angleRad);
        float lookZ = sin(angleRad);

        // Set camera (eyes at player's position)
        gluLookAt(
            playerPosX, 1.7, playerPosZ,                       // Player's eye position
            playerPosX + lookX, 1.7, playerPosZ + lookZ,       // Look direction
            0.0, 1.0, 0.0                                   // Up vector
        );

        // Skybox, player
        glPushMatrix();                 // Draw skybox
            glTranslatef(0.0, 15.0, 0.0);
            glScalef(4.33,4.33,4.33);
            glDisable(GL_LIGHTING);
            sky->drawSkyBox();
            glEnable(GL_LIGHTING);
        glPopMatrix();

        // Timer to move players
        moveTimer += 0.0000000000000016;

        if(!gameOver){
        if (moveTimer >= moveInterval) {
            for (int i = 0; i < 15; i++) {
                playerPositions[i].x += 0.001 * movementDirection[i];
            }
            moveTimer = 0.0f;
        }
        }

        // Draw players
        for (int i = 0; i < 15; i++) {
            if (!playerVisible[i]) continue;

            glPushMatrix();
                glTranslatef(playerPositions[i].x, playerPositions[i].y, playerPositions[i].z);
                glScalef(0.1, 0.1, 0.1);
                // Common X-axis rotation to stand upright
                glRotatef(-90, 1, 0, 0);

                // Rotate around Z to face left or right
                if (playerFacingLeft[i]) {
                    glRotatef(180, 0, 0, 1); // Flip to face left
                } else {
                    glRotatef(0, 0, 0, 1);   // Default (face right)
                }

                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);

                float speed = 0.002f;  // Adjust speed as needed

                if (isFalling[i]) {
                    mdl3D->actionTrigger = _3dmodelloader::FALL;
                    fallFramesLeft[i]--;
                    if (fallFramesLeft[i] <= 0) {
                        playerVisible[i] = false;
                        isFalling[i] = false;
                    }
                } else {
                    // Move left or right
                    if (playerFacingLeft[i]) {
                        playerPositions[i].x -= speed;
                    } else {
                        playerPositions[i].x += speed;
                    }

                    // Occasionally change direction randomly
                    if (rand() % 200 == 0) {  // 1 in 200 frames (roughly)
                        playerFacingLeft[i] = !playerFacingLeft[i];
                    }
                    mdl3D->actionTrigger = _3dmodelloader::RUN;
                }

                mdl3D->actions();
                if (!gameOver) mdl3D->Draw();

                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }

        glPushMatrix();         // Draw weapon model at player's side
            // Calculate right vector based on player angle
            angleRad = playerAngleY * PI / 180.0;
            float rightX = cos(angleRad);
            float rightZ = -sin(angleRad);

            float weaponOffset = -1.0;

            glTranslatef(
                playerPosX + rightX * weaponOffset,
                1.0,
                playerPosZ + rightZ * weaponOffset
            );
            glRotatef(-playerAngleY, 0.0, 1.0, 0.0);  // Rotate gun with mouse-dragged camera
            glScalef(0.1, 0.1, 0.1);
            if(!gameOver) mdl3DW->actions();
            mdl3DW->Draw();
        glPopMatrix();

        for (int i = 0; i < 10; i++) {
            if (bullets[i].active) {
                // Move bullet
                bullets[i].x += bullets[i].dirX * 1.0;
                bullets[i].y += bullets[i].dirY * 1.0;
                bullets[i].z += bullets[i].dirZ * 1.0;

                // Draw bullet
                glPushMatrix();
                    glDisable(GL_LIGHTING);
                    glColor3f(1, 0, 0);
                    glTranslatef(bullets[i].x, bullets[i].y, bullets[i].z);
                    glutSolidSphere(0.2, 20, 20);
                    glEnable(GL_LIGHTING);
                glPopMatrix();
                glColor3f(1.0,1.0,1.0);

                // Check bullet collision with players
                for (int j = 0; j < 15; j++) {
                    if (!playerVisible[j]) continue;

                    float dx = bullets[i].x - playerPositions[j].x;
                    float dy = bullets[i].y - playerPositions[j].y;
                    float dz = bullets[i].z - playerPositions[j].z;
                    float distance = sqrt(dx*dx + dy*dy + dz*dz);

                    if (distance < 2.0) {
                        isFalling[j] = true;
                        fallFramesLeft[j] = 30; // Let’s say ~30 frames for fall animation
                        bullets[i].active = false;

                        killCount++;
                        killTextTimer = 2.0;
                        break;
                    }
                }

                //snds->PlaySoundA("sounds/squid-game-gunshot.mp3");
                // Deactivate if out of bounds
                if (bullets[i].x > 200 || bullets[i].x < -200 ||
                    bullets[i].z > 200 || bullets[i].z < -200 ||
                    bullets[i].y > 200 || bullets[i].y < -10) {
                    bullets[i].active = false;
                }
            }
        }

        // Freeze everything during Game Over popup
        if (showGameOverScreen) {
            gameOverDisplayTimer -= deltaTime;
            gameOver = true;

            glPushMatrix();
                glTranslatef(9.0,3.0,110.0);
                glScalef(3.25, 3.25, 1.0);
                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);
                gameOverScreen->drawBkgrnd(dim.x, dim.y);  // shows gameover.jpg
                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();

            if (gameOverDisplayTimer <= 0.0) {
                showGameOverScreen = false;
                gameOver = false;
                gameOverDisplayTimer = 0.0;

                if (gameOverDisplayTimer <= 0.0) {
                    snds->stopMusic();
                    showGameOverScreen = false;
                    gameOver = false;
                    gameOverDisplayTimer = 0.0f;

                    // Reset scene flags
                    inLandingScene = true;
                    prlx->parallaxInit("images/landing.png");
                    inMenuScene = false;
                    inHelpScene = false;
                    inInfoScene = false;
                    inCreditScene = false;
                    inExitScene = false;
                    inNewGame = false;
                    inCross = false;
                    inGameScene = false;

                    // Reset level states
                    isLevelOne = false;
                    isLevelTwo = false;
                    isLevelThree = false;
                    levelComplete = false;
                    levelTwoStarted = false;
                    levelThreeStarted = false;

                    // Reset game variables
                    killCount = 0;
                    gameTimer = 45.0f;
                    q1pop = false;
                    q1answer = false;

                    for (int i = 0; i < 15; i++) {
                        playerVisible[i] = true;
                        isFalling[i] = false;
                        fallFramesLeft[i] = 0;
                    }

                    for (int i = 0; i < 10; i++) {
                        bullets[i].active = false;
                    }

                    // Reset player position
                    playerPosX = 10.0;
                    playerPosZ = 120.0;
                    playerAngleY = -110.0;
                }

            }

            return;  // <-- prevent everything else from running
        }

        // --- Wall 1 ---
        if(!gameOver){
        wall1->updateMovement();
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            wall1->draw(wall1->wallPosX, -3, 105, 15, 10, 1);
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        // --- Wall 2 ---
        wall2->updateMovement();
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            wall2->draw(wall2->wallPosX, -3, 65, 15, 10, 1);
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        glPopMatrix();

        // --- Wall 3 ---
        wall3->updateMovement();
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            wall3->draw(wall3->wallPosX, -3, 85, 15, 10, 1);
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        glPopMatrix();
        }

        if (killTextTimer > 0.0) {
            glPushMatrix();
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, dim.x, dim.y, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                glDisable(GL_LIGHTING);

                // Background box
                int boxX = dim.x / 2 - 120 + 11;
                int boxY = dim.y / 2 - 30;
                int boxWidth = 240;
                int boxHeight = 50;

                glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // Semi-transparent black
                glBegin(GL_QUADS);
                    glVertex2i(boxX, boxY);
                    glVertex2i(boxX + boxWidth, boxY);
                    glVertex2i(boxX + boxWidth, boxY + boxHeight);
                    glVertex2i(boxX, boxY + boxHeight);
                glEnd();

                glColor3f(1.0, 1.0, 1.0);
                glRasterPos2i(dim.x / 2 - 50, dim.y / 2);
                string message = to_string(killCount) + " Players Killed!";
                for (int i = 0; message[i] != '\0'; i++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
                }

                glEnable(GL_LIGHTING);
                glPopMatrix();
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
            glPopMatrix();

            killTextTimer -= 0.005;
        }

        if (killCount >= 10 && gameTimer > 0.0f) {
            isLevelTwo = false;
            isLevelThree = true;
            // Optional: reset killCount if needed
        }

        // Show countdown timer on screen
        glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, dim.x, dim.y, 0, -1, 1);  // Set 2D projection

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 1.0f, 1.0f);  // White text
            glRasterPos2i(20, 40);  // Top-left corner

            string timerText = "Time Left: " + to_string((int)gameTimer) + "s";
            for (char c : timerText) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }

            glEnable(GL_LIGHTING);

            glPopMatrix(); // ModelView
            glMatrixMode(GL_PROJECTION);
            glPopMatrix(); // Projection
            glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        if (gameTimer <= 0 && killCount < 10) {
            showGameOverScreen = true;
            gameOverDisplayTimer = 10.0f;
            //snds->PlaySoundA("sounds/gameover.mp3");
        }

}

//==============================================Level3

GLvoid _scene::renderLevelThree()
{

    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f; // Convert ms to seconds
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    if(!gameOver){
        if (currentTime - lastAutoTurnTime > 4.0) {
            if (!playerRot.fwd && playerRot.time > 5) {
                playerRot.rot = -180;
                playerRot.fwd = true;  // Turn forward automatically
                lastAutoTurnTime = currentTime;
            }
        }
    }

    int movementDirection[15] = {1, -1, 1, -1, -1, 1, -1, -1, 1, -1, -1, 1, 1, -1, 1};

    if (!levelComplete && !q1pop) {
        gameTimer -= deltaTime;
        if (gameTimer <= 0 && killCount < 10) {
            gameOver = true;
            gameOverTimer = 45.0;
            showGameOverScreen = true;
            gameOverDisplayTimer = 10.0f;
        }

        if (killCount >= 10 && gameTimer > 0) {
            // Advance to next level
            isLevelThree = false;
            isLevelTwo = true;
            useLevelTwoTextures = true;
            levelComplete = true;
        }
    }

    if (!levelThreeStarted) {
        q1pop = true;              // Show popup on level start
        killCount = 0;
        killTextTimer = 0.0f;
        gameTimer = 45.0; // reset the timer
        levelThreeStarted = true;  // Ensure it happens only once

        // Optional reset logic for players if needed:
        for (int i = 0; i < 15; i++) {
            playerVisible[i] = true;
            isFalling[i] = false;
            fallFramesLeft[i] = 0;

            int row = i / columns;
            int col = i % columns;
            playerPositions[i].x = startplyX + col * spacingX;
            playerPositions[i].y = 0.0f;
            playerPositions[i].z = startplyZ + row * spacingZ;
            playerFacingLeft[i] = rand() % 2;
        }

        // Optional: reset any kill/game counters for Level 3
        killCount = 0;
        killTextTimer = 0.0f;
    }

    if (q1pop && !gameOver) {
        glPushMatrix();
            glScalef(1.25, 1.25, 1.0);
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            q1->drawBkgrnd(dim.x, dim.y);  // draw popup
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        glPopMatrix();
    }


    if(!q1pop && !gameOver)
    {
        if (playerRot.rot > 0) {
            playerAngleY += (playerRot.fwd ? 0.1 : -0.1);
            playerRot.rot -= 0.1;
            playerRot.time += 0.1;
        } else {
            if (playerRot.fwd){
                if (playerRot.time <= 0){
                    playerRot.rot = 180;
                    playerRot.fwd = false;
                } else {
                    playerRot.time -= 0.1;
                }
            } else {
                if (playerRot.time < 20)
                    playerRot.time += 0.1;
               }
           }
    }

    // Calculate forward direction based on player angle
        float angleRad = playerAngleY * PI / 180.0f;
        float lookX = cos(angleRad);
        float lookZ = sin(angleRad);

        // Set camera (eyes at player's position)
        gluLookAt(
            playerPosX, 1.7, playerPosZ,                       // Player's eye position
            playerPosX + lookX, 1.7, playerPosZ + lookZ,       // Look direction
            0.0, 1.0, 0.0                                   // Up vector
        );

        // Skybox, player
        glPushMatrix();                 // Draw skybox
            glTranslatef(0.0, 15.0, 0.0);
            glScalef(4.33,4.33,4.33);
            glDisable(GL_LIGHTING);
            sky->drawSkyBox();
            glEnable(GL_LIGHTING);
        glPopMatrix();


        if(!q1pop && !gameOver)
        {
            // Timer to move players
            moveTimer += 0.0000000000000016;

            if (moveTimer >= moveInterval) {
                for (int i = 0; i < 15; i++) {
                    playerPositions[i].x += 0.001 * movementDirection[i];
                }
                moveTimer = 0.0;
            }
        }

        // Draw players
        for (int i = 0; i < 15; i++) {
            if (!playerVisible[i]) continue;

            glPushMatrix();
                glTranslatef(playerPositions[i].x, playerPositions[i].y, playerPositions[i].z);
                glScalef(0.1, 0.1, 0.1);
                // Common X-axis rotation to stand upright
                glRotatef(-90, 1, 0, 0);

                // Rotate around Z to face left or right
                if (playerFacingLeft[i]) {
                    glRotatef(180, 0, 0, 1); // Flip to face left
                } else {
                    glRotatef(0, 0, 0, 1);   // Default (face right)
                }

                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);

                float speed = 0.002f;  // Adjust speed as needed

                if(!q1pop)
                {
                    if (isFalling[i]) {
                        mdl3D->actionTrigger = _3dmodelloader::FALL;
                        fallFramesLeft[i]--;
                        if (fallFramesLeft[i] <= 0) {
                            playerVisible[i] = false;
                            isFalling[i] = false;
                        }
                    } else {
                        // Move left or right
                        if (playerFacingLeft[i]) {
                            playerPositions[i].x -= speed;
                        } else {
                            playerPositions[i].x += speed;
                        }

                        // Occasionally change direction randomly
                        if (rand() % 200 == 0) {  // 1 in 200 frames (roughly)
                            playerFacingLeft[i] = !playerFacingLeft[i];
                        }

                        mdl3D->actionTrigger = _3dmodelloader::RUN;
                    }
                }


                mdl3D->actions();
                if (!gameOver) mdl3D->Draw();

                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }



        glPushMatrix();         // Draw weapon model at player's side
            // Calculate right vector based on player angle
            angleRad = playerAngleY * PI / 180.0;
            float rightX = cos(angleRad);
            float rightZ = -sin(angleRad);

            float weaponOffset = -1.0;
            glTranslatef(
                playerPosX + rightX * weaponOffset,
                1.0,
                playerPosZ + rightZ * weaponOffset
            );
            glRotatef(-playerAngleY, 0.0, 1.0, 0.0);  // Rotate gun with mouse-dragged camera
            glScalef(0.1, 0.1, 0.1);
            if (!gameOver) mdl3DW->actions();
            mdl3DW->Draw();
        glPopMatrix();

        for (int i = 0; i < 10; i++) {
            if (bullets[i].active) {
                // Move bullet
                if(!q1pop)
                {
                    bullets[i].x += bullets[i].dirX * 1.0;
                    bullets[i].y += bullets[i].dirY * 1.0;
                    bullets[i].z += bullets[i].dirZ * 1.0;
                }

            // Draw bullet
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glColor3f(1, 0, 0);
                glTranslatef(bullets[i].x, bullets[i].y, bullets[i].z);
                glutSolidSphere(0.2, 20, 20);
                glEnable(GL_LIGHTING);
            glPopMatrix();
            glColor3f(1.0,1.0,1.0);

            // Check bullet collision with players
            for (int j = 0; j < 15; j++) {
                if (!playerVisible[j]) continue;

                float dx = bullets[i].x - playerPositions[j].x;
                float dy = bullets[i].y - playerPositions[j].y;
                float dz = bullets[i].z - playerPositions[j].z;
                float distance = sqrt(dx*dx + dy*dy + dz*dz);

                if (distance < 2.0) {
                    isFalling[j] = true;
                    fallFramesLeft[j] = 30; // Let’s say ~30 frames for fall animation
                    bullets[i].active = false;
                    killCount++;
                    killTextTimer = 2.0;
                    break;
                }



            }

            //snds->PlaySoundA("sounds/squid-game-gunshot.mp3");
            // Deactivate if out of bounds
            if (bullets[i].x > 200 || bullets[i].x < -200 ||
                bullets[i].z > 200 || bullets[i].z < -200 ||
                bullets[i].y > 200 || bullets[i].y < -10) {
                bullets[i].active = false;
            }
        }
    }

        // Freeze everything during Game Over popup
        if (showGameOverScreen) {
            gameOverDisplayTimer -= deltaTime;
            gameOver = true;

            glPushMatrix();
                glTranslatef(9.0,3.0,110.0);
                glScalef(3.25, 3.25, 1.0);
                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);
                gameOverScreen->drawBkgrnd(dim.x, dim.y);  // shows gameover.jpg
                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();

            if (gameOverDisplayTimer <= 0.0) {
                showGameOverScreen = false;
                gameOver = false;
                gameOverDisplayTimer = 0.0;

                if (gameOverDisplayTimer <= 0.0) {
                    snds->stopMusic();
                    showGameOverScreen = false;
                    gameOver = false;
                    gameOverDisplayTimer = 0.0f;

                    // Reset scene flags
                    inLandingScene = true;
                    prlx->parallaxInit("images/landing.png");
                    inMenuScene = false;
                    inHelpScene = false;
                    inInfoScene = false;
                    inCreditScene = false;
                    inExitScene = false;
                    inNewGame = false;
                    inCross = false;
                    inGameScene = false;

                    // Reset level states
                    isLevelOne = false;
                    isLevelTwo = false;
                    isLevelThree = false;
                    levelComplete = false;
                    levelTwoStarted = false;
                    levelThreeStarted = false;

                    // Reset game variables
                    killCount = 0;
                    gameTimer = 45.0f;
                    q1pop = false;
                    q1answer = false;

                    for (int i = 0; i < 15; i++) {
                        playerVisible[i] = true;
                        isFalling[i] = false;
                        fallFramesLeft[i] = 0;
                    }

                    for (int i = 0; i < 10; i++) {
                        bullets[i].active = false;
                    }

                    // Reset player position
                    playerPosX = 10.0;
                    playerPosZ = 120.0;
                    playerAngleY = -110.0;
                }

            }

            return;  // <-- prevent everything else from running
        }

        if (wall1Visible) {
        if(!q1pop && !gameOver) wall1->updateMovement();
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);
                wall1->draw(wall1->wallPosX, -3, 105, 15, 10, 1);
                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }



        // --- Wall 2 ---
        if (wall2Visible){
        if(!q1pop && !gameOver) wall2->updateMovement();
        glPushMatrix();
            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            wall2->draw(wall2->wallPosX, -3, 65, 15, 10, 1);
            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);
        glPopMatrix();
        }

        if (wall3Breaking) {
            for (int i = 0; i < 50; i++) {
                if (!wall3Fragments[i].active) continue;

                // Update fragment
                wall3Fragments[i].x += wall3Fragments[i].vx;
                wall3Fragments[i].y += wall3Fragments[i].vy;
                wall3Fragments[i].z += wall3Fragments[i].vz;
                wall3Fragments[i].vy -= 0.01f;  // gravity

                wall3Fragments[i].lifetime -= 0.016f; // Assuming 60FPS
                if (wall3Fragments[i].lifetime <= 0.0f) {
                    wall3Fragments[i].active = false;
                }

                // Draw fragment
                glPushMatrix();
                glTranslatef(wall3Fragments[i].x, wall3Fragments[i].y, wall3Fragments[i].z);
                glColor3f(0.6f, 0.3f, 0.2f); // brick color
                glutSolidCube(0.6);  // smaller cubes
                glPopMatrix();
            }

            // Stop breaking once all fragments expire
            bool stillActive = false;
            for (int i = 0; i < 50; i++) {
                if (wall3Fragments[i].active) {
                    stillActive = true;
                    break;
                }
            }
            if (!stillActive) wall3Breaking = false;
        }

        else if (wall3Visible) {
            if (!q1pop && !gameOver) wall3->updateMovement();
            glPushMatrix();
                glDisable(GL_LIGHTING);
                glEnable(GL_TEXTURE_2D);
                wall3->draw(wall3->wallPosX, -3, 85, 15, 10, 1);
                glDisable(GL_TEXTURE_2D);
                glEnable(GL_LIGHTING);
            glPopMatrix();
        }


        if (killTextTimer > 0.0) {
            glPushMatrix();
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glLoadIdentity();
                glOrtho(0, dim.x, dim.y, 0, -1, 1);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glLoadIdentity();

                glDisable(GL_LIGHTING);

                // Background box
                int boxX = dim.x / 2 - 120 + 11;
                int boxY = dim.y / 2 - 30;
                int boxWidth = 240;
                int boxHeight = 50;

                glColor4f(0.0f, 0.0f, 0.0f, 0.6f); // Semi-transparent black
                glBegin(GL_QUADS);
                    glVertex2i(boxX, boxY);
                    glVertex2i(boxX + boxWidth, boxY);
                    glVertex2i(boxX + boxWidth, boxY + boxHeight);
                    glVertex2i(boxX, boxY + boxHeight);
                glEnd();

                glColor3f(1.0, 1.0, 1.0);
                glRasterPos2i(dim.x / 2 - 50, dim.y / 2);
                string message = to_string(killCount) + " Players Killed!";
                for (int i = 0; message[i] != '\0'; i++) {
                    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, message[i]);
                }

                glEnable(GL_LIGHTING);
                glPopMatrix();
                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
            glPopMatrix();

            killTextTimer -= 0.005;
        }

        if (killCount >= 10 && gameTimer > 0.0f) {
            isLevelTwo = false;
            isLevelThree = false;
            inMenuScene = true;
            // Optional: reset killCount if needed
        }

        // Show countdown timer on screen
        glPushMatrix();
            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, dim.x, dim.y, 0, -1, 1);  // Set 2D projection

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 1.0f, 1.0f);  // White text
            glRasterPos2i(20, 40);  // Top-left corner

            string timerText = "Time Left: " + to_string((int)gameTimer) + "s";
            for (char c : timerText) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }

            glEnable(GL_LIGHTING);

            glPopMatrix(); // ModelView
            glMatrixMode(GL_PROJECTION);
            glPopMatrix(); // Projection
            glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

}

bool _scene::isInGameScene()
{
    return !inLandingScene && !inMenuScene && !inHelpScene &&
           !inInfoScene && !inCreditScene && !inNewGame &&
           !inCross && !inExitScene;
}


void _scene::drawLandingScreen()
{
    // Setup positions for Enter, Exit, Menu buttons

    // Switch to 2D (Orthographic projection)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, dim.x, dim.y, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Enter Game Button (rectangle)
    btnWidth = 500;
    btnHeight = 80;
    btnX = dim.x / 2 - btnWidth / 2;
    btnY = dim.y - 130;


    // Menu Button (hamburger)
    menuX = dim.x - 80;
    menuY = 5;
    menuBtnW = 80;
    menuBtnH = 50;

    //Exit button on top-left of landing page
    crossW = 80;
    crossH = 50;
    crossX = 35;
    crossY = 5;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // Restore projection
    glPopMatrix(); // ModelView
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

}

// Draw button coordinates and layout for menu screen
void _scene::drawMenuScreen()
{
    startW = 300;
    startH = 100;
    startX = 800;
    startY = 400;

    helpW = 300;
    helpH = 60;
    helpX = 100;
    helpY = 370;

    infoW = 250;
    infoH = 60;
    infoX = 120;
    infoY = 500;

    creditsW = 250;
    creditsH = 60;
    creditsX = 120;
    creditsY = 600;

    exitW = 250;
    exitH = 60;
    exitX = 120;
    exitY = 700;

    yesW = 250;
    yesH = 60;
    yesX = 750;
    yesY = 425;

    noW = 260;
    noH = 60;
    noX = 910;
    noY = 425;

    newGameW = 250;
    newGameH = 60;
    newGameX = 195;
    newGameY = 260;

}

int _scene::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_KEYDOWN:
        //myKbMs->keyPressed(snds, "sounds/squid_game_alarm.mp3");
    switch (wParam)
    {
    case VK_SPACE:      // Shoot meteors when space key pressed
        showLaser = true;
        cout << "Laser on" << endl;     // debugging
        break;

    case VK_ESCAPE:
        cout << "ESC pressed - switching to exit scene" << endl;
        if (isInGameScene() || inMenuScene) {
            inMenuScene = false;
            inExitScene = true;     // Exit pop when esc key pressed
        }
        else if (inExitScene) {
            PostQuitMessage(0);
        }
        else {
            inExitScene = false;
            inLandingScene = false;
            inMenuScene = true;
            inHelpScene = false;
            inInfoScene = false;
            inCreditScene = false;
            inNewGame = false;
            inCross = false;
        }
        break;

    case VK_RETURN:     // Enter into the game when enter key pressed on landing page
        if (inLandingScene)
            inLandingScene = false;
        break;

    case 'I':    // Press i -> Switch to Level One
        useLevelTwoTextures = false;
        isLevelTwo = false;
        isLevelThree = false;
        cout << "Switched to Level One" << endl;
        break;

    case 'J':    // Press j -> Switch to Level Two
        useLevelTwoTextures = true;
        isLevelTwo = true;
        isLevelThree = false;
        cout << "Switched to Level Two" << endl;
        break;

    case 'K':   // Press k -> Switch to Level Three
        useLevelTwoTextures = false;
        isLevelTwo = false;
        isLevelThree = true;
        cout << "Switched to Level Three" << endl;
        break;

    case 'P':  // Press 'P' to toggle popup
        if (isLevelThree) {
            q1pop = !q1pop;
            cout << (q1pop ? "Showing" : "Hiding") << " word popup" << endl;
        }
        break;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
        if (q1pop && !q1answer) {
            int selectedOption = -1;
            if (wParam == 'A') selectedOption = 0;
            else if (wParam == 'B') selectedOption = 1;
            else if (wParam == 'C') selectedOption = 2;
            else if (wParam == 'D') selectedOption = 3;

            if (selectedOption != -1) {
                if (selectedOption == correctOption) {
                    q1answer = true;
                    q1pop = false;

                    if (wall3Visible && !wall3Breaking) {
                        wall3Breaking = true;
                        wall3Visible = false;

                        snds->PlaySoundA("sounds/wall hitting.mp3");

                        for (int i = 0; i < 50; i++) {
                            wall3Fragments[i].x = wall3->wallPosX + (rand() % 15 - 7); // Width
                            wall3Fragments[i].y = 2.0f + (rand() % 10); // Height
                            wall3Fragments[i].z = 85.0f + (rand() % 3 - 1); // Depth

                            wall3Fragments[i].vx = (rand() % 200 - 100) / 100.0f * 0.2;
                            wall3Fragments[i].vy = (rand() % 100) / 100.0f * 0.5;
                            wall3Fragments[i].vz = (rand() % 200 - 100) / 100.0f * 0.2;

                            wall3Fragments[i].lifetime = 15.0;
                            wall3Fragments[i].active = true;
                        }
                    }
                }

                 else {
                    q1answer = true;
                    q1pop = false;

                    showGameOverScreen = true;
                    gameOverDisplayTimer = 10.0f;
                    snds->PlaySoundA("sounds/gameover.mp3"); // Add your own game over sound file
                }
            }
        }
        break;

    default:
        // Game movement
        if (!inLandingScene && !inMenuScene && !inHelpScene &&
            !inInfoScene && !inCreditScene && !inNewGame && !inCross && !inExitScene)
        {
            float moveSpeed = 0.2;
            float angleRad = playerAngleY * PI / 180.0;
            float forwardX = cos(angleRad);
            float forwardZ = sin(angleRad);
            float rightX = sin(angleRad);
            float rightZ = -cos(angleRad);

            if (playerRot.rot <= 0) {
                switch (wParam)
                {
                    case VK_LEFT:
                        if (playerRot.fwd) {
                            playerRot.rot = 180;
                            playerRot.time += 5;
                            playerRot.fwd = false;
                        }
                        break;
                    case VK_RIGHT:
                         if (!playerRot.fwd && playerRot.time > 5) {
                                cout << "twist" << endl;
                            playerRot.rot = 180;
                            playerRot.fwd = true;
                         }
                         break;
                }
            }

            /*switch (wParam)
            {
                case 'W': // Forward
                    playerPosX += moveSpeed * forwardX;
                    playerPosZ += moveSpeed * forwardZ;
                    break;

                case 'S': // Backward
                    playerPosX -= moveSpeed * forwardX;
                    playerPosZ -= moveSpeed * forwardZ;
                    break;

                case 'A': // Left strafe
                    playerPosX -= moveSpeed * rightX;
                    playerPosZ -= moveSpeed * rightZ;
                    break;

                case 'D': // Right strafe
                    playerPosX += moveSpeed * rightX;
                    playerPosZ += moveSpeed * rightZ;
                    break;
            }*/
        }
        break;
    }
    break;

    case WM_LBUTTONDOWN:
{
    int mouseX = LOWORD(lParam);
    int mouseY = HIWORD(lParam);

    // === LANDING SCREEN ===
    if (inLandingScene)
    {
        if (mouseX >= btnX && mouseX <= btnX + btnWidth &&
            mouseY >= btnY && mouseY <= btnY + btnHeight)
        {
            inLandingScene = false;
        }

        if (mouseX >= menuX && mouseX <= menuX + menuBtnW &&
            mouseY >= menuY && mouseY <= menuY + menuBtnH)
        {
            inLandingScene = false;
            inMenuScene = true;
        }

        if (mouseX >= crossX && mouseX <= crossX + crossW &&
            mouseY >= crossY && mouseY <= crossY + crossH)
        {
            inLandingScene = false;
            inCross = true;
        }
    }

    // === ALL PARALLAX SCREENS (menu/help/info/credits) ===
    else if (inMenuScene || inHelpScene || inInfoScene || inCreditScene || inExitScene || inCross)
    {
        // Switch to Help
        if (mouseX >= helpX && mouseX <= helpX + helpW &&
            mouseY >= helpY && mouseY <= helpY + helpH)
        {
            inMenuScene = false;
            inHelpScene = true;
            inInfoScene = false;
            inCreditScene = false;
            inExitScene = false;
            inNewGame = false;
            inCross = false;
        }

        // Switch to Info
        if (mouseX >= infoX && mouseX <= infoX + infoW &&
            mouseY >= infoY && mouseY <= infoY + infoH)
        {
            inMenuScene = false;
            inHelpScene = false;
            inInfoScene = true;
            inCreditScene = false;
            inExitScene = false;
            inNewGame = false;
            inCross = false;
        }

        // Switch to Credits
        if (mouseX >= creditsX && mouseX <= creditsX + creditsW &&
            mouseY >= creditsY && mouseY <= creditsY + creditsH)
        {
            inMenuScene = false;
            inHelpScene = false;
            inInfoScene = false;
            inCreditScene = true;
            inExitScene = false;
            inNewGame = false;
            inCross = false;
        }

        // Switch to Exit (shows parallax)
        if (mouseX >= exitX && mouseX <= exitX + exitW &&
            mouseY >= exitY && mouseY <= exitY + exitH)
        {
            inMenuScene = false;
            inHelpScene = false;
            inInfoScene = false;
            inCreditScene = false;
            inExitScene = true;  // Show exit parallax
            inNewGame = false;
            inCross = false;
        }

        // New Game button accessible from ANY screen
        if (mouseX >= newGameX && mouseX <= newGameX + newGameW &&
            mouseY >= newGameY && mouseY <= newGameY + newGameH)
        {
            inLandingScene = false;
            inHelpScene = false;
            inInfoScene = false;
            inCreditScene = false;
            inExitScene = false;
            inMenuScene = true;
            inCross = false;
        }

        if (inExitScene || inCross )
        {
            // YES Button: Close the window
            if (mouseX >= yesX && mouseX <= yesX + yesW &&
                mouseY >= yesY && mouseY <= yesY + yesH)
            {
                PostQuitMessage(0);  // Close the game
            }

            // NO Button: Stay in game
            if (mouseX >= noX && mouseX <= noX + noW &&
                mouseY >= noY && mouseY <= noY + noH)
            {
                // Go back to game
                inExitScene = false;
                // Put user in game mode
                inLandingScene = false;
                inMenuScene = false;
                inHelpScene = false;
                inCreditScene = false;
                inInfoScene = false;
                inNewGame = false;
                inCross = false;

                inGameScene = true;
                exitInit = false;
            }
        }

        // Start Game
        if (inMenuScene &&
            mouseX >= startX && mouseX <= startX + startW &&
            mouseY >= startY && mouseY <= startY + startH)
        {
            inMenuScene = false;
            inHelpScene = false;
            inInfoScene = false;
            inCreditScene = false;
            inExitScene = false;
            inLandingScene = false;
            inNewGame = false;
            inCross = false;

            inGameScene = true;
        }
    }
    else if (!q1pop && isInGameScene())
    {
        float normalizedX = (2.0f * mouseX) / dim.x - 1.0f;
        float normalizedY = 1.0f - (2.0f * mouseY) / dim.y;

        // Setup projection and view matrices
        GLdouble modelview[16], projection[16];
        GLint viewport[4];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        // Unproject mouse to world coordinates
        GLdouble worldX, worldY, worldZ;
        gluUnProject(mouseX, viewport[3] - mouseY, 0.5, modelview, projection, viewport, &worldX, &worldY, &worldZ);

        // Direction
        float dirX = worldX - playerPosX;
        float dirY = worldY - 1.7f; // player eye height
        float dirZ = worldZ - playerPosZ;

        // Normalize direction
        float length = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
        dirX /= length;
        dirY /= length;
        dirZ /= length;

        for (int i = 0; i < 10; i++) {
            if (!bullets[i].active) {
                bullets[i].x = playerPosX + dirX * 2.0f;  // move bullet 2 units ahead
                bullets[i].y = 1.7f + dirY * 2.0f;
                bullets[i].z = playerPosZ + dirZ * 2.0f;
                bullets[i].dirX = dirX;
                bullets[i].dirY = dirY;
                bullets[i].dirZ = dirZ;
                bullets[i].active = true;
                snds->PlaySoundA("sounds/squid-game-gunshot.mp3");
                break;
            }
        }
    }
    mouseDragging = true;
    lastMouseX = LOWORD(lParam);
    lastMouseY = HIWORD(lParam);
    SetCapture(hWnd);  // lock mouse input to your window
    break;

    break;
}

    case WM_KEYUP:
        if(wParam == VK_SPACE){
            cout << "Laser off" << endl;
            showLaser = false;
        }
        break;

    case WM_LBUTTONUP:
    {
        mouseDragging = false;
        ReleaseCapture();  // release mouse input
        break;
    }
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_MOUSEMOVE:
    {
        if (!inLandingScene && !inMenuScene && !inHelpScene && !inInfoScene &&
         !inCreditScene && !inExitScene && !inNewGame && !inCross && !q1pop)
         {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            int dx = mouseX - lastMouseX;
            int dy = mouseY - lastMouseY;

            playerAngleY += dx * 0.03f;     // Yaw
            cameraPitch  -= dy * 0.03f;     // Pitch (inverted Y-axis)

            // Clamp Y to avoid flipping
            if (cam->cameraAngleY > 89.0) cam->cameraAngleY = 89.0;
            if (cam->cameraAngleY < -89.0) cam->cameraAngleY = -89.0;

            if (cameraPitch > 89.0) cameraPitch = 89.0;
            if (cameraPitch < -89.0) cameraPitch = -89.0;


            lastMouseX = mouseX;
            lastMouseY = mouseY;
         }
        break;
    }

    case WM_MOUSEWHEEL:
        break;
    }

    return 0;
}
