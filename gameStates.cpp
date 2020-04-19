
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


// JG: 2020-04-15 - setting up needed fx for spawning of transparent item
unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
    //From rainforest.cpp/framework
    //Need to look into using RGBA vs RBG
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}

// JG: 2020-04-15 - function to render the spawned item.
void renderItem(Global &g)
{   
    /* Attempt at making uncolored parts transparent
    From rainforest.cpp/framework
    Need to look into using RGBA vs RBG

    int w = &img[2].width;
	int h = &img[2].height;
    int w = img.width;
	int h = img.height;

    glBindTexture(GL_TEXTURE_2D, g.itemTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(&img[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData);
    */

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.itemTexture);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2i(0, 0);
    glTexCoord2f(0, 0); glVertex2i(0, 15);
    glTexCoord2f(1, 0); glVertex2i(15, 15);
    glTexCoord2f(1, 1); glVertex2i(15, 0);
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
