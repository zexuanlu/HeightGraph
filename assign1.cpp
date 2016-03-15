/*
  CSCI 480 Computer Graphics
  Assignment 1: Height Fields
  C++ starter code
*/

#include <stdlib.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <pic.h>

int g_iMenuId;

int g_vMousePos[2] = {0, 0};
int g_iLeftMouseButton = 0;    /* 1 if pressed, 0 if not */
int g_iMiddleMouseButton = 0;
int g_iRightMouseButton = 0;
int keyboardbutton = 0;
int rotationbutton = 0;

char myfile[2048];

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;

CONTROLSTATE g_ControlState = ROTATE;

/* state of the world */
float g_vLandRotate[3] = {0.0, 0.0, 0.0};
float g_vLandTranslate[3] = {0.0, 0.0, 0.0};
float g_vLandScale[3] = {1.0, 1.0, 1.0};

/* see <your pic directory>/pic.h for type Pic */
Pic * g_pHeightData;

/* Write a screenshot to the specified filename */
void saveScreenshot (char *filename)
{
int i, j;   

Pic *in = NULL;   

Pic *out = NULL;

if (filename == NULL)       

return;

in = pic_alloc(640, 480, 3, NULL);   

out = pic_alloc(640, 480, 3, NULL);

printf("File to save to: %s\n", filename);
//  for (i=479; i>=0; i--) {

//    glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE, &in->pix[i*in->nx*in->bpp]);

//  }

glReadPixels(0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, &in->pix[0]);       

for ( int j=0; j<480; j++ ) { 

for ( int i=0; i<640; i++ ) { 

PIC_PIXEL(out, i, j, 0) = PIC_PIXEL(in, i, 480-1-j, 0); 

PIC_PIXEL(out, i, j, 1) = PIC_PIXEL(in, i, 480-1-j, 1);             

PIC_PIXEL(out, i, j, 2) = PIC_PIXEL(in, i, 480-1-j, 2); 

} 

}

if (jpeg_write(filename, out))       

printf("File saved Successfully\n");   

else       

printf("Error in Saving\n");

pic_free(in);    

pic_free(out);
}

void myinit()
{
  /* setup gl view here */
  glMatrixMode(GL_PROJECTION);

  gluPerspective(60, 640.0/480, 0.01, 1000.0);

  glClearColor(0.0, 0.0, 0.0, 0.0);
}

/* Function that generate Z coordinate */
double CalPix(Pic *pic,int x,int y){
  double temp = PIC_PIXEL(pic, x, y, 0);
  return temp/1000;
}

/* Scale coordinate to make it smaller to fit the screen */
double scaleCor(double x){
  return x/150;
}

/* Draw Triangle */
void Triangle(){

  double width = g_pHeightData->nx/2;
  double height = g_pHeightData->ny/2;

  for (double i=0; i < g_pHeightData->ny - 1; i++){
        glBegin(GL_TRIANGLE_STRIP);
    for (double j=0; j< g_pHeightData->nx - 1; j++){

    glColor3f(CalPix(g_pHeightData, j, i), CalPix(g_pHeightData, j, i),CalPix(g_pHeightData, j, i)*(float)j/(float)width);
        glVertex3f(scaleCor(j-width), scaleCor(i-height), CalPix(g_pHeightData, j, i));
    glColor3f(CalPix(g_pHeightData, j, i+1), CalPix(g_pHeightData, j, i+1),CalPix(g_pHeightData, j, i+1)*(float)j/(float)width);
        glVertex3f(scaleCor(j-width), scaleCor(i-height + 1), CalPix(g_pHeightData, j, i+1));
    glColor3f(CalPix(g_pHeightData, j+1, i), CalPix(g_pHeightData, j+1, i),CalPix(g_pHeightData, j+1, i)*(float)j/(float)width);
        glVertex3f(scaleCor(j-width + 1), scaleCor(i-height), CalPix(g_pHeightData, j+1, i));
    glColor3f(CalPix(g_pHeightData, j+1, i+1), CalPix(g_pHeightData, j+1, i+1),CalPix(g_pHeightData, j+1, i+1)*(float)j/(float)width);
        glVertex3f(scaleCor(j-width + 1), scaleCor(i-height+1), CalPix(g_pHeightData, j+1, i+1));

      }
  glEnd();
}
 
}

/* Draw lines */
void Line(){
  glBegin(GL_LINES);

  double width = g_pHeightData->nx/2;
  double height = g_pHeightData->ny/2;

  for (double i=0; i < g_pHeightData->ny - 1; i++){
    for (double j=0; j< g_pHeightData->nx - 1; j++){
          glColor3f(CalPix(g_pHeightData, j, i), CalPix(g_pHeightData, j, i),CalPix(g_pHeightData, j, i)*(float)j/(float)width);

      glVertex3f(scaleCor(j-width), scaleCor(i-height), CalPix(g_pHeightData, j, i));
          glColor3f(CalPix(g_pHeightData, j+1, i), CalPix(g_pHeightData, j+1, i),CalPix(g_pHeightData, j+1, i)*(float)j/(float)width);

      glVertex3f(scaleCor(j-width + 1), scaleCor(i-height), CalPix(g_pHeightData, j+1, i));
    }
  }



  glEnd();
}

/* Draw points */
void Point(){
  glBegin(GL_POINTS);

    double width = g_pHeightData->nx/2;
  double height = g_pHeightData->ny/2;

  for (double i=0; i < g_pHeightData->ny - 1; i++){
    for (double j=0; j< g_pHeightData->nx - 1; j++){
    glColor3f(CalPix(g_pHeightData, j, i), CalPix(g_pHeightData, j, i),CalPix(g_pHeightData, j, i)*(float)j/(float)width);

      glVertex3f(scaleCor(j-width), scaleCor(i-height), CalPix(g_pHeightData, j, i));
    }
  }



  glEnd();
}

void display()
{
  /* draw 1x1 cube about origin */
  /* replace this code with your height field implementation */
  /* you may also want to precede it with your 
rotation/translation/scaling */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

   glTranslatef(g_vLandTranslate[0], g_vLandTranslate[1], g_vLandTranslate[2]); //Translation

  glRotatef(g_vLandRotate[0], 1.0, 0.0, 0.0); 
  glRotatef(g_vLandRotate[1], 0.0, 1.0, 0.0);        // Rotation
  glRotatef(g_vLandRotate[2], 0.0, 0.0, 1.0);

  glScalef(g_vLandScale[0], g_vLandScale[1], g_vLandScale[2]);   // Scale

  if(keyboardbutton == 0){
    Triangle();
  }
  else if(keyboardbutton == 1){     // Press buttons to switch shapes
    Line();
  }
  else if(keyboardbutton == 2){
    Point();
  }


  glutSwapBuffers();
}

void menufunc(int value)
{
  switch (value)
  {
    case 0:
      exit(0);
      break;
  }
}

void doIdle()
{
  /* Press 'v' to rotate and do screenshot */
  static int count = 0;
  if(rotationbutton == 1){
  GLfloat delta = 1.0;
  GLint axis = 2;
  g_vLandRotate[axis] += delta;
if (g_vLandRotate[axis] > 360.0) g_vLandRotate[axis] -= 360.0;
if (count < 300) {
  sprintf(myfile, "anim.%04d.jpg", count + 1);
  saveScreenshot(myfile);
  count++;
}
// myFilenm will be anim.0001.jpg, anim.0002.jpg..........anim.0999.jpg // ..
}
  /* make the screen update */
  glutPostRedisplay();
}

/* Set keyboard buttons */
void keyboard(unsigned char key, int x, int y){
  if(key =='q'||key == 'Q')
    exit(0);
  if(key =='d'||key == 'D')
    keyboardbutton = 0;
  if(key =='f'||key == 'F')
    keyboardbutton = 1;
  if(key =='g'||key == 'G')
    keyboardbutton = 2;
  if(key =='c'||key =='C')
    rotationbutton = 0;
  if(key =='v'||key =='V')
    rotationbutton = 1 - rotationbutton;  
}


/* converts mouse drags into information about 
rotation/translation/scaling */
void mousedrag(int x, int y)
{
  int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
  
  switch (g_ControlState)
  {
    case TRANSLATE:  
      if (g_iLeftMouseButton)
      {
        g_vLandTranslate[0] += vMouseDelta[0]*0.01;
        g_vLandTranslate[1] -= vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandTranslate[2] += vMouseDelta[1]*0.01;
      }
      break;
    case ROTATE:
      if (g_iLeftMouseButton)
      {
        g_vLandRotate[0] += vMouseDelta[1];
        g_vLandRotate[1] += vMouseDelta[0];
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandRotate[2] += vMouseDelta[1];
      }
      break;
    case SCALE:
      if (g_iLeftMouseButton)
      {
        g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
        g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
      }
      break;
  }
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mouseidle(int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mousebutton(int button, int state, int x, int y)
{

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      g_iLeftMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
      break;
  }
 
  switch(glutGetModifiers())
  {
    case GLUT_ACTIVE_CTRL:
      g_ControlState = TRANSLATE;
      break;
    case GLUT_ACTIVE_SHIFT:
      g_ControlState = SCALE;
      break;
    default:
      g_ControlState = ROTATE;
      break;
  }

  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

int main (int argc, char ** argv)
{
  if (argc<2)
  {  
    printf ("usage: %s heightfield.jpg\n", argv[0]);
    exit(1);
  }

  g_pHeightData = jpeg_read(argv[1], NULL);
  if (!g_pHeightData)
  {
    printf ("error reading %s.\n", argv[1]);
    exit(1);
  }

  glutInit(&argc,argv);
  
  /*
    create a window here..should be double buffered and use depth testing
  
    the code past here will segfault if you don't have a window set up....
    replace the exit once you add those calls.
  */

    glutInitDisplayMode (GLUT_DOUBLE |GLUT_DEPTH |GLUT_RGB);

    glutInitWindowSize(640,480);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Assign1");
    //glutReshapeFunc();

    glEnable(GL_DEPTH_TEST); // enable depth test

    glutKeyboardFunc(keyboard); // enable keyboard



  /* tells glut to use a particular display function to redraw */
  glutDisplayFunc(display);
  
  /* allow the user to quit using the right mouse button menu */
  g_iMenuId = glutCreateMenu(menufunc);
  glutSetMenu(g_iMenuId);
  glutAddMenuEntry("Quit",0);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  /* replace with any animate code */
  glutIdleFunc(doIdle);

  /* callback for mouse drags */
  glutMotionFunc(mousedrag);
  /* callback for idle mouse movement */
  glutPassiveMotionFunc(mouseidle);
  /* callback for mouse button changes */
  glutMouseFunc(mousebutton);

  /* do initialization */
  myinit();

  glutMainLoop();
  return(0);
}
