/*
SDL2 + OpenGL demo, prepared by Nick Toothman.
*/
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

using u32 = uint32_t;

// Size of window in pixels
#define WinWidth 1000
#define WinHeight 1000

// How long it takes to switch from one color to another in milliseconds
#define ColorChange 3000

int main (int ArgCount, char **Args)
{
    // Tries to initialize all SDL subsystems (video, audio, input, etc).
    // Exits on failure
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }
    
    // Sets a callback function so if the progrm exits from any location, 
    // SDL cleans up after itself.
    atexit(SDL_Quit);

    // 3.0 will display correctly if you run it remotely on Odin with X11 window
    // forwarding (ssh nick@odin -YC), but anything later (up to 4.5) will 
    // not display on remote. It's a bad idea to execute remotely (too slow), so
    // copy the code to your local Linux machine and try running it there.
    // You can see what the max supported version is on a system by running 
    // `glxinfo | grep version' and looking for the max core/compat profiles. 
    // If you don't get a working window, try adjusting the version to match
    // the output of glxinfo.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Compatibility mode means you can execute deprecated functions for 
    // backwards compatibilty. This can be helpful for beginners learning how
    // to use OpenGL. If you want to stick to core functions, use 
    // SDL_GL_CONTEXT_PROFILE_CORE instead.
    // Note that https://learnopengl.com/ will stick mainly to core functions
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    u32 context_flags = 0;

    // You can run `make clean ; make debug ' to get a build that's useful for
    // debugging (error messages will be more verbose). By default this is off.
#ifdef DEBUG
    context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);

    u32 WindowFlags = SDL_WINDOW_OPENGL;

    SDL_Window *Window = SDL_CreateWindow("SDL + OpenGL Test", 0, 0, WinWidth, 
        WinHeight, WindowFlags);
    assert(Window);
    SDL_GLContext Context = SDL_GL_CreateContext(Window);

    if (Context == NULL)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK )
    {
        std::cerr << "Error initializing GLEW! " 
                  << glewGetErrorString(glewError) << "\n";
        return EXIT_FAILURE;
    }

    bool Running = true;
    bool FullScreen = false;

    // Lambda function to generate a random color
    auto randomColor = []()
    {
        return glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
    };

    glm::vec3 lastColor;
    glm::vec3 nextColor = randomColor();
    u32 lastColorChange = SDL_GetTicks();
    u32 now = lastColorChange;

    // Lambda function to update the next color value and timer
    auto updateColor = [&]()
    {
        lastColorChange = now;
        lastColor = nextColor;
        nextColor = randomColor();
    };
    
    while (Running)
    {
        SDL_Event Event;
        while (SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
            {
                switch (Event.key.keysym.sym)
                {
                    // Exit on escape
                    case SDLK_ESCAPE:
                        Running = false;
                        break;
                    // Toggle fullscreen/window on f
                    case 'f':
                        FullScreen = !FullScreen;
                        if (FullScreen)
                        {
                            SDL_SetWindowFullscreen(Window, 
                                WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                        }
                        else
                        {
                            SDL_SetWindowFullscreen(Window, WindowFlags);
                        }
                        break;
                    case ' ':
                        updateColor(); 
                    default:
                        break;
                }
            }
            else if (Event.type == SDL_QUIT)
            {
                Running = 0;
            }
        }

        glViewport(0, 0, WinWidth, WinHeight);

        now = SDL_GetTicks();

        if (now - lastColorChange >= ColorChange)
        {
            updateColor();
        }

        float t = (float)(now - lastColorChange) / ColorChange;

        glm::vec3 mixColor = glm::mix(lastColor, nextColor, t);
        glClearColor(mixColor.x, mixColor.y, mixColor.z, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(Window);
    }

    SDL_GL_DeleteContext(Context);
    return 0;
}