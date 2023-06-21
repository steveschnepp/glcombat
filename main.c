#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>

#include <assert.h>

#include "v3f.h"
#include "object.h"
#include "projectile.h"
#include "map.h"
#include "draw.h"

#define ANTIALIAS 0
#define OBJ_FILE "data/cube.obj"
#define MAP_FILE "data/b1heightmap.map"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

int nb_call_gl_Vertex3f;
int nb_call_gl_Vertex3fv;

float x = 0;
float y = 0;

static float angleX = 0;
static float angleY = 0;
static float angleZ = 0;

# define PI          3.141592653589793238462643383279502884L /* pi */

static float rad2deg(float radians) {
    return radians * (180.0 / PI);
}

static float deg2rad(float degrees) {
    return degrees * (PI  / 180.0);
}

static float cosfd(float degrees) {
	float rad = deg2rad(degrees);
    return cosf(rad);
}

static float sinfd(float degrees) {
	float rad = deg2rad(degrees);
    return sinf(rad);
}

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

struct obj *cube;
struct map map;
int i_step = 4;
int j_step = 4;

int object_rotate_active = 1;

void update(uint32_t delta_ms) {

	if (! object_rotate_active ) return;

	angleX += delta_ms * (1.0 / 30);
	angleY += 0.1 / 36;
	angleZ += 0.1 / 36 / 36;

	projectile_update_all(delta_ms, &map);
}

float camera_x = 76.0;
float camera_y = 64.0;
float camera_z = -65.0;

float camera_rot_x = -70.0;
float camera_rot_y = 0.0;
float camera_rot_z = 44.0;

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
		glVertex3f(0, 0, 0);  nb_call_gl_Vertex3f++;
		glVertex3f(10, 0, 0); nb_call_gl_Vertex3f++;

		glColor4f(0, 1, 0, .25);
		glVertex3f(0, 0, 0);  nb_call_gl_Vertex3f++;
		glVertex3f(0, 10, 0);  nb_call_gl_Vertex3f++;

		glColor4f(0, 0, 1, .25);
		glVertex3f(0, 0, 0); nb_call_gl_Vertex3f++;
		glVertex3f(0, 0, 10);  nb_call_gl_Vertex3f++;
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
	glRotatef(9	, 1, 0, 0);

	glRotatef(camera_rot_x, 1, 0, 0);
	glRotatef(camera_rot_y, 0, 1, 0);
	glRotatef(camera_rot_z, 0, 0, 1);

	glTranslatef(camera_x, camera_y, camera_z);

	draw_reference();

	draw_map(&map);

	struct v3f pos = {10 * cosfd(angleX), 10 * sinfd(angleX), map_get_height(&map, pos.x, pos.y) };
	struct v3f rot = {0, 0, angleX};
	draw_obj(cube, pos, rot);

	printf("new_bullet_idx %d \n", new_bullet_idx);
	for (int i = 0; i < new_bullet_idx; i++) {
		struct projectile *p = bullets + i;
		if (p->props.is_explosion) {
			draw_explosion(p->pos, p->delta.x);
		} else {
			draw_projectile(p->pos, p->delta);
		}
	}
}

void emit_projectile() {
	struct v3f pos = {10 * cosfd(angleX), 10 * sinfd(angleX), map_get_height(&map, pos.x, pos.y) };
	struct v3f vel = { 10 * cosfd(angleX), 10 * sinfd(angleX), 30 };

	projectile_new(pos, vel);
}


int main(int argc, char* argv[]) {
	SDL_Init( SDL_INIT_VIDEO );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

	int sdl_flags = 0;
	sdl_flags |= SDL_WINDOW_RESIZABLE;
	sdl_flags |= SDL_WINDOW_OPENGL;
	sdl_flags |= SDL_WINDOW_SHOWN;
	SDL_Window *window = SDL_CreateWindow( "GL Combat", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, sdl_flags);

	SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(-1); // adaptive vsync

	// Initialization de OpenGL
	glEnable(GL_DEPTH_TEST);

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
/*
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0);
*/

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

	cube = object_new_from_file(OBJ_FILE);
	map_load(&map, MAP_FILE, i_step, j_step);

	{
		int maxlights;
		glGetIntegerv(GL_MAX_LIGHTS, &maxlights);
		printf("maxlights %d\n", maxlights);
	}

	uint32_t last_frame_tick = SDL_GetTicks();

	bool fullScreen = false;
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
				camera_rot_z += e.motion.xrel / 3.0;
				camera_rot_x += e.motion.yrel / 3.0;
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

			if(e.type == SDL_WINDOWEVENT) {
				switch(e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
					{
						int new_width = e.window.data1;
						int new_height = e.window.data2;
						glViewport(0, 0, new_width, new_height);

						glMatrixMode(GL_PROJECTION);
						glLoadIdentity();
						gluPerspective(60.0, 1.0 * new_width / new_height, 0.1, 1000.0);
					}

					break;

				default:
					break;
				}
			}

			if( e.type == SDL_KEYDOWN ) {
				switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case ' ':
						emit_projectile();
						break;
					case 'z':
					case 'w':
					case SDLK_UP:
						camera_y -= 1.0 * cosfd(camera_rot_z);
						camera_x -= 1.0 * sinfd(camera_rot_z);
						break;
					case 's':
					case SDLK_DOWN:
						camera_y += 1.0 * cosfd(camera_rot_z);
						camera_x += 1.0 * sinfd(camera_rot_z);
						break;
					case 'a':
					case 'q':
					case SDLK_LEFT:
						camera_y += 1.0 * cosfd(camera_rot_z + 90);
						camera_x += 1.0 * sinfd(camera_rot_z + 90);
						break;
					case 'd':
					case SDLK_RIGHT:
						camera_y += 1.0 * cosfd(camera_rot_z - 90);
						camera_x += 1.0 * sinfd(camera_rot_z - 90);
						break;
					case 'i':
						i_step += 1;
						j_step += 1;
						if (i_step > 0 && j_step > 0) map_load(&map, MAP_FILE, i_step, j_step);

						break;
					case 'u':
						i_step -= 1;
						j_step -= 1;
						if (i_step > 0 && j_step > 0) map_load(&map, MAP_FILE, i_step, j_step);

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

		}

		uint32_t start_frame_tick = SDL_GetTicks();
		update(start_frame_tick - last_frame_tick);
		uint32_t start_draw_tick = SDL_GetTicks();
		draw();
		uint32_t end_draw_tick = SDL_GetTicks();

		printf("updated %d draw %d ms\n", start_draw_tick - start_frame_tick, end_draw_tick - start_draw_tick);
		printf("angle:      x % 11.4f y % 11.4f z % 11.4f\n", angleX, angleY, angleY);
		printf("camera:     x % 11.4f y % 11.4f z % 11.4f\n", camera_x, camera_y, camera_z);
		printf("camera_rot: x % 11.4f y % 11.4f z % 11.4f\n", camera_rot_x, camera_rot_y, camera_rot_z);
		printf("glVertex:   3f % 10d 3fv % 9d\n", nb_call_gl_Vertex3f, nb_call_gl_Vertex3fv);
		printf("map_steps:  i  % 10d j % 12d\n", i_step, j_step);

		SDL_GL_SwapWindow(window);

		nb_call_gl_Vertex3f = nb_call_gl_Vertex3fv = 0;

		last_frame_tick = start_draw_tick;
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
}
