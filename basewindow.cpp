#include "basewindow.h"
#define nullptr 0
basewindow::basewindow(QWidget* parent):QWidget(parent)
{

}

QComboBox* basewindow::getBox() {return nullptr;}
scale* basewindow::getSca() {return nullptr;}

void basewindow::zoomIn() { }

void basewindow::zoomOut() { }

void basewindow::original() { }

void basewindow::fullScreen() { }

void basewindow::reloadImg() {}

void basewindow::setNum(int) { }

float basewindow::getRatio() { return 0; }

void basewindow::setPixelSize(float) { }

showwindow* basewindow::getshowwindow() {return nullptr;}

void basewindow::setCalculate(bool) { }

bool basewindow::getCalculate(){ return 0; }

void basewindow::setPath(QString) { }

void basewindow::reset() { }

void basewindow::addItem(int) { }

void basewindow::setIndex(int) { }

void basewindow::setTotalNum(int) { }

QString basewindow::getPath() { return "";}

void basewindow::setI(bool*) {}
