/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <math.h>
/* Global variables */
char title[] = "3D Shapes";

float X_RESOLUTION = 80;
float Y_RESOLUTION = 80;

float CAMERA_ANGLE = 0;

float getTerrainHeight(float x, float y)
{
   return sin(y);
}

typedef struct point
{
    float x, y, z;
}point;

typedef struct triangle
{
    point p1, p2, p3;
}triangle;

/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a pyramid consists of 4 triangles
   glLoadIdentity();                  // Reset the model-view matrix
   glTranslatef(0.0f, 1.0f, -1.0f);
   glRotatef(-60.0f, 1.0f, 0.0f, 0.0f);
      for(int i = -Y_RESOLUTION; i < Y_RESOLUTION; i++)
      {
        for(int j = -X_RESOLUTION; j < X_RESOLUTION; j++)
        {
            glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
            triangle t1 = {
                {(float)(j / X_RESOLUTION),       (float)(i / Y_RESOLUTION), -1.0f},
                {(float)(j / X_RESOLUTION),       (float)((i + 1) / Y_RESOLUTION), -1.0f},
                {(float)((j + 1) / X_RESOLUTION), (float)(i / Y_RESOLUTION), -1.0f},
            };

             triangle t2 = {
                {(float)((j + 1) / X_RESOLUTION), (float)((i + 1) / Y_RESOLUTION), -1.0f},
                {(float)(j / X_RESOLUTION),       (float)((i + 1) / Y_RESOLUTION), -1.0f},
                {(float)((j + 1) / X_RESOLUTION), (float)(i / Y_RESOLUTION), -1.0f}
            };

            float h1 = 0.1*getTerrainHeight(t1.p1.x, t1.p1.y);
            float h2 = 0.1*getTerrainHeight(t1.p2.x, t1.p2.y);
            float h3 = 0.1*getTerrainHeight(t1.p3.x, t1.p3.y);
            float h4 = 0.1*getTerrainHeight(t2.p1.x, t2.p1.y);

            glColor3f(37.0/225.0, 150.0/225.0, 190.0/225.0);
            glVertex3f( t1.p1.x, t1.p1.y, t1.p1.z + h1);
            glVertex3f( t1.p2.x, t1.p2.y, t1.p2.z + h2);
            glVertex3f( t1.p3.x, t1.p3.y, t1.p3.z + h3);

            glColor3f(237.0/225.0, 171.0/225.0, 97.0/225.0);
            glVertex3f( t2.p1.x, t2.p1.y, t2.p1.z + h4);
            glVertex3f( t2.p2.x, t2.p2.y, t2.p2.z + h2);
            glVertex3f( t2.p3.x, t2.p3.y, t2.p3.z + h3);
            glEnd();   // Done drawing the pyramid

        }
      }
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void rotateCamera(int value)
{
   CAMERA_ANGLE = CAMERA_ANGLE + 0.5;
   glutPostRedisplay();
   glutTimerFunc(100, rotateCamera, 1);
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   glutTimerFunc(100, rotateCamera, 1);
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}