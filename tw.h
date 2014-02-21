#pragma once

#include "fbo.h"
#include "fractal.h"

void TW_CALL CopyStdStringToClient(std::string& dest, const std::string& src) {
  dest = src;
}

void TW_CALL twSetPrecisionPoints(const void *value, void *clientData) {
  const int *srcPtr = static_cast<const int *>(value);
  setPrecisionPoints(*srcPtr);
}

void TW_CALL twGetPrecisionPoints(void *value, void *clientData) {
  int *destPtr = static_cast<int *>(value);
  *destPtr = PRECISION_POINTS;  
}

void TW_CALL AttractorFractal::setXStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  fractal->expressionX->setString(*srcPtr);
  fractal->clear();
  fractal->calculate();
  glutPostRedisplay();
}
void TW_CALL AttractorFractal::setYStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  fractal->expressionY->setString(*srcPtr);
  fractal->clear();
  fractal->calculate();
  glutPostRedisplay();
}
void TW_CALL AttractorFractal::setZStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  fractal->expressionZ->setString(*srcPtr);
  fractal->clear();
  fractal->calculate();
  glutPostRedisplay();
}
void TW_CALL AttractorFractal::setRStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  fractal->expressionR->setString(*srcPtr);
  fractal->clear();
  fractal->calculate();
  glutPostRedisplay();
}
void TW_CALL AttractorFractal::setGStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  fractal->expressionG->setString(*srcPtr);
  fractal->clear();
  fractal->calculate();
  glutPostRedisplay();
}
void TW_CALL AttractorFractal::setBStr(const void *value, void *clientData) {
  const std::string *srcPtr = static_cast<const std::string *>(value);
  AttractorFractal *fractal = static_cast<AttractorFractal *>(clientData);
  fractal->expressionB->setString(*srcPtr);
  fractal->clear();
  fractal->calculate();
  glutPostRedisplay();
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
