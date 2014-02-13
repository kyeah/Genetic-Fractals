#include "fractalEditor.h"

int createFractalEditorWindow() {
  TwWindowSize(window_width, window_height);
  
  TwBar *editBar;
  editBar = TwNewBar("Fractal Editor");
  TwDefine(" 'Fractal Editor' size='400 300' ");

  TwAddVarCB(editBar, "X Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setXStr, AttractorFractal::getXStr, &(fractals[0]),
             "");
  return 1;
}
