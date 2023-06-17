#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <assert.h>

#include "object.h"
#include "map.h"
#include "v3f.h"
#include "draw.h"

#define ANTIALIAS 0
#define OBJ_FILE "data/teapot_surface2.obj"
#define MAP_FILE "data/b1heightmap.map"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

float x = 0;
float y = 0;

static float angleX = 0;
static float angleY = 0;
static float angleZ = 0;

GLfloat LIGHT_BLACK[4]  = { 0 };

struct c4f COLOR_RED =   { 1, 0, 0, 1, };
struct c4f COLOR_GREEN = { 0, 1, 0, 1, };
struct c4f COLOR_BLUE =  { 0, 0, 1, 1, };

void setupLights() {
	float radius = 25;
	float hover = 10;
	{
		float offset = 1;
		struct v3f pos = {
			radius * sinf(offset * angleX / 360.0),
			radius * cosf(3 * offset * angleX / 360.0),
			hover + hover * cosf(offset*angleX/720) / 2,
		};
		draw_light(1, pos, COLOR_RED);
	}

	{
		float offset = 2;
		struct v3f pos = {
			radius * sinf(offset * angleX / 360.0),
			radius * cosf(2 * offset * angleX / 360.0),
			hover + hover * cosf(offset*angleX/720) / 2,
		};
		draw_light(2, pos, COLOR_GREEN);
	}

	{
		float offset = 3;
		struct v3f pos = {
			radius * sinf(offset * angleX / 360.0),
			radius * cosf(offset * angleX / 360.0),
			hover + hover * cosf(offset*angleX/720) / 2,
		};
		draw_light(3, pos, COLOR_BLUE);
	}
}

struct obj cube;
struct map map;

int object_rotate_active = 1;

void update(uint32_t delta_ms) {

	if (! object_rotate_active ) return;

	angleX += 92.0f * delta_ms / 1000;
	angleY += 0.1 / 36;
	angleZ += 0.1 / 36 / 36;

}

float camera_x = 4.0;
float camera_y = -36.0;
float camera_z = 6.0;

float camera_extra_rot_x = -144.0;
float camera_extra_rot_y = 0.0;
float camera_extra_rot_z = 91.0;

void draw_reference() {
	// Put a refernce point at the origin
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable (GL_BLEND);
	glDepthMask (GL_FALSE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	glLineWidth(5.0);
	glBegin(GL_LINES);
		glColor4f(1, 0, 0, .25);
		glVertex3f(0, 0, 0);
		glVertex3f(10, 0, 0);

		glColor4f(0, 1, 0, .25);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 10, 0);

		glColor4f(0, 0, 1, .25);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 10);
	glEnd();
	glLineWidth(1.0);

	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void draw() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(90, 1, 0, 0);

	glTranslatef(camera_x, camera_y, camera_z);

	glRotatef(camera_extra_rot_x, 1, 0, 0);
	glRotatef(camera_extra_rot_y, 0, 1, 0);
	glRotatef(camera_extra_rot_z, 0, 0, 1);

	draw_reference();

	// setupLights();

	draw_options.no_wireframe = 1;
	draw_map(&map);

	draw_options.no_wireframe = 01;
	struct v3f pos = {4, 4, 4};
	struct v3f rot = {angleX, angleY, angleZ};
//	draw_obj(&cube, pos, rot);

	draw_reference();
}


int main(int argc, char* argv[]) {
	SDL_Init( SDL_INIT_VIDEO );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

	SDL_Window *window = SDL_CreateWindow( "GL Combat", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
	SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

	// Initialization de OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glShadeModel(GL_FLAT);
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
#endif

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0 * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 1000.0);

	GLfloat light_ambient[]  = { .05, .05, .017 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
	//glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0);


	glEnable(GL_LIGHT0);

#if 0
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 5.0, 20.0, 5.0, 1.0 };

//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
#endif

	object_read_from_file(&cube, OBJ_FILE);
	map_load(&map, MAP_FILE);

	{
		int maxlights;
		glGetIntegerv(GL_MAX_LIGHTS, &maxlights);
		printf("maxlights %d\n", maxlights);
	}

	uint32_t start_frame_tick = SDL_GetTicks();

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

			static int camera_rotate_active = 0;
			if( e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT) {
				camera_rotate_active = 1;
			}

			if( e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT) {
				camera_rotate_active = 0;
			}

			if( e.type == SDL_MOUSEMOTION && camera_rotate_active) {
				camera_extra_rot_z += e.motion.xrel / 3.0;
				camera_extra_rot_x += e.motion.yrel / 3.0;
			}

			if( e.type == SDL_MOUSEWHEEL) {
				switch(e.wheel.type) {
				case SDL_MOUSEWHEEL:
					camera_z += e.wheel.y;
					break;

				default:
					break;
				}
			}



			if( e.type == SDL_KEYDOWN ) {
				switch (e.key.keysym.sym) {
					case 'q':
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case ' ':
						object_rotate_active = !object_rotate_active;
						break;
					case SDLK_UP:
						camera_y -= 1.0;
						break;
					case SDLK_DOWN:
						camera_y += 1.0;
						break;
					case SDLK_LEFT:
						camera_x -= 1.0;
						break;
					case SDLK_RIGHT:
						camera_x += 1.0;
						break;
				}
			}
			printf("camera_x %f camera_y %f camera_z %f\n", camera_x, camera_y, camera_z);
			printf("camera_extra_rot x %f y %f z %f\n", camera_extra_rot_x, camera_extra_rot_y, camera_extra_rot_z);
		}

		uint32_t now = SDL_GetTicks();
		update(now - start_frame_tick);
		start_frame_tick = now;

		draw();

		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
