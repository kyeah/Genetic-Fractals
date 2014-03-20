#include "fractalEditor.h"
#include "tw.h"

int createFractalEditorWindow() {
  TwWindowSize(window_width, window_height);
  
  TwBar *editBar;
  editBar = TwNewBar("Fractal Editor");
  TwDefine(" 'Fractal Editor' size='400 300' valueswidth='fit' ");
  TwCopyStdStringToClientFunc(CopyStdStringToClient);

  TwAddVarCB(editBar, "Precision Points", TW_TYPE_INT32, 
             twSetPrecisionPoints, twGetPrecisionPoints, NULL,
             "");

  TwAddVarCB(editBar, "Per-point Alpha", TW_TYPE_FLOAT, 
             twSetAlpha, twGetAlpha, NULL,
             "");

  TwAddVarCB(editBar, "BG Color", TW_TYPE_COLOR4F,
             twSetBGColor, twGetBGColor, NULL,
             "");

  TwAddSeparator(editBar, "Equation Separator", "");

  TwAddVarCB(editBar, "X Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setXStr, AttractorFractal::getXStr, mainFractal,
             "group=Shape");
  
  TwAddVarCB(editBar, "Y Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setYStr, AttractorFractal::getYStr, mainFractal,
             "group=Shape");

  TwAddVarCB(editBar, "Z Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setZStr, AttractorFractal::getZStr, mainFractal,
             "group=Shape");

  TwAddVarCB(editBar, "R Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setRStr, AttractorFractal::getRStr, mainFractal,
             "group=Color");

  TwAddVarCB(editBar, "G Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setGStr, AttractorFractal::getGStr, mainFractal,
             "group=Color");

  TwAddVarCB(editBar, "B Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setBStr, AttractorFractal::getBStr, mainFractal,
             "group=Color");

  // Allow users to toggle between center-view mode and WASD/Camera movement.
  //TwAddButton(editBar, "View-mode", toggleMovement, NULL, "");

  TwDefine(" 'Fractal Editor'/'Precision Points' min=0");
  TwDefine(" 'Fractal Editor'/'Per-point Alpha' min=0 max=1");
  return 1;
}
