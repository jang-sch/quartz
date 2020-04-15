
//TS:2020-04-08 functions relating to rendering gamestates go here

#include <stdlib.h>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <GL/glx.h>

#include "log.h"
#include "fonts.h"
#include "snake.h"

typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];

using namespace std;

// TS:2020-04-08 reads information from g.gameMenu to display 
// main menu function
void renderMenu(Global &g)
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.gameMenu);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2i(0, 0);
    glTexCoord2f(0, 0); glVertex2i(0, g.yres);
    glTexCoord2f(1, 0); glVertex2i(g.xres, g.yres);
    glTexCoord2f(1, 1); glVertex2i(g.xres, 0);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}
//TS: 2020-04-13 reads information from g.gameOverScreen
//to display the end of the game
void renderGameOverScreen(Global &g)
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.gameOverScreen);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2i(0, 0);
    glTexCoord2f(0, 0); glVertex2i(0, g.yres);
    glTexCoord2f(1, 0); glVertex2i(g.xres, g.yres);
    glTexCoord2f(1, 1); glVertex2i(g.xres, 0);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}
//TS: 2020-04-14 reads information from g.creditsScreen
//to display the end of the game
void renderCreditsScreen(Global &g)
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.creditsScreen);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2i(0, 0);
    glTexCoord2f(0, 0); glVertex2i(0, g.yres);
    glTexCoord2f(1, 0); glVertex2i(g.xres, g.yres);
    glTexCoord2f(1, 1); glVertex2i(g.xres, 0);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}
//TS: 2020-04-14 reads information from g.controlsScreen 
//to display the control screen
void renderControlsScreen(Global &g)
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.controlsScreen);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2i(0, 0);
    glTexCoord2f(0, 0); glVertex2i(0, g.yres);
    glTexCoord2f(1, 0); glVertex2i(g.xres, g.yres);
    glTexCoord2f(1, 1); glVertex2i(g.xres, 0);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}


//TS:2020-04-08 fixes jitters when a key is pressed
void rateFixReset()
{
	system("xset r rate");	
}
//TS:2020-04-11 function to decrement the time to show the player how much time is left
int countdown(int& currentTime){
    return(currentTime--);
}
