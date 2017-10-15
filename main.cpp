#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "common.h"
#include "mainWindow.h"
#include "fractalEditor.h"
#include "expression.h"
#include "fractal.h"
#include "fbo.h"

using namespace std;

bool rendering = true;
atomic_bool waiting(false);
int window_width, window_height;

// Legacy variables for Mandelbrot Set
GLfloat minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; // complex plane boundaries
const GLfloat radius = 5.0f;
const int paletteSize = 128;
GLfloat palette[paletteSize][3];

//****************************************
/*
  Legacy palette generator for Mandelbrot Set
*/
GLfloat* calculateColor(GLfloat u, GLfloat v){
  GLfloat re = u;
  GLfloat im = v;
  GLfloat tempRe=0.0;
  for(int i=0; i < paletteSize; i++){
    tempRe = re*re - im*im + u;
    im = re * im * 2 + v;
    re = tempRe;
    if( (re*re + im*im) > radius ){
      return palette[i];
    }
  }
  return kBlack;
}


//****************************************
void createPalette(){
  for(int i=0; i < 32; i++){
    palette[i][0] = (8*i)/(GLfloat)255;
    palette[i][1] = (128-4*i)/(GLfloat)255;
    palette[i][2] = (255-8*i)/(GLfloat)255;
  }
  for(int i=0; i < 32; i++){
    palette[32+i][0] = (GLfloat)1;
    palette[32+i][1] = (8*i)/(GLfloat)255;
    palette[32+i][2] = (GLfloat)0;
  }
  for(int i=0; i < 32; i++){
    palette[64+i][0] = (128-4*i)/(GLfloat)255;
    palette[64+i][1] = (GLfloat)1;
    palette[64+i][2] = (8*i)/(GLfloat)255;
  }
  for(int i=0; i < 32; i++){
    palette[96+i][0] = (GLfloat)0;
    palette[96+i][1] = (255-8*i)/(GLfloat)255;
    palette[96+i][2] = (8*i)/(GLfloat)255;
  }
}

void registerControlCallbacks() {

}

void renderInGlut() {
  glutMainLoop();
  TwTerminate();
}

void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}

void startDisplay() {
  TwInit(TW_OPENGL, NULL);
  createFractalEditorWindow();
  registerCallbacks();
  renderInGlut();
}

//****************************************
int main(int argc, char** argv){

  bool savingImage = ( argc >= 3 && strcmp(argv[1],"-save") == 0 );
  bool runInBackground = ( argc >= 2 && strcmp(argv[1],"-bg") == 0);

  glutInit(&argc, argv);
  createMainWindow("Aesthetic Fractals", savingImage);

  // Enable GLEW library for External rendering
  #ifndef __APPLE__
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(0);
  }
  #endif

  if (runInBackground) {
    //    string cmd;
    //    
    //    while(cmd << cin) {
    //      char* 
    //    }
  }

  if (argc >= 3) {
    rendering = !savingImage;
    int startIndex = (savingImage ? 2 : 1);
    GLuint renderbuffer;
    char* imgName = "test";

    if (savingImage) {
      /*
        User wants to save the fractal image.
        ./aesthetics -save [ -p PRECISION_POINTS ] [ -s WIDTH HEIGHT ] IMG_NAME
        EXPR_X EXPR_Y EXPR_Z EXPR_R EXPR_G EXPR_B
      */

      // Create new offscreen renderbuffer
      glutHideWindow();
      ExternalRenderer::switchToExternalTarget();
      ExternalRenderer::getNewRenderBuffer(&renderbuffer);
      glutHideWindow();
    }

    // Iterate over all fractal definitions provided by user
    for (int i = startIndex; i <= argc - 6; i+=6) {

      // Set number of points to calculate (default 1,000,000)
      if (strcmp(argv[i],"-p") == 0) {
        setPrecisionPoints(stoi(argv[++i]));
        ++i;
      }

      // Resize window
      if (strcmp(argv[i],"-s") == 0) {
        int width = stoi(argv[++i]);
        int height = stoi(argv[++i]);
        resize(width, height);
        ++i;
      }

      if (savingImage) {
        imgName = argv[i++];
      }

      // Calculate points
      CliffordAttractor ca(imgName, argv[i], argv[i+1], argv[i+2], argv[i+3], argv[i+4], argv[i+5]);
      mainFractal = new CliffordAttractor(imgName, argv[i], argv[i+1], argv[i+2], argv[i+3], argv[i+4], argv[i+5]);
      //      mainFractal = &ca;

      if (savingImage) {
        mainFractal->calculate();
        glutHideWindow();
        
        pthread_join(mainFractal->getCalcThread(), NULL);
        Repaint();
        ExternalRenderer::outputToImage(mainFractal->getName());

        grayscale = true;
        alpha_save = false;
        resize(640, 360);
        std::stringstream ss;
        ss << mainFractal->getName() << "_filter";
        Repaint();
        ExternalRenderer::outputToImage(ss.str());
        //mainFractal->saveToFile(mainFractal->getName());

      } else {
        startDisplay();
      }
    }

  } else {
    // No fractal definitions provided; draw an example fractal.
    // mainFractal = new CliffordAttractor("test", "sin(-1.4 * y) + cos(-1.4 * x)", "sin(1.6 * x) + 0.7 * cos(1.6 * y)", "x", "x", "y", "z");
     mainFractal = new CliffordAttractor("test", "sin( a * y ) + c * cos(a * x)", "sin(b * x) + d * cos(b * y)", "x", "x", "y", "z");
    
    startDisplay();
  }
  
  return 0;
}

/*
  TODO: Use glfw instead of glut if detected, for hidden window support.
  http://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project
  int glfwStartup() {
  // Initialise GLFW
  if( !glfwInit() )
  {
  fprintf( stderr, "Failed to initialize GLFW\n" );
  return -1;
  }

  glfwSetErrorCallback(error_callback);
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL
  glfwWindowHint(GLFW_VISIBLE, 0);

  // Open a window and create its OpenGL context
  GLFWwindow* window = glfwCreateWindow( 1024, 768, "Aesthetic Fractals glfw Test", NULL, NULL );
  if (!window)
  {
  fprintf( stderr, "Failed to open GLFW window\n" );
  glfwTerminate();
  return -1;
  }

  // Enable GLEW library for External rendering
  GLenum err = glewInit();
  if (GLEW_OK != err) {
  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  exit(0);
  }

  glfwMakeContextCurrent(window);
  return 1;
  }
*/
