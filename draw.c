#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <assert.h>

#include "draw.h"


struct draw_options draw_options;

static void draw_obj_gl(struct obj *o);
static void draw_map_gl(struct map *m);

static void draw_map_gl(struct map *m) {
	draw_obj_gl(&m->o);
}

void draw_map(struct map *m) {
	glPushMatrix();

	if (!draw_options.no_fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 1.0, 1.0 );
		glColor3f(1, 1, 1);
		draw_map_gl(m);
	}

	if (!draw_options.no_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE  );
		glDisable( GL_POLYGON_OFFSET_FILL );
		glColor3f(.1, .1, .1);
		draw_map_gl(m);
	}

	glPopMatrix();
}

static void draw_obj_gl(struct obj *o) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < o->nb_faces; i++) {
		for (int j = 0; j < 3; j++) {
			struct obj_faces faces = o->faces[i];

			unsigned int vertice_normal_idx = faces.face[j].normal;
			if (vertice_normal_idx) {
				vertice_normal_idx --; // off by one in the obj file
				assert(vertice_normal_idx < o->nb_vertices_normal);
				float nx = o->vertices_normal[vertice_normal_idx].x;
				float ny = o->vertices_normal[vertice_normal_idx].y;
				float nz = o->vertices_normal[vertice_normal_idx].z;
				glNormal3f(nx, ny, nz);
			}

			unsigned int vertice_idx = faces.face[j].vertice;
			if (vertice_idx) {
				vertice_idx --; // off by one in the obj file
				assert(vertice_idx < o->nb_vertices);
				float x = o->vertices[vertice_idx].x;
				float y = o->vertices[vertice_idx].y;
				float z = o->vertices[vertice_idx].z;
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

	if (!draw_options.no_fill) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 1.0, 1.0 );
		glColor3f(1, 1, 1);
		draw_obj_gl(o);
	}

	if (!draw_options.no_wireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE  );
		glDisable( GL_POLYGON_OFFSET_FILL );
		glColor3f(.1, .1, .1);
		draw_obj_gl(o);
	}

	glPopMatrix();
}

void draw_light(int light, struct v3f pos, struct c4f col) {
	float radius = 32;
	float hover = 10;
	int gl_light = GL_LIGHT0 + light;

	glPushMatrix();

	glEnable(GL_LIGHTING);

	GLfloat light_diffuse[]  = { col.r/2, col.g/2, col.b/2, col.a/2 };
	float is_position = 1.0; // Point, not directional
	GLfloat light_position[] = { pos.x, pos.y, pos.z, is_position, };

	glLightfv(gl_light, GL_DIFFUSE, light_diffuse);
	glLightfv(gl_light, GL_POSITION, light_position);

	{
		float light_attenuation = 0.0f;
		glLightfv(gl_light, GL_CONSTANT_ATTENUATION, &light_attenuation);

		light_attenuation = 0.1f;
		glLightfv(gl_light, GL_LINEAR_ATTENUATION, &light_attenuation);

		light_attenuation = 0.01f;
		glLightfv(gl_light, GL_QUADRATIC_ATTENUATION, &light_attenuation);
	}
	glEnable(gl_light);

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
