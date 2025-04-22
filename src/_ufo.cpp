#include "_ufo.h"

_ufo::_ufo() {
    // Constructor
}

_ufo::~_ufo() {
    // Destructor
}

void _ufo::initUFO(const char* fileName) {
    _textureLoader loader;
    ufoTexID = loader.loadImages(fileName);

    if (!ufoTexID) {
        cout << "Failed to load UFO texture: " << fileName << endl;
    } else {
        cout << "Loaded UFO texture: " << fileName << endl;
    }

    // Initialize all meteors with random spawn values and inactive
    for (int i = 0; i < 10; i++) {
        ufos[i].x = -10 + rand() % 20;
        ufos[i].z = -10 + rand() % 20;
        ufos[i].y = 20.0 + rand() % 20;  // high up
        ufos[i].speed = 0.02 + (rand() % 20) * 0.005;  // varied speed
        ufos[i].active = (i == 0);  // only the first one active
    }
}

// Spawn a single meteors at a random X/Z position and fixed Y height
void _ufo::spawnRandom(int index) {
    if (index >= 0 && index < 10) {
        ufos[index].x = (rand() % 200 - 100);  // Random X between -100 to +100
        ufos[index].z = (rand() % 200 - 100);  // Random Z between -100 to +100
        ufos[index].y = 50.0;                  // Spawn at a height of 50
        ufos[index].speed = 0.02;              // Set descent speed
        ufos[index].active = true;             // Mark it active
    }
}

// Update meteors: move them down based on speed, deactivate if landed
void _ufo::update(float deltaTime) {
    for (int i = 0; i < 10; i++) {
        if (ufos[i].active) {
            ufos[i].y -= ufos[i].speed * deltaTime;     // Move meteor down

            if (ufos[i].y <= 0.0) {
                ufos[i].y = 0.0;
                ufos[i].active = false;  // mark as landed
            }
        }
    }
}

// Draw all active meteors as textured spheres in the scene
void _ufo::draw() {
    if (ufoTexID <= 0) return;      // Exit if texture not loaded

    for (int i = 0; i < 10; i++) {
        if (ufos[i].active) {
            glPushMatrix();
                glTranslatef(ufos[i].x, ufos[i].y, ufos[i].z);      // Move to meteor position
                glColor3f(1.0, 1.0, 1.0);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, ufoTexID);
                GLUquadric* quad = gluNewQuadric();     // Create quadric for sphere
                gluQuadricTexture(quad, GL_TRUE);
                gluSphere(quad, 0.5, 20, 20);
                gluDeleteQuadric(quad);         // Clean up
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();
        }
    }
}
