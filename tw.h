#pragma once

#include "fbo.h"
#include "fractal.h"

void redrawFractal(AttractorFractal *fractal) {
  fractal->clear();
  fractal->calculate();
  glutPostRedisplay();
}

void TW_CALL CopyStdStringToClient(std::string& dest, const std::string& src) {
  dest = src;
}

void TW_CALL twSetBGColor(const void *value, void *clientData) {
  const float *c = static_cast<const float *>(value);
  //  const float *c = *srcPtr;
  if(clearColor[0] != c[0] || 
     clearColor[1] != c[1] || 
     clearColor[2] != c[2] || 
     clearColor[3] != c[3]) {
    clearColor = Color(c[0], c[1], c[2], c[3]);
    glClearColor(c[0], c[1], c[2], c[3]);
    glutPostRedisplay();
  }
}

void TW_CALL twGetBGColor(void *value, void *clientData) {
  float *c = static_cast<float *>(value);
  c[0] = clearColor[0];
  c[1] = clearColor[1];
  c[2] = clearColor[2];
  c[3] = clearColor[3];
}

void TW_CALL twSetPrecisionPoints(const void *value, void *clientData) {
  const int *srcPtr = static_cast<const int *>(value);
  if (PRECISION_POINTS != *srcPtr) {
    setPrecisionPoints(*srcPtr);
    redrawFractal(mainFractal);
  }
}

void TW_CALL twGetPrecisionPoints(void *value, void *clientData) {
  int *destPtr = static_cast<int *>(value);
  *destPtr = PRECISION_POINTS;  
}

void TW_CALL twSetAlpha(const void *value, void *clientData) {
  const float *srcPtr = static_cast<const float *>(value);
  if (ALPHA != *srcPtr) {
    setAlpha(*srcPtr);
    redrawFractal(mainFractal);
  }
}

void TW_CALL twGetAlpha(void *value, void *clientData) {
  float *srcPtr = static_cast<float *>(value);
  *srcPtr = ALPHA;
}

void TW_CALL AttractorFractal::setXStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  if (fractal->expressionX->getString().compare(*srcPtr) != 0) {
    fractal->expressionX->setString(*srcPtr);
    redrawFractal(fractal);
  }
}
void TW_CALL AttractorFractal::setYStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  if (fractal->expressionY->getString().compare(*srcPtr) != 0) {
    fractal->expressionY->setString(*srcPtr);
    redrawFractal(fractal);
  }
}
void TW_CALL AttractorFractal::setZStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  if (fractal->expressionZ->getString().compare(*srcPtr) != 0) {
    fractal->expressionZ->setString(*srcPtr);
    redrawFractal(fractal);
  }
}
void TW_CALL AttractorFractal::setRStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
    if (fractal->expressionR->getString().compare(*srcPtr) != 0) {
      fractal->expressionR->setString(*srcPtr);
      redrawFractal(fractal);
    }
}
void TW_CALL AttractorFractal::setGStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  if (fractal->expressionG->getString().compare(*srcPtr) != 0) {
    fractal->expressionG->setString(*srcPtr);
    redrawFractal(fractal);
  }
}
void TW_CALL AttractorFractal::setBStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  if (fractal->expressionB->getString().compare(*srcPtr) != 0) {
    fractal->expressionB->setString(*srcPtr);
    redrawFractal(fractal);
  }
}

void TW_CALL AttractorFractal::getXStr(void *value, void *clientData) {
  const AttractorFractal *fractal = static_cast<const AttractorFractal *>(clientData);
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, fractal->expressionX->getString());
}
void TW_CALL AttractorFractal::getYStr(void *value, void *clientData) {
  const AttractorFractal *fractal = static_cast<const AttractorFractal *>(clientData);
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, fractal->expressionY->getString());
}
void TW_CALL AttractorFractal::getZStr(void *value, void *clientData) {
  const AttractorFractal *fractal = static_cast<const AttractorFractal *>(clientData);
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, fractal->expressionZ->getString());
}
void TW_CALL AttractorFractal::getRStr(void *value, void *clientData) {
  const AttractorFractal *fractal = static_cast<const AttractorFractal *>(clientData);
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, fractal->expressionR->getString());
}
void TW_CALL AttractorFractal::getGStr(void *value, void *clientData) {
  const AttractorFractal *fractal = static_cast<const AttractorFractal *>(clientData);
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, fractal->expressionG->getString());
}
void TW_CALL AttractorFractal::getBStr(void *value, void *clientData) {
  const AttractorFractal *fractal = static_cast<const AttractorFractal *>(clientData);
  std::string *destPtr = static_cast<std::string *>(value);
  TwCopyStdStringToLibrary(*destPtr, fractal->expressionB->getString());
}
