#include "usdl.h"

#include "SDL_Image.h"

SDL_Window *window;
SDL_Renderer *renderer;

int SCREEN_WIDTH = 720;
int SCREEN_HEIGHT = 576;
unsigned long frame = 0;

__attribute__((weak))
char* APP_TITLE = _(USDL_APPNAME);

__attribute__((weak))
void setup(int argc, char* argv[])
{
}

__attribute__((weak))
void update_event(SDL_Event e)
{
}

__attribute__((weak))
void update(uint32_t delta_ms)
{
}


__attribute__((weak))
enum draw_keepscreen draw(unsigned long frame)
{
	return 1; // should_clear_screen
}

void clear_screen() {
#if OPENGL_ENABLED
			glClearColor(0, 0, 0, 0);
	        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
#else // OPENGL_ENABLED
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
#endif // OPENGL_ENABLED
}

int main(int argc, char* argv[])
{
	SDL_Init( SDL_INIT_VIDEO );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

	int sdl_flags = 0;
	sdl_flags |= SDL_WINDOW_RESIZABLE;
#if OPENGL_ENABLED
	sdl_flags |= SDL_WINDOW_OPENGL;
#endif // OPENGL_ENABLED
	sdl_flags |= SDL_WINDOW_SHOWN;
	window = SDL_CreateWindow( "APP_TITLE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, sdl_flags);

	int sdl_renderer_flags = 0;
	sdl_renderer_flags |= SDL_RENDERER_PRESENTVSYNC;
	renderer = SDL_CreateRenderer(window, -1, sdl_renderer_flags);

#if OPENGL_ENABLED
	SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(-1); // adaptive vsync
#endif // OPENGL_ENABLED

	SDL_RaiseWindow(window);

#if OPENGL_ENABLED
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEPTH_TEST);

#if ANTIALIAS
	glShadeModel(GL_SMOOTH);

	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_LINE_SMOOTH );
	glEnable( GL_POLYGON_SMOOTH );

	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_FLAT);
#else // ANTIALIAS
#endif // ANTIALIAS
	glShadeModel(GL_SMOOTH);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0 * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 1000.0);
#endif  // OPENGL_ENABLED

	setup(argc, argv);

	long last_frame_tick = SDL_GetTicks();

	int fullScreen = 0;
	int quit = 0;
	while (! quit) {
		SDL_Event e;
		// Gere la file d'événements
		while( SDL_PollEvent( &e ) != 0 ) {
			//User requests quit
			if( e.type == SDL_QUIT ) {
				quit = 1;
				break; // while( SDL_PollEvent( &e ) != 0 )
			}

			if(e.type == SDL_WINDOWEVENT) {
				switch(e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					{
						int new_width = e.window.data1;
						int new_height = e.window.data2;
#if OPENGL_ENABLED
						glViewport(0, 0, new_width, new_height);

						glMatrixMode(GL_PROJECTION);
						glLoadIdentity();
						gluPerspective(60.0, 1.0 * new_width / new_height, 0.1, 1000.0);
#endif // OPENGL_ENABLED
						SCREEN_WIDTH = new_width;
						SCREEN_HEIGHT = new_height;
					}

					break;

				default:
					break;
				}
			}

			if( e.type == SDL_KEYDOWN ) {
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						goto quit;
						break;
					case 'f':
						fullScreen = !fullScreen;
						if(fullScreen){
							SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						} else {
							SDL_SetWindowFullscreen(window, 0);
						}

						break;
				}
			}

			update_event(e);
		}

		uint32_t start_frame_tick = SDL_GetTicks();
		update(start_frame_tick - last_frame_tick);
		uint32_t start_draw_tick = SDL_GetTicks();

        // Draw
        {
			static enum draw_keepscreen should_clear_screen = UDSL_CLEARSCREEN;
			if (should_clear_screen == UDSL_KEEPSCREEN) clear_screen();
            should_clear_screen = draw(frame);
			frame++;
        }

		uint32_t end_draw_tick = SDL_GetTicks();

#if OPENGL_ENABLED
		SDL_GL_SwapWindow(window);
#else // OPENGL_ENABLED
		SDL_RenderPresent(renderer);
#endif // OPENGL_ENABLED
		uint32_t end_sync_tick = SDL_GetTicks();
		printf(
			"updated %d draw %d wait %d ms\n",
			start_draw_tick - start_frame_tick,
			end_draw_tick - start_draw_tick,
			end_sync_tick - end_draw_tick
		);

		last_frame_tick = start_frame_tick;
	}

quit:
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


void screenshot() {
	static int filenumber = 0;
	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	SDL_Surface *screenshot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, screenshot->pixels, screenshot->pitch);

	char filename[256] = {0};
	sprintf(filename, "screenshots/file-%d.png", filenumber);
	IMG_SavePNG(screenshot, filename);
	SDL_FreeSurface(screenshot);
	filenumber++;
}
