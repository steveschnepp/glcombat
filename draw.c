#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>
#include <assert.h>

#include "draw.h"
#include "unit.h"

extern int nb_call_gl_Vertex3f;
extern int nb_call_gl_Vertex3fv;
extern int nb_call_glDrawArrays;
extern int nb_call_glDrawArrays_arrays;

static void draw_obj_gl(struct obj *o);
static void draw_map_gl(struct map *m);

static void draw_map_gl(struct map *m) {
	draw_obj_gl(&m->o);
}

void draw_map(struct map *m) {

	glPushMatrix();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
	glEnable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 1.0, 1.0 );

	const GLfloat WHITE[]  = { 1, 1, 1, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, WHITE);
	draw_map_gl(m);

	glPopMatrix();
}

static void draw_obj_gl(struct obj *o) {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, o->vertices_array);
	glNormalPointer(GL_FLOAT, 0, o->vertices_normal_array);

	glDrawArrays(GL_TRIANGLES, 0, o->nb_faces * 3);
	nb_call_glDrawArrays ++; nb_call_glDrawArrays_arrays += o->nb_faces * 3;

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void draw_obj(struct obj *o, struct v3f pos, struct v3f rot) {
	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.x, 1, 0, 0);
	glRotatef(rot.y, 0, 1, 0);
	glRotatef(rot.z, 0, 0, 1);

	if (!o->options.skip_solid) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 1.0, 1.0 );
		glColor3f(1, 1, 1);
		draw_obj_gl(o);
	}

	if (o->options.wireframe) {
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
		glVertex3fv(light_position); nb_call_gl_Vertex3fv++;
	glEnd();
	glEnable(GL_LIGHTING);
	glPointSize(1.0);

	glPopMatrix();
}

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

void draw_projectile(struct v3f pos, struct v3f vel) {

	printf("draw_projectile pos x %f y %f z %f ", pos.x, pos.y, pos.z);
	printf("vel x %f y %f z %f", vel.x, vel.y, vel.z);
	printf("\n");

	const GLfloat ORANGE[]  = { 1, 0.647, 0, .2 };

	glDisable(GL_LIGHTING);
	glEnable (GL_BLEND);
	glDepthMask (GL_FALSE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_LINES);
		glColor4fv(ORANGE);
		glVertex3fv(v3f_to_f3v(&pos)); nb_call_gl_Vertex3fv++;
		struct v3f end = v3f_sub(pos, v3f_mul(vel, 1.0/20));
		glVertex3fv(v3f_to_f3v(&end)); nb_call_gl_Vertex3fv++;
	glEnd();
	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);
	glEnable(GL_LIGHTING);
}

static GLUquadricObj *qobj;

void draw_explosion(struct v3f pos, float size) {
	printf("draw_explosion pos x %f y %f z %f size %f", pos.x, pos.y, pos.z, size);
	printf("\n");

	const GLfloat YELLOW[]  = { 1, 1, 0, 0.2 };

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);

	glDisable(GL_LIGHTING);
	glEnable (GL_BLEND);
	glDepthMask (GL_FALSE);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4fv(YELLOW);

	if (!qobj) qobj = gluNewQuadric();

	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluSphere(qobj, size, 8, 8);

	glDepthMask (GL_TRUE);
	glDisable (GL_BLEND);
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void draw_unit(struct unit *u) {
	struct obj *cube = object_get_by_name("cube");
	GLfloat color[]  = { u->color.r, u->color.g, u->color.b, u->color.a };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	draw_obj(cube, u->pos, u->rot);

#if 0
	glDisable(GL_LIGHTING);
	glLineWidth(1.0);
	glBegin(GL_LINES);
		glColor4fv(GREEN);
		glVertex3fv(v3f_to_f3v(&pos)); nb_call_gl_Vertex3fv++;
		glVertex3fv(v3f_to_f3v(&destination)); nb_call_gl_Vertex3fv++;
	glEnd();
	glEnable(GL_LIGHTING);
#endif
}
