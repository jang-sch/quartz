// Jan G 2020-04-10 : this file was not used, item code was distributed into 
// snake.cpp, snake.h, and gameStates.cpp
//file for spawn items definitions/classes 

#pragma once // what does this mean??

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

