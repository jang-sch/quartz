//Functions relating to rendering gamestates go here

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

void rateFix()
{
	system("xset r rate 1 40");
}
void rateFixReset()
{
	system("xset r rate");	
}
