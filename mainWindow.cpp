#include "common.h"
#include "mainWindow.h"
#include "expression.h"
#include "fractal.h"
#include "fbo.h"

char* name;
int windowID;

float window_aspect;
bool fullScreen=false;

// GLUT Utility Variables
float zoom = 1;
float mouse_x, mouse_y;
float arcmouse_x, arcmouse_y, arcmouse_z;

bool right_mouse_button= false;
bool left_mouse_button = false;

GLfloat rot_matrix[16] = {1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};

Queue<CliffordAttractor> fractals;
CliffordAttractor *mainFractal;

Color clearColor = kBlack;

int createMainWindow(string _name, bool saving) {
  window_width = glutGet(GLUT_SCREEN_WIDTH);
  window_height = glutGet(GLUT_SCREEN_HEIGHT);
  window_aspect = window_width / static_cast<float>(window_height);

  int doubleBuffer = (saving ? GLUT_SINGLE : GLUT_DOUBLE);
  glutInitDisplayMode(doubleBuffer | GLUT_RGBA);

  GLsizei windowX = ( saving ? 1 : (glutGet(GLUT_SCREEN_WIDTH)-window_width)/2 );
  GLsizei windowY = ( saving ? 1 : (glutGet(GLUT_SCREEN_HEIGHT)-window_height)/2 );

  glutInitWindowPosition(windowX, windowY);
  glutInitWindowSize(window_width, window_height);

  windowID = glutCreateWindow(_name.c_str());
  glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

  // Enable Blending for transparency
  glShadeModel(GL_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_DEPTH_TEST);

  ExternalRenderer::setImageWidth(window_width);
  ExternalRenderer::setImageHeight(window_height);
  return windowID;
}

/*
  Adjust the camera to fit Fractal f.
*/
void adjustBounds(AttractorFractal& f) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, window_width/window_height, 1, 1500);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  BoundingBox bbox = f.getbb();
  float x, y, z, r, fdistance;
  x = (bbox.max[0] + bbox.min[0]) / 2.0f;
  y = (bbox.max[1] + bbox.min[1]) / 2.0f;
  z = (bbox.max[2] + bbox.min[2]) / 2.0f;
  r = sqrt((bbox.max[0] - x)*(bbox.max[0] - x) + (bbox.max[1] - y)*(bbox.max[1] - y) + (bbox.max[2] - z)*(bbox.max[2] - z));
  fdistance = r / .3697f;
  gluLookAt(0.0f, 0.0f, fdistance*zoom, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  glTranslatef(-x, -y, -z);

  glMultMatrixf(rot_matrix);
}

//****************************************
void Repaint() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (!waiting) {
    adjustBounds(*mainFractal);
    if (mainFractal->paint()) {
      if (rendering) TwDraw();
    } else {
      waiting = true;
    }
  }
  
  glFlush();
  glutSwapBuffers();
}

void Idle() {
  static float angle = 1.0;
  if (waiting) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0,1.0,1.0);
    glRotatef(angle,0,1,0);
    glutWireCube(20);
    glFlush();
    glutSwapBuffers();
  }
}

//****************************************
/*
  Update viewport in response to change in window size
*/
void Reshape(int w, int h){
  glViewport (0, 0, (GLsizei)w, (GLsizei)h);
  if (rendering) TwWindowSize(w, h);
  glutPostRedisplay();
}

void resize() {
  glutReshapeWindow(window_width,window_height); // Default window size
  GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-window_width)/2;
  GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-window_height)/2;
  glutPositionWindow(windowX, windowY); // Center window
}

void resize(int w, int h) {
  window_width = w;
  window_height = h;
  ExternalRenderer::setImageWidth(window_width);
  ExternalRenderer::setImageHeight(window_height);
  resize();
  Reshape(w, h);
}

//****************************************
/*
  Return a vector from fractal center to world coordinates of mouse
*/
Vec3f arcSnap(float x, float y) {
  x = (2.0*x / window_width) - 1;
  y = (2.0*y / window_height) - 1;

  float mag2 = x * x + y * y;
  float mag = sqrt(mag2);

  if (mag > 1) {
    x = x*0.999 / mag;  // mult by .999 to account for edge cases of rounding up
    y = y*0.999 / mag;
  }

  float z = sqrt(1.0 - (x*x + y*y));
  return Vec3f::makeVec(x, y, z);
}

//****************************************
void MouseButton(int button, int state, int x, int y) {
  if (rendering) {
    if (!TwEventMouseButtonGLUT(button, state, x, y)) {
      y = window_height - y;

      if (button == GLUT_LEFT_BUTTON) {
        Vec3f arc_coords = arcSnap(x, y);
        arcmouse_x = arc_coords[0];
        arcmouse_y = arc_coords[1];
        arcmouse_z = arc_coords[2];

        left_mouse_button = !state;  // state==0 if down
      }

      if (button == GLUT_RIGHT_BUTTON) {
        right_mouse_button = !state;  // state==0 if down
      }

      mouse_x = x, mouse_y = y;
    }
    glutPostRedisplay();
  }
}

//****************************************
void MouseMotion(int x, int y) {
  if (rendering) {
    if (!TwEventMouseMotionGLUT(x, y)) {
      y = window_height - y;

      if (left_mouse_button) {
        // Arc-ball rotational movement
        Vec3f arc_coords = arcSnap(x, y);
        float fx = arc_coords[0];
        float fy = arc_coords[1];
        float fz = arc_coords[2];

        // Find rotational axis
        float normal_x = arcmouse_y*fz - arcmouse_z*fy;
        float normal_y = arcmouse_z*fx - arcmouse_x*fz;
        float normal_z = arcmouse_x*fy - arcmouse_y*fx;

        // Find rotational angle
        float ax = sqrt(normal_x*normal_x +
                        normal_y*normal_y +
                        normal_z*normal_z);

        float ay = arcmouse_x*fx + arcmouse_y*fy + arcmouse_z*fz;
        float angle = atan2(ax, ay)*180/3.14159;

        // Modify and save rotation matrix
        glLoadIdentity();
        glRotatef(angle, normal_x, normal_y, normal_z);
        glMultMatrixf(rot_matrix);
        glGetFloatv(GL_MODELVIEW_MATRIX, rot_matrix);

        arcmouse_x = fx, arcmouse_y = fy, arcmouse_z = fz;

      } else if (right_mouse_button && y && mouse_y) {
        // Zoom: Multiplies current zoom by ratio between initial and current y
        float smy = mouse_y+window_height;
        float sy = y+window_height;
        float dy;

        if (sy < 0 && smy < 0) {
          dy = abs(smy/sy);
        } else {
          dy = abs(sy/smy);
        }

        zoom *= dy;
      }

      mouse_x = x, mouse_y = y;
    }
    glutPostRedisplay();
  }
}


void Keyboard(unsigned char key, int x, int y){
  if (rendering) {
    if (!TwEventKeyboardGLUT(key, x, y)) {
      switch(key){
      case 32:  // (Spacebar) Mutate fractal
        mainFractal->mutateConstants();
        zoom = 1;
        glutPostRedisplay();
        break;
      case 'F':  // Toggle fullscreen
      case 'f':
        if(fullScreen){
          resize();
          fullScreen = false;
        }
        else{
          fullScreen = true;
          glutFullScreen();
        }
        glutPostRedisplay();
        break;
      case 's':  // Save to test.png
        ExternalRenderer::outputToImage("test");
        break;
      case 27 : // (ESC) close the program
        glutDestroyWindow(windowID);
        exit(0);
        break;
      }
    }
    glutPostRedisplay();         
  }
}

void registerCallbacks() {
  // set the event handling methods
  TwGLUTModifiersFunc(glutGetModifiers);
  glutDisplayFunc(Repaint);
  glutReshapeFunc(Reshape);
  glutMouseFunc(MouseButton);
  glutMotionFunc(MouseMotion);
  glutKeyboardFunc(Keyboard);
  glutFullScreen();
  fullScreen=true;
}
