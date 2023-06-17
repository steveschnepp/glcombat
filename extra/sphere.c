#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <math.h>

void init(void)
{
   GLfloat mat_shininess[] = { 50.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   glShadeModel (GL_SMOOTH);

   glEnable(GL_LIGHTING);

   {
	   GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 0.0 };
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	   glEnable(GL_LIGHT0);
   }

   {
	   GLfloat mat_specular[] = { 0.0, 1.0, 0.0, 0.0 };
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	   glEnable(GL_LIGHT1);
   }

   {
	   GLfloat mat_specular[] = { 0.0, 0.0, 1.0, 0.0 };
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	   glEnable(GL_LIGHT2);
   }

   glEnable(GL_DEPTH_TEST);
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glutSolidSphere (1.0, 32, 32);
   static float frame = 0; frame += .001;
   {
   GLfloat light_position[] = { cosf(frame) * 1.0, sinf(frame) * 1.0, cosf(frame * 3.5) * 3.0, 0.0 };
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   }
   {
   GLfloat light_position[] = { cosf(frame + 3.14) * 1.0, sinf(frame) * 1.0, cosf(frame * 3.5) * 3.0, 0.0 };
   glLightfv(GL_LIGHT1, GL_POSITION, light_position);
   }
   {
   GLfloat light_position[] = { cosf(frame * 2) * 1.0, sinf(frame) * 1.0, cosf(frame * 3.5) * 3.0, 0.0 };
   glLightfv(GL_LIGHT2, GL_POSITION, light_position);
   }
   glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho (-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w,
         1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho (-1.5*(GLfloat)w/(GLfloat)h,
         1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5, -10.0, 10.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void timer(int t) {
    glutPostRedisplay();
    glutTimerFunc(1000/60, timer, 0);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   timer(0);
   glutMainLoop();
   return 0;
}
