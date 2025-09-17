
#include <stdio.h>
#include <gl/glut.h>
#include <math.h>                                   // For the compiler to recognize PI in order to draw the Pac-Man icon
#include <stdlib.h>                                 // for rand() and srand()
#include <time.h>                                   // for time()

// -------------------------------------------------------------------------------------------------------------------------

#define MAX_DOTS 100                                // maximum number of dots
float dotX[MAX_DOTS];                               // x-coordinates of the dots
float dotY[MAX_DOTS];                               // y-coordinates of the dots
int numDots = 0;                                    // current number of dots
float posX = 0.0;                                   // x-coordinate of Pac-Man
float posY = 0.0;                                   // y-coordinate of Pac-Man

// -------------------------------------------------------------------------------------------------------------------------

void RenderScene(void)
{
    // glClear(GL_COLOR_BUFFER_BIT);			    // Clear the color buffer (background), Cannot be used since the Display function is executed first
    glFlush();										// Flush drawing commands

}

// -------------------------------------------------------------------------------------------------------------------------

// define a new structure to represent a wall

typedef struct {
    float x1, y1; // coordinates of one end of the wall
    float x2, y2; // coordinates of the other end of the wall
} Wall;

// declare an array of Wall structures to represent the walls
// initialize it with the desired wall positions

#define MAX_WALLS 100 // maximum number of walls
Wall walls[MAX_WALLS]; // array of walls
int numWalls = 0; // current number of walls

// -------------------------------------------------------------------------------------------------------------------------

// initialize walls

void initWalls() {

    // Firt Border
    walls[0] = (Wall){ -0.8, -0.8, -0.8, 0.8 };                     // Right
    walls[1] = (Wall){ -0.4, 0.8, 0.4, 0.8 };                       // Up
    walls[2] = (Wall){ 0.8, 0.8, 0.8, -0.8 };                       // Left
    walls[3] = (Wall){ 0.4, -0.8, -0.4, -0.8 };                     // Down

    // Second Border
    walls[4] = (Wall){ -0.0, 0.4, -0.0, 0.8 };                      // Upper Center
    walls[5] = (Wall){ -0.6, 0.0, -0.3, 0.0 };                      // Middle Right
    walls[6] = (Wall){ 0.0, -0.4, 0.0, -0.8 };                      // Lower Center
    walls[7] = (Wall){ 0.6, -0.0, 0.3, -0.0 };                      // Middle Left
    numWalls = 8;
}

// -------------------------------------------------------------------------------------------------------------------------

// Drawing the moving object

float posZ = 0;

void pacman() {
    int i;
    float angle;
    int num_segments = 100;                     // number of segments used to draw the circle
    float radius = 0.05;                         // radius of the circle
    float mouth_angle = 30;                     // angle of the mouth opening in degrees
    #define M_PI 3.14159265358979323846

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0);                           // center of the circle
    for (i = mouth_angle / 2; i <= 360 - mouth_angle / 2; i++) {
        angle = i * M_PI / 180;
        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    }
    glEnd();
}

// -------------------------------------------------------------------------------------------------------------------------

// Define a new structure to represent a ghost

typedef struct {
    float x, y;                                     // position of the ghost
    float speed;                                    // speed of the ghost
    float r, g, b;                                  // color of the ghost
} Ghost;

// declare an array of Ghost structures to represent the ghosts
// initialize it with the desired ghost positions and colors


#define MAX_GHOSTS 4                                // maximum number of ghosts
Ghost ghosts[MAX_GHOSTS];                           // array of ghosts
int numGhosts = 0;                                  // current number of ghosts


// -------------------------------------------------------------------------------------------------------------------------

// initialize ghosts
// Four ghosts with different colors
// their movement speed is 0.25
// Placed across the map with each ghost at a corner of the screen

void initGhosts() {

    ghosts[0] = (Ghost){ -0.75, -0.95, 0.25, 1.0, 0.0, 0.0 };                 // Red Ghost
    ghosts[1] = (Ghost){ -0.75, 0.95, 0.25, 0.0, 0.68, 0.93 };                // Cyan Ghost
    ghosts[2] = (Ghost){ 0.75, -0.95, 0.25, 1.0, 0.75, 0.8 };                 // Pink Ghost
    ghosts[3] = (Ghost){ 0.75, 0.95, 0.25, 1.0, 0.65, 0.0 };                  // Orange Ghost
    numGhosts = MAX_GHOSTS;

}

// -------------------------------------------------------------------------------------------------------------------------


// create a new function to update the positions of the ghosts and move them towards Pac-Man:

void updateGhosts() {

    int i;

    srand(time(0)); // seed the random number generator

    for (i = 0; i < numGhosts; i++) {
        float dx = posX - ghosts[i].x;
        float dy = posY - ghosts[i].y;
        float distance = sqrt(dx * dx + dy * dy);

        // printf("Ghost %d: dx=%f, dy=%f, distance=%f\n", i, dx, dy, distance);        // Debugging Statement

        if (distance > 0) {
            dx /= distance;
            dy /= distance;

            if (rand() % 10 == 0) {                       // Move the ghosts randomly across the screen
                dx = (rand() % 3) - 1;
                dy = (rand() % 3) - 1;
            }

            ghosts[i].x += dx * ghosts[i].speed;
            ghosts[i].y += dy * ghosts[i].speed;
        }

        // check if the ghost has caught Pac-Man
        if (distance < 0.01) {

            // restart Pac-Man to the starting point
            posX = 0.0;
            posY = 0.0;
        }
    }
}

// -------------------------------------------------------------------------------------------------------------------------

// create a new function to draw the ghosts on the screen

void drawGhosts() {
    int i;
    float ghostRadius = 0.05;

    for (i = 0; i < numGhosts; i++) {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(ghosts[i].r, ghosts[i].g, ghosts[i].b);
        glVertex2f(ghosts[i].x, ghosts[i].y);
        int j;

        for (j = 180; j <= 540; j += 10) {
            float angle = j * M_PI / 180;
            glVertex2f(ghosts[i].x + cos(angle) * ghostRadius,
                ghosts[i].y + sin(angle) * ghostRadius);
        }

        glEnd();
    }
}


// -------------------------------------------------------------------------------------------------------------------------

int dotsConsumed = 0;                                   // new variable to keep track of the number of dots consumed

void handleCollisions() {

    // Add collision detection between Pac-Man and the dots

    int i;
    float pacmanRadius = 0.1;
    float dotRadius = 0.01;

    for (i = 0; i < numDots; i++) {
        float dx = dotX[i] - posX;
        float dy = dotY[i] - posY;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance < pacmanRadius + dotRadius) {

            // remove dot

            int j;
            for (j = i; j < numDots - 1; j++) {
                dotX[j] = dotX[j + 1];
                dotY[j] = dotY[j + 1];
                }
                numDots--;                                  // decrement numDots whenever a dot is consumed
                i--;
                dotsConsumed++;                             // increment dotsConsumed whenever a dot is consumed
            
        }
    }

   
}

// -------------------------------------------------------------------------------------------------------------------------

int totalNumberOfDots;                                  // declare totalNumberOfDots
int initialNumberOfDots;                                // declare initialNumberOfDots

void initGame() {

    initialNumberOfDots = 121;                           // initialize initialNumberOfDots
    totalNumberOfDots == initialNumberOfDots;            // initialize totalNumberOfDots
}

// -------------------------------------------------------------------------------------------------------------------------

// Add collision detection between Pac-Man and the Ghosts
// Not needed anymore 
// A detection if statement has been implemented to restart Pac-Man just as he touches one of the ghosts


// -------------------------------------------------------------------------------------------------------------------------

// Adding Dots across the screen

#define MAX_DOTS 100            // maximum number of dots

float dotX[MAX_DOTS];           // x-coordinates of the dots
float dotY[MAX_DOTS];           // y-coordinates of the dots

// int numDots = 0;             // current number of dots                           // It should be defined here  
                                                            // but since it also needs to be defined for the handleCollisions function
                                                            // I defined it the begining of the code before any function is defined

void drawDots() {

    int i;
    float radius = 0.01;                                    // radius of the dots

    for (i = 0; i < numDots; i++) {

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0, 1.0, 1.0);                           // color of the dot
        glVertex2f(dotX[i], dotY[i]);                       // center of the dot

        int j;

        for (j = 0; j <= 360; j += 10) {
            float angle = j * M_PI / 180;
            glVertex2f(dotX[i] + cos(angle) * radius, dotY[i] + sin(angle) * radius);
        }

        glEnd();
    }
}

// -------------------------------------------------------------------------------------------------------------------------

// a function to draw the walls on the screen

void drawWalls() {
    int i;
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_LINES);
    for (i = 0; i < numWalls; i++) {
        glVertex2f(walls[i].x1, walls[i].y1);
        glVertex2f(walls[i].x2, walls[i].y2);
    }
    glEnd();
}

// -------------------------------------------------------------------------------------------------------------------------

// Timer function

void timer(int value) {                                     // responsible for periodically updating the positions of the ghosts
    updateGhosts();                                         // by calling the updateGhosts function every 100 milliseconds
    glutTimerFunc(100, timer, 0);
}

// -------------------------------------------------------------------------------------------------------------------------

// A function to write "GAME OVER" whenever all the dots have been consumed

void displayGameOver() {

    // printf("displayGameOver called\n");                 // Debugging Statement to check if the function is being called

    glPushMatrix();

    glTranslatef(-0.5, 0.0, 0.0);                           // Set position of text
    glColor3f(1.0, 1.0, 1.0);                               // Set color of text

    char* text = "GAME OVER";
    int i;

    for (i = 0; i < strlen(text); i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
         }
    
    glPopMatrix();
}

// -------------------------------------------------------------------------------------------------------------------------


void display() {

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0, 0, 700, 700);                 // set viewport to cover entire window

    initWalls();                                // Initialize the walls before drawing them
    drawWalls();                                // Call drawWalls function to draw walls

    drawDots();                                 // Call drawDots function to draw dots

    initGhosts();                               // Initialize the ghosts before drawing them
    updateGhosts();                             // Update the positions of the ghosts before drawing them on the screen
    drawGhosts();                               // Call Ghosts function to draw ghosts

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    pacman();                                   // Call pacman function to draw polygon
    glPopMatrix();

    handleCollisions();                         // handle collisions between Pac-Man and the dots
    if (dotsConsumed == totalNumberOfDots) {    // check if all dots have been consumed
        displayGameOver();                      // Display "GAME OVER" message if all dots have been consumed
    }

    RenderScene();                              // Call RenderScene function to draw rectangles

    glutSwapBuffers();                          // Update the screen with the new frame

    glFlush();

}


// -------------------------------------------------------------------------------------------------------------------------


void init() {
    // set clear color to black
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // set fill color to white
    glColor3f(1.0, 1.0, 1.0);

    //set up standard orthogonal view with clipping
    //box as cube of side 2 centered at origin
    //This is the default view and these statements could be removed
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

    // add initial dots

    int i, j;
    for (i = -5; i <= 5; i++) {
        for (j = -5; j <= 5; j++) {
            float x = i * 0.3;
            float y = j * 0.3;
            if (numDots < MAX_DOTS) {
                dotX[numDots] = x;
                dotY[numDots] = y;
                numDots++;
            }
        }
    }

}

// -------------------------------------------------------------------------------------------------------------------------

int checkWallCollision(float x1, float y1, float x2, float y2) {

    int i;

    for (i = 0; i < numWalls; i++) {

        float wx1 = walls[i].x1;
        float wy1 = walls[i].y1;
        float wx2 = walls[i].x2;
        float wy2 = walls[i].y2;
        float d = (x1 - x2) * (wy1 - wy2) - (y1 - y2) * (wx1 - wx2);

        if (d != 0) {

            float t = ((x1 - wx1) * (wy1 - wy2) - (y1 - wy1) * (wx1 - wx2)) / d;
            float u = ((x2 - x1) * (y1 - wy1) - (y2 - y1) * (x1 - wx1)) / d;

            if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {

                return 1;
            }
        }
    }

    return 0;
}

// -------------------------------------------------------------------------------------------------------------------------


float move_unit = 0.1f;


void processNormalKeys(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_RIGHT:
        if (!checkWallCollision(posX, posY, posX + move_unit, posY)) {
            posX += move_unit;
        }
        break;

    case GLUT_KEY_LEFT:
        if (!checkWallCollision(posX, posY, posX - move_unit, posY)) {
            posX -= move_unit;
        }
        break;

    case GLUT_KEY_UP:
        if (!checkWallCollision(posX, posY, posX, posY + move_unit)) {
            posY += move_unit;
        }
        break;

    case GLUT_KEY_DOWN:
        if (!checkWallCollision(posX, posY, posX, posY - move_unit)) {
            posY -= move_unit;
        }
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

// -------------------------------------------------------------------------------------------------------------------------

// Main program entry point


int main(int argc, char** argv) {


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Pac-Man");
    glutDisplayFunc(display);
    glutTimerFunc(100, timer, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			// Set background color to black
    init();
    glutSpecialFunc(processNormalKeys);
    initGame(); // call initGame() to initialize the game state
    glutMainLoop();
    return 0;
}

// -------------------------------------------------------------------------------------------------------------------------