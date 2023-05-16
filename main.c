#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <assert.h>

#include "object.h"
#include "map.h"
#include "v3f.h"

#define ANTIALIAS 0
#define OBJ_FILE "data/quad.obj"
#define MAP_FILE "data/cos.gen.map"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

float x = 0;
float y = 0;

void draw_obj_gl(struct obj *o);

void draw_map_gl(struct map *m) {
	draw_obj_gl(&m->o);
}

void draw_map(struct map *m) {
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1.0, 1.0 );
	glColor3f(1, 1, 1);
	draw_map_gl(m);

	return;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE  );
	glDisable( GL_POLYGON_OFFSET_FILL );
	glColor3f(.1, .1, .1);
	draw_map_gl(m);
	glPopMatrix();
}

void draw_obj_gl(struct obj *o) {
//	printf("draw_obj(%s);\n", o->name);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < o->nb_faces; i++) {
		for (int j = 0; j < 3; j++) {
			struct obj_faces faces = o->faces[i];
//			printf("i %d j %d o->nb_vertices %d vertice_idx %d\n", i, j, o->nb_vertices, vertice_idx);

			uint16_t vertice_normal_idx = faces.face[j].normal;
			if (vertice_normal_idx) {
				vertice_normal_idx --; // off by one in the obj file
				assert(vertice_normal_idx < o->nb_vertices_normal);
				float nx = o->vertices_normal[vertice_normal_idx].x;
				float ny = o->vertices_normal[vertice_normal_idx].y;
				float nz = o->vertices_normal[vertice_normal_idx].z;
				glNormal3f(nx, ny, nz);
//				printf("glNormal3f(%f, %f, %f);\n", nx, ny, nz);
			}

			uint16_t vertice_idx = faces.face[j].vertice;
			if (vertice_idx) {
				vertice_idx --; // off by one in the obj file
				assert(vertice_idx < o->nb_vertices);
				float x = o->vertices[vertice_idx].x;
				float y = o->vertices[vertice_idx].y;
				float z = o->vertices[vertice_idx].z;
//				printf("glVertex3f(%f, %f, %f);\n", x, y, z);
				glVertex3f(x, y, z);
			}


		}
	}
	glEnd();
}

void draw_obj(struct obj *o, struct v3f pos, struct v3f rot) {
	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);

	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1.0, 1.0 );
	glColor3f(1, 1, 1);
	draw_obj_gl(o);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE  );
	glDisable( GL_POLYGON_OFFSET_FILL );
	glColor3f(.1, .1, .1);
	draw_obj_gl(o);
	glPopMatrix();
}

static float angleX = 0;
static float angleY = 0;
static float angleZ = 0;

GLfloat LIGHT_BLACK[]  = { 0 };

void setup_light(int light, float offset, float r, float g, float b) {
	float radius = 32;
	float hover = 10;

	glPushMatrix();

	glRotatef(45, 0, 0, 1);

	glEnable(GL_LIGHTING);

	GLfloat light_ambient[]  = { 0.1f * r, 0.1f * g, 0.1f * b, 0.0f };
	GLfloat light_diffuse[]  = { r, g, b, 1.0f };
	GLfloat light_position[] = { radius * sinf(offset * angleX / 360.0), radius * cosf(offset * angleX / 360.0), hover + hover * cosf(offset*angleX/720), 1.0f };

	glLightfv(light, GL_AMBIENT, light_ambient);
	glLightfv(light, GL_DIFFUSE, light_diffuse);
	glLightfv(light, GL_POSITION, light_position);
	{
		float light_attenuation = 0.0f;
		glLightfv(light, GL_CONSTANT_ATTENUATION, &light_attenuation);

		light_attenuation = 0.1f;
		glLightfv(light, GL_LINEAR_ATTENUATION, &light_attenuation);

		light_attenuation = 0.01f;
		glLightfv(light, GL_QUADRATIC_ATTENUATION, &light_attenuation);
	}
	glEnable(light);

	glDisable(GL_LIGHTING);
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glColor3fv(light_diffuse);
		glVertex3fv(light_position);
	glEnd();
	glEnable(GL_LIGHTING);
	glPointSize(1.0);

	glPopMatrix();
}

void setupLights() {
	setup_light(GL_LIGHT1, 1, 1, 0, 0);
	setup_light(GL_LIGHT2, 2, 0, 1, 0);
	setup_light(GL_LIGHT3, 3, 0, 0, 1);
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

float camera_x = 100.0;
float camera_y = 30.0;
float camera_z = 40.0;

float camera_extra_rot_x = 0.0;
float camera_extra_rot_y = 0.0;
float camera_extra_rot_z = 0.0;

void draw() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		//eye
		camera_x, camera_y, camera_z,
		// center
		0, 0, 0,
		// up
		0, 0, 1
	);

	setupLights();

	glEnable(GL_COLOR_MATERIAL);

	// Put a refernce point at the origin
	glLineWidth(5.0);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
	glEnd();
	glLineWidth(1.0);

	glRotatef(camera_extra_rot_z, 0, 0, 1);
	glRotatef(camera_extra_rot_y, 0, 1, 0);
	glRotatef(camera_extra_rot_x, 1, 0, 0);

	draw_map(&map);

	glRotatef(angleZ, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	glRotatef(angleX, 0, 0, 1);

	struct v3f pos = {0};
	struct v3f rot = {0};
	draw_obj(&cube, pos, rot);

//	printf("glFlush();\n");

	glFlush();
}


int main(int argc, char* argv[]) {
	SDL_Init( SDL_INIT_VIDEO );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );

	SDL_Window *window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
	SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

	// Initialization de OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

#if ANTIALIAS
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
	gluPerspective(45.0, 1.0 * SCREEN_WIDTH / SCREEN_HEIGHT, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LIGHT_BLACK);
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
				printf("SDL_MOUSEMOTION x %d y %d\n", e.motion.xrel, e.motion.yrel);
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
				printf("camera_x %f camera_y %f camera_z %f\n", camera_x, camera_y, camera_z);
			}
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
