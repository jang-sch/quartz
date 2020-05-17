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
// . Rat texture --> working on it; aka item texture
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
// TS: 2020-04-12 - added a testing gameOver image for when the
// game runs out of time
// TS: 2020-04-13 - added a testing credits image for the credits
// page
// TS:2020-04-13 - added a testing controls image for the controls
// page
// DC: 2020-04-16 - updated controls screen
// DC: 2020-04-30 - Added the update main menu and game over screen 
// DC: 2020-05-07 - Added the credits screen to the credit function
Image img[6] = {
	"./images/Game_Map.png",
	"./images/GameMainMenu.png",
	"./images/testItem.png",
	"./images/GameOverF.png",
	"./images/Game_Credits.png" ,
	"./images/Game_Controls.png"
};

// JG: 2020-05-16 - 2D array that contains all valid road spawn points.
int vSpawn[][15] =  {{3,8},{4,8},{5,8},{6,8},{7,8},{8,8},{9,8},{10,8},
                    {11,8},{12,8},{13,8},{14,8},{15,8},{16,8},{17,8},
					
					{17,5},{18,5},{19,5},{20,5},{21,5},
                    {22,5},{23,5},{24,5},{25,5},{26,5},
                    {27,5},{28,5},{29,5},{30,5},{31,5},
                    {32,5},{33,5},{34,5},{35,5},{36,5},

                    {36,10},{37,10},{38,10},{39,10},{40,10},{41,10},
                    {42,10},{43,10},{44,10},{45,10},{46,10},{47,10},
                    {48,10},{49,10},{50,10},{51,10},{52,10},{53,10},
                    {54,10},{55,10},{56,10},{57,10},

                    {17,13},{18,13},{19,13},{20,13},{21,13},

                    {21,17},{22,17},{23,17},{24,17},

                    {3,24},{4,24},{5,24},{6,24},{7,24},{8,24},
                    {9,24},{10,24},{11,24},{12,24},{13,24},{14,24},
                    {15,24},{16,24},{17,24},{18,24},{19,24},{20,24},
                    {21,24},{22,24},{23,24},{24,24},

                    {3,36},{4,36},{5,36},{6,36},{7,36},{8,36},{9,36},

                    {9,42},{10,42},{11,42},{12,42},{13,42},
                    {14,42},{15,42},{16,42},{17,42},{18,42},

                    {3,45},{4,45},{5,45},{6,45},{7,45},
                    {8,45},{9,45},{10,45},{11,45},{12,45},

                    {3,54},{4,54},{5,54},{6,54},{7,54},{8,54},
                    {9,54},{10,54},{11,54},{12,54},{13,54},{14,54},
                    {15,54},{16,54},{17,54},{18,54},{19,54},{20,54},
                    {21,54},{22,54},{23,54},{24,54},{25,54},{26,54},
                    {27,54},{28,54},{29,54},{30,54},

                    {24,20},{25,20},{26,20},{27,20},{28,20},
                    {29,20},{30,20},{31,20},{32,20},{33,20},
                    {34,20},{35,20},{36,20},

                    {36,24},{37,24},{38,24},{39,24},{40,24},{41,24},
                    {42,24},{43,24},{44,24},{45,24},{46,24},

                    {46,26},{47,26},{48,26},{49,26},{50,26},{51,26},
                    {52,26},{53,26},{54,26},{55,26},{56,26},{57,26},

                    {30,34},{31,34},{32,34},{33,34},{34,34},
                    {35,34},{36,34},{37,34},{38,34},{39,34},
                    {40,34},{41,34},{42,34},{43,34},{44,34},

                    {44,40},{45,40},{46,40},{47,40},{48,40},
                    {49,40},{50,40},{51,40},{52,40},{53,40},
                    {54,40},{55,40},{56,40},{57,40},

                    {30,51},{31,51},{32,51},{33,51},{34,51},{35,51},
                    {36,51},{37,51},{38,51},{39,51},{40,51},{41,51},
                    {42,51},{43,51},{44,51},{45,51},{46,51},{47,51},
                    {48,51},{49,51},{50,51},{51,51},{52,51},{53,51},
                    {54,51},{55,51},{56,51},{57,51},

                    {3,9},{3,10},{3,11},{3,12},{3,13},{3,14},
                    {3,15},{3,16},{3,17},{3,18},{3,19},{3,20},
                    {3,21},{3,22},{3,23},{3,25},{3,26},
                    {3,27},{3,28},{3,29},{3,30},{3,31},{3,32},
                    {3,33},{3,34},{3,35},

                    {3,46},{3,47},{3,48},{3,49},
                    {3,50},{3,51},{3,52},{3,53},

                    {9,37},{9,38},{9,39},{9,40},
                    {9,41},{9,43},{9,44},

                    {18,25},{18,26},{18,27},{18,28},{18,29},{18,30},
                    {18,31},{18,32},{18,33},{18,34},{18,35},{18,36},
                    {18,37},{18,38},{18,39},{18,40},{18,41},

                    {17,6},{17,7},{17,9},{17,10},{17,11},{17,12},

                    {21,14},{21,15},{21,16},

                    {24,18},{24,19},{24,21},{24,22},{24,23},

                    {30,21},{30,22},{30,23},{30,24},{30,25},
                    {30,26},{30,27},{30,28},{30,29},{30,30},
                    {30,31},{30,32},{30,33},{30,35},{30,36},
                    {30,37},{30,38},{30,39},{30,40},{30,41},
                    {30,42},{30,43},{30,44},{30,45},{30,46},
                    {30,47},{30,48},{30,49},{30,50},{30,52},
                    {30,53},

                    {36,6},{36,7},{36,8},{36,9}, 
                    {36,11},{36,12},{36,13},{36,14},
                    {36,15},{36,16},{36,17},{36,18},
                    {36,19},{36,21},{36,22},{36,23},

                    {44,35},{44,36},{44,37},{44,38},{44,39},
                    {44,41},{44,42},{44,43},{44,44},{44,45},
                    {44,46},{44,47},{44,48},{44,49},{44,50},

                    {46,25},

                    {57,11},{57,12},{57,13},{57,14},{57,15},{57,16},
                    {57,17},{57,18},{57,19},{57,20},{57,21},{57,22},
                    {57,23},{57,24},{57,25},{57,27},{57,28},{57,29},
                    {57,30},{57,31},{57,32},{57,33},{57,34},{57,35},
                    {57,36},{57,37},{57,38},{57,39},{57,41},{57,42},
                    {57,43},{57,44},{57,45},{57,46},{57,47},{57,48},
                    {57,49},{57,50},

                    {10,43},{11,43},{12,43},{10,44},{11,44},{12,44}

					};
// JG: 2020-05-16 - to get length of vSpawn array for later indexing.
int vSpwnLen = sizeof(vSpawn)/sizeof(vSpawn[0]);

//TS:2020-04-08 - Singleton global instance, global defined in snake.h
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
//JG: 2020-04-15 - function prototype for render item
extern void renderItem(Global &);
extern void rateFixReset();
//TS: 2020-04-11 function for a timer to display on
//screen
extern int countdown(int&);
//TS: 2020-04-13 - function to render the game over screen
extern void renderGameOverScreen(Global &);
//TS: 2020-04-14 function to render the credit screen
extern void renderCreditsScreen(Global &);
//TS:2020-04-14 function to render the controls screen
extern void renderControlsScreen(Global &);

/*================ MAIN =================*/

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
	// JG: 2020-05-12 playing with sound
	playSound(g.alSourceSong);
	
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
	// JG: 2020-05-12 - adding sound information for custom song
	g.alBufferSong = alutCreateBufferFromFile("./sounds/wild-goose-chase.wav");
	g.alBufferDrip = alutCreateBufferFromFile("./sounds/drip.wav");
	g.alBufferTick = alutCreateBufferFromFile("./sounds/tick.wav");
	//
	//Source refers to the sound.----------------------
	
	// JG: 2020-05-12 - Generate a source, and store it in a buffer , for song.
	alGenSources(1, &g.alSourceSong);
	alSourcei(g.alSourceSong, AL_BUFFER, g.alBufferSong);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(g.alSourceSong, AL_GAIN, 1.0f);
	alSourcef(g.alSourceSong, AL_PITCH, 1.0f);
	alSourcei(g.alSourceSong, AL_LOOPING, AL_TRUE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return;
	}

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
	alDeleteSources(1, &g.alSourceSong);
	//Delete the buffer.
	alDeleteBuffers(1, &g.alBufferDrip);
	alDeleteBuffers(1, &g.alBufferTick);
	alDeleteBuffers(1, &g.alBufferSong);
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
	//TS:2020-04-08 openGL texture for main menu
	glGenTextures(1, &g.gameMenu);
	//TS:2020-04-11 openGL texture for the gameOver Screen
        glGenTextures(1, &g.gameOverScreen);
        //TS: 2020-04-14 openGL texture for the credits Screen
	glGenTextures(1, &g.creditsScreen);
	//TS: 2020-04-14 openGL texture for the controls page
	glGenTextures(1, &g.controlsScreen);
	//JG: 2020-04-15 - openGL texture for spawn item
	glGenTextures(1, &g.itemTexture);

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
        //TS: 2020-04-13 Load endgame
	//save data into g.gameOverScreen for rendergameOverScreen() function 
	glBindTexture(GL_TEXTURE_2D, g.gameOverScreen);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img[3].width, img[3].height, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, img[3].data);
        //TS: 2020-04-14 Load credits
	//save data into g.creditsScreen for renderCreditsScreen() function
        glBindTexture(GL_TEXTURE_2D, g.creditsScreen);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img[4].width, img[4].height, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, img[4].data);
        //TS: 2020-04-14 Load controls
	//save data into g.controlsScreen for renderControlsScreen() function
        glBindTexture(GL_TEXTURE_2D, g.controlsScreen);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img[5].width, img[5].height, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, img[5].data);

		//JG: 2020-04-15 - load spawn item texture and save data 
		glBindTexture(GL_TEXTURE_2D, g.itemTexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, img[2].width, img[2].height, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, img[2].data);
}

// JG: 2020-04-07 - changed starting position of snake
// NL: 2020-04-10 - made snake position a little lower so it is on the road
// NL: 2020-04-15 - changed position to fit the modified map & grid
void initSnake()
{
	int i;
	g.snake.status = 1;
	g.snake.delay = .15;
	g.snake.length = rand() % 4 + 3;
	for (i=0; i<g.snake.length; i++) {
		g.snake.pos[i][0] = 1;
		g.snake.pos[i][1] = 8;
	}
	g.snake.direction = DIRECTION_RIGHT;
	//snake.timer = glfwGetTime() + 0.5;
}
// JG: 2020-04-07 - changed starting position of rat
void initRat()
{
	g.rat.status = 1;
	g.rat.pos[0] = 20;
	g.rat.pos[1] = 5;
}

// JG: 2020-04-27 - similar to original "rat"
void initItem()
{
	g.item.status = 1;
	g.item.pos[0] = 15;
	g.item.pos[1] = 8;
}

void init()
{
	// JG: why * 10?
	g.boardDim = g.gridDim * 10; 
	//
	initSnake();
	initRat();
	initItem(); // JG: 2020-04-27
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
	g.button[g.nbuttons].r.left = 410;
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
	g.button[g.nbuttons].r.left = 580;
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
	// JG: 2020-05-16 to initialize item to be collected, reset item counter.
	initItem();
	g.collCount = 0;
	g.gameover  = 0;
	g.winner    = 0;
	//TS:2020-04-11 - when reseting the game the time restarts
	//(at a fixed time our group decides on)
	// JG: 2020-05-16 - adjusted to have 33 seconds of time to 
	// the game upon reset. To adjust time upon start of game
	// need to change variable "timeRemaining" in snake.h
	g.framesRemaining = 2000;
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
               //TS: 2020-04-14 the user will press c from the main menu
	       //and it will go to the credits page and if you push c again
	       //in the credits screen then it will go back to the 
	       //main menu
		case XK_c:
			if(currentScreen == MENU)
			currentScreen = CREDITS;
			else if(currentScreen != GAME && currentScreen != GAMEOVER && currentScreen != MENU && currentScreen != CONTROLS){
		        currentScreen = MENU;
			}
			break;
		//TS: 2020-04-14 the user will press h from the main menu
		//and it will go the controls page and if you push h
		//again in the controls page then it will go back to the
		//main menu
                case XK_h:
		    if(currentScreen == MENU)
	            currentScreen = CONTROLS;
		    else if(currentScreen != GAME && currentScreen != GAMEOVER && currentScreen != MENU && currentScreen != CREDITS){
			currentScreen = MENU;
		    }
		    break;
		//TS:2020-04-15 the user will press m from the game over
		//screen and it will go back to the main menu
		case XK_m:
		    if(currentScreen == GAMEOVER)
			currentScreen = MENU;
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
		//TS:2020-04-15 added to where you can push enter
		//from the gameover screen to go back to the game
		case XK_Return:
			if(currentScreen == MENU)
				currentScreen = GAME;
			else if(currentScreen != GAME && currentScreen != CREDITS && currentScreen != MENU && currentScreen != CONTROLS){
			    currentScreen = GAME;
			}
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
							//printf("Quit was clicked!\n"); - original
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
	//TS: 2020-04-11 the fix where the game would start at the menu
	//Is it time to move the snake?
	static struct timespec snakeTime;
	//static int firsttime=1; - original code
	//if(firsttime){ - original code
	if (currentScreen == MENU) {
		//firsttime=0; - original code
		clock_gettime(CLOCK_REALTIME, &snakeTime);
	}
	//TS: 2020-04-15 the game wont start when the game over screen 
	//is shown at the end of the game if the user decides to restart
	//the game after the game is over
	if(currentScreen == GAMEOVER){
	    clock_gettime(CLOCK_REALTIME, &snakeTime);
	}
	//TS: 2020-04-15 the game wont start at the credits screen
	if(currentScreen == CREDITS){
	    clock_gettime(CLOCK_REALTIME, &snakeTime);
	}
	//TS: 2020-04-15 the game wont start at the controls screen
	if(currentScreen == CONTROLS){
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
	//NL: 2020-04-11 - checks if snake is within the map, the outter range, collision detection
	//NL: 2020-04-16 - update to collision for new map & grid size
	if (g.snake.pos[0][0] < 2 || //left bound 
			g.snake.pos[0][0] > g.gridDim-1 || //right bound
			g.snake.pos[0][1] < 4 || //upper bound
			g.snake.pos[0][1] > g.gridDim-4) { //lower bound
		if(g.snake.pos[0][0] == 1){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][0] == g.gridDim){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][1] == 3){
			g.snake.pos[0][1] += 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-3){
			g.snake.pos[0][1] -= 1;
		}
	}
	if(g.snake.pos[0][0] > 2 &&
			g.snake.pos[0][0] < g.gridDim-44 &&
			g.snake.pos[0][1] < 7){
		if(g.snake.pos[0][0] == g.gridDim-45){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 6){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 39 &&
			g.snake.pos[0][0] < g.gridDim &&
			g.snake.pos[0][1] < 8){
		if(g.snake.pos[0][0] == 40){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][1] == 7){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 32 &&
			g.snake.pos[0][0] < g.gridDim &&
			g.snake.pos[0][1] > g.gridDim-6){
		if(g.snake.pos[0][0] == 33){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-5){
			g.snake.pos[0][1] -= 1;
		}
	}
	//bottom left section-ish
	if(g.snake.pos[0][0] > 5 && //left
			g.snake.pos[0][0] < g.gridDim-31 && //right
			g.snake.pos[0][1] > 48 && //upper
			g.snake.pos[0][1] < g.gridDim-8){ //lower
		if(g.snake.pos[0][0] == 6){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-32){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 49){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-9){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 13 && 
			g.snake.pos[0][0] < g.gridDim-31 &&
			g.snake.pos[0][1] > 44 &&
			g.snake.pos[0][1] < g.gridDim-8){
		if(g.snake.pos[0][0] == 14){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-32){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 45){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-9){
			g.snake.pos[0][1] += 1;
		}
	}
	//above the pond
	if(g.snake.pos[0][0] > 19 &&
			g.snake.pos[0][0] < g.gridDim-31 &&
			g.snake.pos[0][1] > 26 &&
			g.snake.pos[0][1] < g.gridDim-21){
		if(g.snake.pos[0][0] == 20){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-32){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 27){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-22){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 25 &&
			g.snake.pos[0][0] < g.gridDim-31 &&
			g.snake.pos[0][1] > 23 &&
			g.snake.pos[0][1] < g.gridDim-21){
		if(g.snake.pos[0][0] == 26){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-32){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 24){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-22){
			g.snake.pos[0][1] += 1;
		}
	}
	//middle left
	if(g.snake.pos[0][0] > 5 &&
			g.snake.pos[0][0] < g.gridDim-44 &&
			g.snake.pos[0][1] > 26 &&
			g.snake.pos[0][1] < g.gridDim-26){
		if(g.snake.pos[0][0] == 6){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-45){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 27){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-27){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 11 &&
			g.snake.pos[0][0] < g.gridDim-44 &&
			g.snake.pos[0][1] > 26 &&
			g.snake.pos[0][1] < g.gridDim-20){
		if(g.snake.pos[0][0] == 12){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-45){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 27){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-21){
			g.snake.pos[0][1] += 1;
		}
	}
	//top left section
	if(g.snake.pos[0][0] > 5 &&
			g.snake.pos[0][0] < g.gridDim-44 &&
			g.snake.pos[0][1] > 11 &&
			g.snake.pos[0][1] < g.gridDim-38){
		if(g.snake.pos[0][0] == 6){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-45){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 12){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-39){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 5 &&
			g.snake.pos[0][0] < g.gridDim-41 &&
			g.snake.pos[0][1] > 16 &&
			g.snake.pos[0][1] < g.gridDim-38){
		if(g.snake.pos[0][0] == 6){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-42){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 17){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-39){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 5 &&
			g.snake.pos[0][0] < g.gridDim-37 &&
			g.snake.pos[0][1] > 19 &&
			g.snake.pos[0][1] < g.gridDim-38){
		if(g.snake.pos[0][0] == 6){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-38){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 20){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-39){
			g.snake.pos[0][1] += 1;
		}
	}
	//lower right squares
	if(g.snake.pos[0][0] > 32 &&
			g.snake.pos[0][0] < g.gridDim-18 &&
			g.snake.pos[0][1] > 36 &&
			g.snake.pos[0][1] < g.gridDim-11){
		if(g.snake.pos[0][0] == 33){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-19){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 37){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-12){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 45 &&
			g.snake.pos[0][0] < g.gridDim-5 &&
			g.snake.pos[0][1] > 42 &&
			g.snake.pos[0][1] < g.gridDim-11){
		if(g.snake.pos[0][0] == 46){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-6){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 43){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-12){
			g.snake.pos[0][1] += 1;
		}
	}	
	//middle right section
	if(g.snake.pos[0][0] > 45 &&
			g.snake.pos[0][0] < g.gridDim-5 &&
			g.snake.pos[0][1] > 27 &&
			g.snake.pos[0][1] < g.gridDim-22){
		if(g.snake.pos[0][0] == 46){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-6){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 28){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-23){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 32 &&
			g.snake.pos[0][0] < g.gridDim-14 &&
			g.snake.pos[0][1] > 26 &&
			g.snake.pos[0][1] < g.gridDim-29){
		if(g.snake.pos[0][0] == 33){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-15){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 27){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-30){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 32 &&
			g.snake.pos[0][0] < g.gridDim-18 &&
			g.snake.pos[0][1] > 27 && 
			g.snake.pos[0][1] < g.gridDim-29){
		if(g.snake.pos[0][0] == 33){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-19){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 28){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-30){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 32 &&
			g.snake.pos[0][0] < g.gridDim-25 &&
			g.snake.pos[0][1] > 24 &&
			g.snake.pos[0][1] < g.gridDim-29){
		if(g.snake.pos[0][0] == 33){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-26){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 25){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-30){
			g.snake.pos[0][1] += 1;
		}
	}
	//top right section
	if(g.snake.pos[0][0] > 38 &&
			g.snake.pos[0][0] < g.gridDim-5 &&
			g.snake.pos[0][1] > 12 &&
			g.snake.pos[0][1] < g.gridDim-38){
		if(g.snake.pos[0][0] == 39){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-6){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 13){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-39){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 43 &&
			g.snake.pos[0][0] < g.gridDim-5 &&
			g.snake.pos[0][1] > 12 &&
			g.snake.pos[0][1] < g.gridDim-37){
		if(g.snake.pos[0][0] == 44){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-6){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 13){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-38){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 48 &&
			g.snake.pos[0][0] < g.gridDim-5 &&
			g.snake.pos[0][1] > 12 &&
			g.snake.pos[0][1] < g.gridDim-35){
		if(g.snake.pos[0][0] == 49){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-6){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 13){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-36){
			g.snake.pos[0][1] += 1;
		}
	}
	//top middle area
	if(g.snake.pos[0][0] > 20 &&
			g.snake.pos[0][0] < g.gridDim-25 &&
			g.snake.pos[0][1] > 8 &&
			g.snake.pos[0][1] < g.gridDim-48){
		if(g.snake.pos[0][0] == 21){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-26){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 9){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-49){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 22 &&
			g.snake.pos[0][0] < g.gridDim-25 &&
			g.snake.pos[0][1] > 8 &&
			g.snake.pos[0][1] < g.gridDim-45){
		if(g.snake.pos[0][0] == 23){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-26){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 8){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-46){
			g.snake.pos[0][1] += 1;
		}
	}
	if(g.snake.pos[0][0] > 25 &&
			g.snake.pos[0][0] < g.gridDim-25 &&
			g.snake.pos[0][1] > 8 &&
			g.snake.pos[0][1] < g.gridDim-41){
		if(g.snake.pos[0][0] == 26){
			g.snake.pos[0][0] -= 1;
		}
		if(g.snake.pos[0][0] == g.gridDim-26){
			g.snake.pos[0][0] += 1;
		}
		if(g.snake.pos[0][1] == 9){
			g.snake.pos[0][1] -= 1;
		}
		if(g.snake.pos[0][1] == g.gridDim-42){
			g.snake.pos[0][1] += 1;
		}
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
			// JG: 2020-05-16 - obtain random number to obtain index of random
			// set of valid coordinates within the vSpawn array.
			int randIndex = rand() % vSpwnLen;
			g.rat.pos[0] = vSpawn[randIndex][0];
			g.rat.pos[1] = vSpawn[randIndex][1];
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
         if(g.framesRemaining > 0 && currentScreen == GAME){
           countdown(g.framesRemaining);
	   if(g.framesRemaining%60 == 0)
	       g.timeRemaining = g.framesRemaining/60;
	 }
	 //TS:2020-04-13 when the timer gets to 0 the gameover screen 
	 //will show
	 //TS:2020-04-14 fixed the issue so the user can go back to main menu
	 //or either restart the game
         if(g.framesRemaining == 0){
           currentScreen = GAMEOVER;
           resetGame();
	 }

        //TS:2020-04-08 - load to check the gamestate
	//IF loop when the currentScreen is menu
	//then it will render the menu
	if (currentScreen == MENU){
		rateFixReset();
		renderMenu(g);
	}
	//TS:2020-04-13 - IF loop when the currentScreen
	//is gameover then it will render the gameOverScreen
	if(currentScreen == GAMEOVER){
	    renderGameOverScreen(g);
	}
	//TS:2020-04-14 IF loop when the currentScreen 
	//is credits then it will render the CreditsScreen
        if(currentScreen == CREDITS){
            renderCreditsScreen(g);
        }
	//TS:2020-04-14 IF loop when the currentScreen
	//is controls then it will render the ControlsScren
	if(currentScreen == CONTROLS){
	    renderControlsScreen(g);
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

		// JG: 2020-04-15 - rendering our collectible item
		renderItem(g);

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
