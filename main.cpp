/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <stdio.h>
#include <math.h>
#include "definitions.h"

/* Global variables */
char title[] = "3D Shapes";

float X_RESOLUTION = 80;
float Y_RESOLUTION = 80;

float PERSISTENCE = 0.5;
float LACUNARITY = 2.5;

float ambientStrength = 0.5;

color lightColor = {1.0f, 1.0f, 1.0f};
color ambient = {lightColor.r * ambientStrength, 
                 lightColor.g * ambientStrength, 
                 lightColor.b * ambientStrength};

color terrainColor = {37.0/225.0, 150.0/225.0, 190.0/225.0};

float getTerrainHeight(float x, float y)
{
   //float n = y * 10;
   float val = 0;
   float amplitude = 0.5;
   float frequency = 1.0;
   for(int i = 0; i < 8; i++)
   {
      val+=  amplitude * (sin(y * frequency) * sin(x * frequency));
      amplitude *= PERSISTENCE;
      frequency *= LACUNARITY;
   }
   return val;
}

camera cam {{0.0f, 1.0f, -1.0f},
            {-45.0f, 0.0f, 0.0f}};



/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(1.0f, 1.0f, 1.0f, 0.5f); // Set background color to black and opaque
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
   glTranslatef(cam.pos.x, cam.pos.y, cam.pos.z);
   glRotatef(cam.ang.angleX, 1.0f, 0.0f, 0.0f);
   glRotatef(cam.ang.angleY, 0.0f, 1.0f, 0.0f);
   glRotatef(cam.ang.angleZ, 0.0f, 0.0f, 1.0f);

      for(int i = -Y_RESOLUTION; i < Y_RESOLUTION; i++)
      {
        for(int j = -X_RESOLUTION; j < X_RESOLUTION; j++)
        {
            glBegin(GL_TRIANGLES);          
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
            color drawColor = { ambient.r * terrainColor.r,
                                ambient.g * terrainColor.g,
                                ambient.b * terrainColor.b};
            drawColor = {37.0/225.0, 150.0/225.0, 190.0/225.0};
            glColor3f(drawColor.r, drawColor.g, drawColor.b);
            float h1 = getTerrainHeight(t1.p1.x, t1.p1.y);
            float h2 = getTerrainHeight(t1.p2.x, t1.p2.y);
            float h3 = getTerrainHeight(t1.p3.x, t1.p3.y);
            float h4 = getTerrainHeight(t2.p1.x, t2.p1.y);

            glVertex3f( t1.p1.x, t1.p1.y, t1.p1.z + h1);
            glVertex3f( t1.p2.x, t1.p2.y, t1.p2.z + h2);
            glVertex3f( t1.p3.x, t1.p3.y, t1.p3.z + h3);
            drawColor = {87.0/225.0, 15.0/225.0, 112.0/225.0};
            glColor3f(drawColor.r, drawColor.g, drawColor.b);

            glVertex3f( t2.p1.x, t2.p1.y, t2.p1.z + h4);
            glVertex3f( t2.p2.x, t2.p2.y, t2.p2.z + h2);
            glVertex3f( t2.p3.x, t2.p3.y, t2.p3.z + h3);
            glEnd();  

        }
      }
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

void handleKeyboard(int key, int x, int y)
{
   int mod = glutGetModifiers();
   if(key == GLUT_KEY_UP)
   {
      if(mod == GLUT_ACTIVE_SHIFT)
      {
         cam.ang.angleX -= 1;
      }
      else
      {
         cam.pos.z += 0.01;
      }
   }
   else if(key == GLUT_KEY_DOWN)
   {
      if(mod == GLUT_ACTIVE_SHIFT)
      {
         cam.ang.angleX += 1;
      }
      else
      {
         cam.pos.z -= 0.01;
      }
   }
   else if(key == GLUT_KEY_LEFT)
   {
      if(mod == GLUT_ACTIVE_SHIFT)
      {
         cam.ang.angleZ += 1;
      }
      else
      {
         cam.pos.x += 0.01;
      }
   }
   else if(key == GLUT_KEY_RIGHT)
   {
       if(mod == GLUT_ACTIVE_SHIFT)
      {
         cam.ang.angleZ -= 1;
      }
      else
      {
         cam.pos.x -= 0.01;
      }
   }
   glutPostRedisplay();
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
   glutSpecialFunc(handleKeyboard);
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}