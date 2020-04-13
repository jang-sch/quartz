//Commenting scheme is " FL: YYYY-MM-DD - blah blah blah comment"
//program: snake.cpp
//author:  Gordon Griesel
//purpose: Simple snake game
//cs335 Spring 2014
//cmps3350 Spring 2018
//
//
//notes:
//This may be used as the starting point for a class project.
//Students should add the following components:
//
//A minor bug was left in the program for demonstration.
//
//These items could be used to generate your requirements phase.
//
// . Introduction page
// . Title page
// . Credits page
// . Main menu (DONE 4/8/2020)
// . Snake texture
// . Rat texture
// . Levels
// . Changes in difficulty/speed
// . Scoring (IN PROGRESS 2020-04-09)
// . Save high scores
// . Board texture (done)
// . Sound
// . Special effects
// . Mouse dragging
// . Win and lose indication
// . Additional features
//
//
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
#include "snake.h"
#define USE_OPENAL_SOUND
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif //USE_OPENAL_SOUND

// JG: 2020-04-06 - changed background image to be our game map.
// TS: 2020-04-08 - Added main menu image
// JG: 2020-04-10 - added test image for item spawn testing
Image img[3] = {
	"./images/Game_Map.png",
	"./images/mainMenu.png",
	"./images/testItem.png"
};

//TS:2020-04-08
//Singleton global instance, global defined in snake.h
Global g;

gameState currentScreen;

//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown = 0.0;
double timeSpan = 0.0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------


class X11_wrapper {
	private:
		Display *dpy;
		Window win;
	public:
		X11_wrapper() {
			GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
			XSetWindowAttributes swa;
			setupScreenRes(g.xres, g.yres);
			dpy = XOpenDisplay(NULL);
			if (dpy == NULL) {
				printf("\n\tcannot connect to X server\n\n");
				exit(EXIT_FAILURE);
			}
			Window root = DefaultRootWindow(dpy);
			XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
			if (vi == NULL) {
				printf("\n\tno appropriate visual found\n\n");
				exit(EXIT_FAILURE);
			} 
			Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
			swa.colormap = cmap;
			swa.event_mask = ExposureMask |
				KeyPressMask |
				KeyReleaseMask |
				ButtonPressMask |
				ButtonReleaseMask |
				PointerMotionMask |
				StructureNotifyMask |
				SubstructureNotifyMask;
			win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,	vi->depth,
					InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
			GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
			glXMakeCurrent(dpy, win, glc);
			setTitle();
		}
		~X11_wrapper() {
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
		}
		void setTitle() {
			//Set the window title bar.
			XMapWindow(dpy, win);
			XStoreName(dpy, win, "Get Off My Lawn");
		}
		void setupScreenRes(const int w, const int h) {
			g.xres = w;
			g.yres = h;
		}
		void reshapeWindow(int width, int height) {
			//window has been resized.
			//Change made by DC:April8th, the background is no longer upsidedown
			// JG: 2020-04-08/09 - background was not fixed here, it was fixed
            // render() function. Background no longer needs to be uploaded
            // upside-down to be projected right-side-up.
			setupScreenRes(width, height);
			//
			glViewport(0, 0, (GLint)width, (GLint)height);
			glMatrixMode(GL_PROJECTION); glLoadIdentity();
			glMatrixMode(GL_MODELVIEW); glLoadIdentity();
			glOrtho(0, g.xres, 0, g.yres, -1, 1);
			setTitle();
		}
		void checkResize(XEvent *e) {
			//The ConfigureNotify is sent by the server if the window is resized.
			if (e->type != ConfigureNotify)
				return;
			XConfigureEvent xce = e->xconfigure;
			if (xce.width != g.xres || xce.height != g.yres) {
				//Window size did change.
				reshapeWindow(xce.width, xce.height);
			}
		}
		bool getXPending() {
			return XPending(dpy);
		}
		XEvent getXNextEvent() {
			XEvent e;
			XNextEvent(dpy, &e);
			return e;
		}
		void swapBuffers() {
			glXSwapBuffers(dpy, win);
		}
} x11;

//function prototypes
void initOpengl(void);
int checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
//TS: 2020-04-08 added endgame function to end the game
//when the quit button is pressed
void endGame();
void initSounds(void);
void physics(void);
void render(void);
void getGridCenter(const int i, const int j, int cent[2]);
#ifdef USE_OPENAL_SOUND
void initSound();
void cleanupSound();
void playSound(ALuint source);
#endif //USE_OPENAL_SOUND

//TS: 2020-04-08 functions for rendering menu
extern void renderMenu(Global &);
extern void rateFixReset();
//TS: 2020-04-11 function for a timer to display on
//screen
extern int countdown(int&);




/*================= MAIN =================*/

int main(int argc, char *argv[])
{
	if (argc) {}
	if (argv[0]) {}
	logOpen();
	initOpengl();
	init();
	initialize_fonts();
	initSound();
	srand((unsigned int)time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	int done = 0;

	// TS: 2020-04-08 Sets the current screen to the main menu
	currentScreen = MENU;
	g.gameSelect = 1;
	
	//Edit the window size here so menu can render properly
	x11.reshapeWindow(1300,1300);
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			done = checkMouse(&e);
			done = checkKeys(&e);
		}
		//
		//Below is a process to apply physics at a consistent rate.
		//1. Get the time right now.
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		//2. How long since we were here last?
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		//3. Save the current time as our new starting time.
		timeCopy(&timeStart, &timeCurrent);
		//4. Add time-span to our countdown amount.
		physicsCountdown += timeSpan;
		//5. Has countdown gone beyond our physics rate? 
		//       if yes,
		//           In a loop...
		//              Apply physics
		//              Reducing countdown by physics-rate.
		//              Break when countdown < physics-rate.
		//       if no,
		//           Apply no physics this frame.
		while(physicsCountdown >= physicsRate) {
			//6. Apply physics
			physics();
			//7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		//Always render every frame.
		render();
		x11.swapBuffers();
	}
	cleanupSound();
	rateFixReset();
	cleanup_fonts();
	logClose();
	return 0;
}

void initSound()
{
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	g.alBufferDrip = alutCreateBufferFromFile("./sounds/drip.wav");
	g.alBufferTick = alutCreateBufferFromFile("./sounds/tick.wav");
	//
	//Source refers to the sound.
	//Generate a source, and store it in a buffer.
	alGenSources(1, &g.alSourceDrip);
	alSourcei(g.alSourceDrip, AL_BUFFER, g.alBufferDrip);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceDrip, AL_GAIN, 1.0f);
	alSourcef(g.alSourceDrip, AL_PITCH, 1.0f);
	alSourcei(g.alSourceDrip, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
	//Generate a source, and store it in a buffer.
	alGenSources(1, &g.alSourceTick);
	alSourcei(g.alSourceTick, AL_BUFFER, g.alBufferTick);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceTick, AL_GAIN, 1.0f);
	alSourcef(g.alSourceTick, AL_PITCH, 1.0f);
	alSourcei(g.alSourceTick, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}
#endif //USE_OPENAL_SOUND
}

void cleanupSound()
{
#ifdef USE_OPENAL_SOUND
	//First delete the source.
	alDeleteSources(1, &g.alSourceDrip);
	alDeleteSources(1, &g.alSourceTick);
	//Delete the buffer.
	alDeleteBuffers(1, &g.alBufferDrip);
	alDeleteBuffers(1, &g.alBufferTick);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
#endif //USE_OPENAL_SOUND
}

void playSound(ALuint source)
{
#ifdef USE_OPENAL_SOUND
	alSourcePlay(source);
#endif //USE_OPENAL_SOUND
}

void initOpengl(void)
{
	//OpenGL initialization
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	//
	//choose one of these
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	glEnable(GL_TEXTURE_2D);
	//map_texture = loadBMP("....?.bmp");
	glBindTexture(GL_TEXTURE_2D, 0);
	//
	//load the image file into a ppm structure.
	//
	//g.mapImage = ppm6GetImage("./images/map.ppm");
	g.mapImage = &img[0];
	//
	//create opengl texture elements
	glGenTextures(1, &g.mapTexture);
	//openGL texture for main menu
	glGenTextures(1, &g.gameMenu);

	glBindTexture(GL_TEXTURE_2D, g.mapTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
			g.mapImage->width, g.mapImage->height,
			0, GL_RGB, GL_UNSIGNED_BYTE, g.mapImage->data);

	int menuWidth = img[1].width;
	int menuHeight = img[1].height;  

	/*g.tex.xc[0] = 0.0;
	  g.tex.xc[1] = 1.0;
	  g.tex.yc[0] = 0.0;
	  g.tex.yc[1] = 1.0;
	 */

	//TS: 2020-04-08 Load menu
	//save data into g.gameMenu for renderMenu() function
	glBindTexture(GL_TEXTURE_2D, g.gameMenu);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, menuWidth, menuHeight, 0,
			GL_RGB, GL_UNSIGNED_BYTE, img[1].data);

}

// JG: 2020-04-07 - changed starting position of snake
void initSnake()
{
	int i;
	g.snake.status = 1;
	g.snake.delay = .15;
	g.snake.length = rand() % 4 + 3;
	for (i=0; i<g.snake.length; i++) {
		g.snake.pos[i][0] = 1;
		g.snake.pos[i][1] = 10;
	}
	g.snake.direction = DIRECTION_RIGHT;
	//snake.timer = glfwGetTime() + 0.5;
}
// JG: 2020-04-07 - changed starting position of rat
void initRat()
{
	g.rat.status = 1;
	g.rat.pos[0] = 25;
	g.rat.pos[1] = 10;
}

void init()
{
	// JG: why * 10?
	g.boardDim = g.gridDim * 10; 
	//
	initSnake();
	initRat();
	//
	//initialize buttons...
	g.nbuttons=0;
	//size and position
	//TS: 2020-04-08 - changed height, width, and location of the buttons
	//WILL CHANGE MORE LATER!
	//g.button[g.nbuttons].r.width = 100; - original
	//g.button[g.nbuttons].r.height = 60; - original
	g.button[g.nbuttons].r.width = 65;
	g.button[g.nbuttons].r.height = 25;
	//g.button[g.nbuttons].r.left = 10; - original
	//g.button[g.nbuttons].r.bot = 320; - original
	g.button[g.nbuttons].r.left = 575;
	g.button[g.nbuttons].r.bot = 25;
	g.button[g.nbuttons].r.right =
		g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
	g.button[g.nbuttons].r.top =
		g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.height;
	g.button[g.nbuttons].r.centerx =
		(g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.right) / 2;
	g.button[g.nbuttons].r.centery =
		(g.button[g.nbuttons].r.bot + g.button[g.nbuttons].r.top) / 2;
	strcpy(g.button[g.nbuttons].text, "Reset");
	g.button[g.nbuttons].down = 0;
	g.button[g.nbuttons].click = 0;
	//g.button[g.nbuttons].color[0] = 0.4f; - original
	g.button[g.nbuttons].color[0] = 0.3f; 
	//g.button[g.nbuttons].color[1] = 0.4f; - original
	g.button[g.nbuttons].color[1] = 0.3f;
	//g.button[g.nbuttons].color[2] = 0.7f; - original
	g.button[g.nbuttons].color[2] = 0.6f;
	g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
	g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
	g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
	g.button[g.nbuttons].text_color = 0x00ffffff;
	g.nbuttons++;
	//g.button[g.nbuttons].r.width = 100; - original
	//g.button[g.nbuttons].r.height = 60; - original
	g.button[g.nbuttons].r.width = 65;
	g.button[g.nbuttons].r.height = 25;
	//g.button[g.nbuttons].r.left = 10; - original 
	//g.button[g.nbuttons].r.bot = 160; - original
	g.button[g.nbuttons].r.left = 725;
	g.button[g.nbuttons].r.bot = 25;
	g.button[g.nbuttons].r.right =
		g.button[g.nbuttons].r.left + g.button[g.nbuttons].r.width;
	g.button[g.nbuttons].r.top = g.button[g.nbuttons].r.bot +
		g.button[g.nbuttons].r.height;
	g.button[g.nbuttons].r.centerx = (g.button[g.nbuttons].r.left +
			g.button[g.nbuttons].r.right) / 2;
	g.button[g.nbuttons].r.centery = (g.button[g.nbuttons].r.bot +
			g.button[g.nbuttons].r.top) / 2;
	strcpy(g.button[g.nbuttons].text, "Quit");
	g.button[g.nbuttons].down = 0;
	g.button[g.nbuttons].click = 0;
	g.button[g.nbuttons].color[0] = 0.3f;
	g.button[g.nbuttons].color[1] = 0.3f;
	g.button[g.nbuttons].color[2] = 0.6f;
	g.button[g.nbuttons].dcolor[0] = g.button[g.nbuttons].color[0] * 0.5f;
	g.button[g.nbuttons].dcolor[1] = g.button[g.nbuttons].color[1] * 0.5f;
	g.button[g.nbuttons].dcolor[2] = g.button[g.nbuttons].color[2] * 0.5f;
	g.button[g.nbuttons].text_color = 0x00ffffff;
	g.nbuttons++;
}

void resetGame()
{
	initSnake();
	initRat();
	g.gameover  = 0;
	g.winner    = 0;
	//TS:2020-04-11 - when reseting the game the time restarts
	//(at a fixed time our group decides on)
	g.timeRemaining = 1000;
}

int checkKeys(XEvent *e)
{
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	(void)shift;
	switch (key) {
		case XK_r:
			resetGame();
			break;
		case XK_equal:
			g.snake.delay *= 0.9;
			if (g.snake.delay < 0.001)
				g.snake.delay = 0.001;
			break;
		case XK_minus:
			g.snake.delay *= (1.0 / 0.9);
			break;
		case XK_Left:
			g.snake.direction = DIRECTION_LEFT;
			break;
		case XK_Right:
			g.snake.direction = DIRECTION_RIGHT;
			break;
		case XK_Up:
			g.snake.direction = DIRECTION_UP;
			break;
		case XK_Down:
			g.snake.direction = DIRECTION_DOWN;
			break;
		/* JG: 08-04-09 - this is a quickfix for our issue where the game still
		starts in background during menu screen. To help during testing of item
		spawns while a more proper fix is figured out. desp:changing soundtrack

		case XK_Return:
			if(currentScreen == MENU) {
			currentScreen = GAME;
			resetGame();
			}
		*/
		//TS:2020-04-08 to let the player press enter to 
		//go to the game screen from the main menu; fixed
		//the code in the physics() function so the game 
		//no longer starts when the menu is showing.
		case XK_Return:
			if(currentScreen == MENU)
				currentScreen = GAME;
			break;
		case XK_Escape:
			return 1;		
	}
	return 0;
}

int checkMouse(XEvent *e)
{
	static int savex = 0;
	static int savey = 0;
	int i,x,y;
	int lbutton=0;
	int rbutton=0;
	//
	if (e->type == ButtonRelease)
		return 0;
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
			rbutton=1;
			if (rbutton){}
		}
	}
	x = e->xbutton.x;
	y = e->xbutton.y;
	y = g.yres - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	for (i=0; i<g.nbuttons; i++) {
		g.button[i].over=0;
		if (x >= g.button[i].r.left &&
				x <= g.button[i].r.right &&
				y >= g.button[i].r.bot &&
				y <= g.button[i].r.top) {
			g.button[i].over=1;
			if (g.button[i].over) {
				if (lbutton) {
					switch (i) {
						case 0:
							resetGame();
							break;
						case 1:
					//TS: 2020-04-08 - the game will end when button is pushed
					//instead of saying the button was pushed
							endGame();
							//printf("Quit was clicked!\n");
							return 1;
					}
				}
			}
		}
	}
	return 0;
}

void getGridCenter(const int i, const int j, int cent[2])
{
	//This function can be optimized, and made more generic.
	int b2 = g.boardDim/2;
	int screenCenter[2] = {g.xres/2, g.yres/2};
	int s0 = screenCenter[0];
	int s1 = screenCenter[1];
	int bq;
	//quad upper-left corner
	int quad[2];
	//bq is the width of one grid section
	bq = (g.boardDim / g.gridDim);
	//-------------------------------------
	//because y dimension is bottom-to-top in OpenGL.
	int i1 = g.gridDim - i - 1;
	quad[0] = s0-b2;
	quad[1] = s1-b2;
	cent[0] = quad[0] + bq/2;
	cent[1] = quad[1] + bq/2;
	cent[0] += (bq * j);
	cent[1] += (bq * i1);
}

//TS: 2020-04-08 added an endGame function so the player
//can quit the game when button is pushed
void endGame()
{
    exit(EXIT_FAILURE);
}
void physics(void)
{
	int i;
	if (g.gameover)
		return;
	//
	//
	//TS: 2020-04-11 the fix where the game would start at the menu
	//Is it time to move the snake?
	static struct timespec snakeTime;
	//static int firsttime=1; - original code
	//if(firsttime){ - original code
	if (currentScreen == MENU) {
		//firsttime=0; - original code
		clock_gettime(CLOCK_REALTIME, &snakeTime);
	}
	struct timespec tt;
	clock_gettime(CLOCK_REALTIME, &tt);
	double timeSpan = timeDiff(&snakeTime, &tt);
	if (timeSpan < g.snake.delay)
		return;
	timeCopy(&snakeTime, &tt);
	//
	playSound(g.alSourceDrip);
	//move the snake segments...
	int headpos[2];
	int newpos[2];
	int oldpos[2];
	//save the head position.
	headpos[0] = g.snake.pos[0][0];
	headpos[1] = g.snake.pos[0][1];
	//snake.direction:
	//0=down
	//1=left
	//2=up
	//3=right
	switch (g.snake.direction) {
		case DIRECTION_DOWN:  g.snake.pos[0][1] += 1; break;
		case DIRECTION_LEFT:  g.snake.pos[0][0] -= 1; break;
		case DIRECTION_UP:    g.snake.pos[0][1] -= 1; break;
		case DIRECTION_RIGHT: g.snake.pos[0][0] += 1; break;
	}
	//check for snake off board...
	if (g.snake.pos[0][0] < 0 ||
			g.snake.pos[0][0] > g.gridDim-1 ||
			g.snake.pos[0][1] < 0 ||
			g.snake.pos[0][1] > g.gridDim-1) {
		g.gameover=1;
		return;
	}

	// JG: 2020-04-06 - removed game over when snake crosses itself. Will 
	// revisit this later to see if can be a part of game elsewhere.
	//check for snake crossing itself...
	/*for (-i=1; i<g.snake.length; i++) {
	  if (g.snake.pos[i][0] == g.snake.pos[0][0] &&
	  g.snake.pos[i][1] == g.snake.pos[0][1]) {
	  g.gameover=1;
	  return;
	  }
	  }*/

	//
	newpos[0] = headpos[0];
	newpos[1] = headpos[1];
	for (i=1; i<g.snake.length; i++) {
		oldpos[0] = g.snake.pos[i][0];
		oldpos[1] = g.snake.pos[i][1];
		if (g.snake.pos[i][0] == newpos[0] &&
				g.snake.pos[i][1] == newpos[1])
			break;
		g.snake.pos[i][0] = newpos[0];
		g.snake.pos[i][1] = newpos[1];
		newpos[0] = oldpos[0];
		newpos[1] = oldpos[1];
	}
	//did the snake eat the rat???
	if (headpos[0] == g.rat.pos[0] && headpos[1] == g.rat.pos[1]) {
		//yes, increase length of snake.
		playSound(g.alSourceTick);
		//put new segment at end of snake.
		Log("snake ate rat. snake.length: %i   dir: %i\n",
				g.snake.length,g.snake.direction);

		// JG: 2020-04-07 - when treasure collected, it follows you now (?)
		// changed it to add length by 2 to accomodate for space. Also
		// set up PSEUDOCODE for item scoring based on rarity of item.
		int addlength = 2;

		/* JG: 2020-04-07 - pseudocode
		   if (itemRarity == 1)
		   totalScore += common
		   else if (itemRarity == 2)
		   totalScore += uncommon
		   else if (itemRarity == 3)
		   totalScore += rare
		   else if (itemRarity == 4)
		   totalScore += epic
		   else if (itemRarity == 5)
		   totalScore += legendray
		   else
		   thers some sort of error, consider what happens when snake
		   eats itself.
		 */


		//TS: 2020-04-07 added a counter to count when
		//the snake collects the item
		// JG: 2020-04-07 - worked with TS to make collected item counter 
		// count correctly.
		g.collCount++;
		for (i=0; i<addlength; i++) {
			g.snake.pos[g.snake.length][0] = g.snake.pos[g.snake.length-1][0];
			g.snake.pos[g.snake.length][1] = g.snake.pos[g.snake.length-1][1];
			g.snake.length++;
		}
		//new position for rat...
		int collision=0;
		int ntries=0;
		while (1) {
			g.rat.pos[0] = rand() % g.gridDim;
			g.rat.pos[1] = rand() % g.gridDim;
			collision=0;
			for (i=0; i<g.snake.length; i++) {
				if (g.rat.pos[0] == g.snake.pos[i][0] &&
						g.rat.pos[1] == g.snake.pos[i][1]) {
					collision=1;
					break;
				}
			}
			if (!collision) break;
			if (++ntries > 1000000) break;
		}
		Log("new rat: %i %i\n",g.rat.pos[0],g.rat.pos[1]);
		return;
	}
}

void render(void)
{
	int i,j;
	Rect r;
        //TS: 2020-04-11 - to make sure the countdown works correctly 
        //and does not go into the negatives
	//TS:2020-04-12 - added a couple of lines of code to change the 
	//display from frames per second to just seconds
         if(g.framesRemaining > 0){
           countdown(g.framesRemaining);
	   if(g.framesRemaining%60 == 0)
	       g.timeRemaining = g.framesRemaining/60;
	 }

        //TS:2020-04-08 - load to check the gamestate
	//IF loop when the currentScreen is menu
	//then it will render the menu
	if (currentScreen == MENU){
		rateFixReset();
		renderMenu(g);
	}
	//ELSE then the game will the gaming screen
	else if (currentScreen == GAME) {

		//--------------------------------------------------------
		//This code is repeated several times in this program, so
		//it can be made more generic and cleaner with some work.
		int b2 = g.boardDim/2;
		int s0 = g.xres>>1;
		int s1 = g.yres>>1;
		//center of a grid
		int cent[2];
		//bq is the width of one grid section
		//--------------------------------------------------------
		//start the opengl stuff
		//set the viewing area on screen
		glViewport(0, 0, g.xres, g.yres);
		//clear color buffer
		glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//init matrices
		glMatrixMode (GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		//this sets to 2D mode (no perspective)
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		//
		//screen background
		// JG: 2020-04-07 - modified values in glColor3f() to remove dim effect
		glColor3f(1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, g.mapTexture);
		glBegin(GL_QUADS);
		// JG: 2020-04-08 - fixed upside-down projection of background 
		// JG: 2020-04-09 - fixed projection of background 2: electric boogaloo
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0,      0);        // prev: 0000 
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0,      g.yres);   // prev: 010y
		glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);   // prev: 11xy
		glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);        // prev: 10x0
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0); // WAHT DOES THIS MEAN?
		//
		//draw all buttons
		for (i=0; i<g.nbuttons; i++) {
			if (g.button[i].over) {
				int w=2;
				//TS: 2020-04-08 - commented out the yellow highlight
				//to have a whiteish highlight around button
				//glColor3f(1.0f, 1.0f, 0.0f);
				//draw a highlight around button
				glLineWidth(3);
				glBegin(GL_LINE_LOOP);
				glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
				glVertex2i(g.button[i].r.left-w,  g.button[i].r.top+w);
				glVertex2i(g.button[i].r.right+w, g.button[i].r.top+w);
				glVertex2i(g.button[i].r.right+w, g.button[i].r.bot-w);
				glVertex2i(g.button[i].r.left-w,  g.button[i].r.bot-w);
				glEnd();
				glLineWidth(1);
			}
			if (g.button[i].down) {
				glColor3fv(g.button[i].dcolor);
			} else {
				glColor3fv(g.button[i].color);
			}
			glBegin(GL_QUADS);
			glVertex2i(g.button[i].r.left,  g.button[i].r.bot);
			glVertex2i(g.button[i].r.left,  g.button[i].r.top);
			glVertex2i(g.button[i].r.right, g.button[i].r.top);
			glVertex2i(g.button[i].r.right, g.button[i].r.bot);
			glEnd();
			r.left = g.button[i].r.centerx;
			r.bot  = g.button[i].r.centery-8;
			r.center = 1;
			if (g.button[i].down) {
				ggprint16(&r, 0, g.button[i].text_color, "Pressed!");
			} else {
				ggprint16(&r, 0, g.button[i].text_color, g.button[i].text);
			}
		}
		//draw the main game board in middle of screen

		// JG: 2020-04-07 - removed board that was occluding background
		
		   //glColor3f(0.6f, 0.5f, 0.2f);
		   //glBegin(GL_QUADS);
		   //glVertex2i(s0-b2, s1-b2);
		   //glVertex2i(s0-b2, s1+b2);
		   //glVertex2i(s0+b2, s1+b2);
		   //glVertex2i(s0+b2, s1-b2);
		   //glEnd();
		 

		//
		//grid lines...
		int x0 = s0-b2;
		int x1 = s0+b2;
		int y0 = s1-b2;
		int y1 = s1+b2;
		// JG: 2020-04-07 - changed gridline color, to remove by end but the lines
		// are being kept for the moment to help with positioning/object spawns.
		// Quartz color is glColor3f(0.85f, 0.85f, 0.95f);
		//glColor3f(0.658824f, 0.658824f, 0.658824f);
		glColor3f(0.13f, 0.37f, 0.31f);
		glBegin(GL_LINES);
		for (i=1; i<g.gridDim; i++) {
			y0 += 10;
			glVertex2i(x0,y0);
			glVertex2i(x1,y0);
		}
		x0 = s0-b2;
		y0 = s1-b2;
		y1 = s1+b2;
		for (j=1; j<g.gridDim; j++) {
			x0 += 10;
			glVertex2i(x0,y0);
			glVertex2i(x0,y1);
		}
		glEnd();
		//
#define COLORFUL_SNAKE
		//
		//draw snake...
#ifdef COLORFUL_SNAKE
		float c[3]={1.0f,1.0,0.5};
		float rgb[3];
		rgb[0] = -0.9 / (float)g.snake.length;
		rgb[2] = -0.45 / (float)g.snake.length;
		glColor3fv(c);
		//
		glBegin(GL_QUADS);
		for (i=0; i<g.snake.length; i++) {
			getGridCenter(g.snake.pos[i][1],g.snake.pos[i][0],cent);
			glVertex2i(cent[0]-4, cent[1]-3);
			glVertex2i(cent[0]-4, cent[1]+4);
			glVertex2i(cent[0]+3, cent[1]+4);
			glVertex2i(cent[0]+3, cent[1]-3);
			c[0] +=	rgb[0];
			c[2] +=	rgb[2];
			glColor3fv(c);
		}
		glEnd();
#else //COLORFUL_SNAKE
		glColor3f(0.1f, 0.8f, 0.1f);
		glBegin(GL_QUADS);
		for (i=0; i<g.snake.length; i++) {
			getGridCenter(g.snake.pos[i][1],g.snake.pos[i][0],cent);
			glVertex2i(cent[0]-4, cent[1]-3);
			glVertex2i(cent[0]-4, cent[1]+4);
			glVertex2i(cent[0]+3, cent[1]+4);
			glVertex2i(cent[0]+3, cent[1]-3);
			glColor3f(0.0f, 0.6f, 0.0f);
		}
		glEnd();
#endif //COLORFUL_SNAKE
		//
		//
		//draw rat...
		getGridCenter(g.rat.pos[1],g.rat.pos[0],cent);
		// JG: 2020-04-07/10 - rat colors to be treasures, for testing purposes
		glColor3f(1.0, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex2i(cent[0]-4, cent[1]-3);
		glVertex2i(cent[0]-4, cent[1]+4);
		glVertex2i(cent[0]+3, cent[1]+4);
		glVertex2i(cent[0]+3, cent[1]-3);
		glEnd();
		//
		//
		// JAN

		



		// END JAN
		
		
		//r.left   = g.xres/2;
		//r.bot    = g.yres-100;
		//r.center = 1;
		//TS: 2020-04-08 - changed the title of game and allowed to show
		//user the number of items collected, and the number of points
		// JG: 2020-04-07 - added scorekeeping, depends on item rarity; font color
		r.bot = g.yres - 20;
		r.left = 10;
		r.center = 0;
		ggprint8b(&r, 16, 0x00006600, "Get Off My Lawn");
                //TS:2020-04-11 added to show the remaining time for the player 
                //to collect the items
                ggprint8b(&r, 16, 0x00006600, "Time left: %i seconds", g.timeRemaining);
		ggprint8b(&r, 16, 0x00006600, "Number of items collected: %i", g.collCount);
		ggprint8b(&r, 16, 0x00006600, "Points: %i", g.totalScore, ":");
                ggprint8b(&r, 16, 0x00006600, "Quit: Esc");

		
	
	}
}




