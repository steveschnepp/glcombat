#include <SDL.h>

// Needs 2 lines because https://gcc.gnu.org/onlinedocs/gcc-3.4.3/cpp/Stringification.html
#define __STRINGIFY__(x) #x
#define _(x) __STRINGIFY__(x)


#if OPENGL_ENABLED
#include <SDL_opengl.h>
#include <GL/glu.h>
#endif

extern SDL_Window *window;
extern SDL_Renderer *renderer;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern unsigned long frame;

extern char* APP_TITLE;

enum draw_keepscreen {
    UDSL_KEEPSCREEN = 0,
    UDSL_CLEARSCREEN = 1,
};

/** 
 * Callback when drawing a frame
 * 
 * @param frame current frame to display
 * @return 0 if the screen shall not be cleared on the next frame
 * 
*/
enum draw_keepscreen draw(unsigned long frame);
void update(uint32_t delta_ms);
void setup(int argc, char* argv[]);
void update_event(SDL_Event e);

void screenshot();
