#include "fractalEditor.h"

int createFractalEditorWindow() {
  TwWindowSize(window_width, window_height);
  
  TwBar *editBar;
  editBar = TwNewBar("Fractal Editor");
  return 1;
}
