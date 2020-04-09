#pragma once

//Header file for global and other stuff pertaining to the game
//Placed here for ease of customization
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
//#include "ppm.h"
#include "fonts.h"
#define USE_OPENAL_SOUND
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

//macros
#define rnd() (double)rand()/(double)RAND_MAX

#define DIRECTION_DOWN  0
#define DIRECTION_LEFT  1
#define DIRECTION_UP    2
#define DIRECTION_RIGHT 3
//
#define MAX_GRID 80
typedef struct t_grid {
	int status;
	float color[4];
} Grid;
//
typedef struct t_snake {
	int status;
	int length;
	int pos[MAX_GRID*MAX_GRID][2];
	int direction;
	double timer;
	double delay;
} Snake;
//
typedef struct t_rat {
	int status;
	int pos[2];
} Rat;
//
//
//
//
#define MAXBUTTONS 4
typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;

//enumerated type for determining what state 
//the game is in (menu, ingame, etc..)
//used for main menu
enum gameState {
	MENU = 1,
	GAME
};




class Image {
public:
	int width, height;
	unsigned char *data;
	// This is a destructor JG
    ~Image() { delete [] data; }
    // This is a constructor JG
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert img.jpg img.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};



struct Global {
	int xres, yres;
	Grid grid[MAX_GRID][MAX_GRID];
	Snake snake;
	Rat rat;
	int gridDim;
	
	GLuint gameMenu;
	
	int boardDim;
	int gameover;
	int winner;
	int gameSelect;
	//TS: added a count function for the items collected
    //JG: 2020-04-07 - changed variable name for clarity
	int collCount = 0;
    //JG: 2020-04-07 - added variableS to calculate and keep score
    const int COMMON_I = 5;
	const int UNCOMMON_I = 10;
	const int RARE_I = 20;
	const int EPIC_I = 40;
	const int LEGENDARY_I = 80;
	int totalScore = 0;
	//
	Image *marbleImage;
	GLuint marbleTexture;
	Button button[MAXBUTTONS];
	int nbuttons;
	//
	ALuint alBufferDrip, alBufferTick;
	ALuint alSourceDrip, alSourceTick;
	Global() {
		// JG: 2020-04-07 - changed window start size
        xres = 900;
		yres = 900;
        // gridDim aka grid dimension
		// JG: 2020-04-07 - adjusted size of grid to be playable area 
        gridDim = 72;
		gameover = 0;
		winner = 0;
		nbuttons = 0;
		marbleImage=NULL;
		gameSelect = 1;
	}
};



//-----------------------------------------------------------------------------

