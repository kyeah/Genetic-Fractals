#include "fractalEditor.h"
#include "tw.h"

int createFractalEditorWindow() {
  TwWindowSize(window_width, window_height);
  
  TwBar *editBar;
  editBar = TwNewBar("Fractal Editor");
  TwDefine(" 'Fractal Editor' size='400 300' ");
  TwCopyStdStringToClientFunc(CopyStdStringToClient);

  TwAddVarCB(editBar, "X Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setXStr, AttractorFractal::getXStr, &(fractals[0]),
             "");
  /*
  TwAddVarCB(editBar, "Y Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setYStr, AttractorFractal::getYStr, &(fractals[0]),
             "");

  TwAddVarCB(editBar, "Z Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setZStr, AttractorFractal::getZStr, &(fractals[0]),
             "");

  TwAddVarCB(editBar, "R Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setRStr, AttractorFractal::getRStr, &(fractals[0]),
             "");

  TwAddVarCB(editBar, "G Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setGStr, AttractorFractal::getGStr, &(fractals[0]),
             "");

  TwAddVarCB(editBar, "B Equation", TW_TYPE_STDSTRING, 
             AttractorFractal::setBStr, AttractorFractal::getBStr, &(fractals[0]),
             "");
  */
  return 1;
}
