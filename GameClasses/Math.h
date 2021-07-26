//
// Created by drive on 26/7/21.
//

#ifndef OPENGLTEST_MATH_H
#define OPENGLTEST_MATH_H
#include <iostream>

static float lerp(float x, float a, float b) {return x*(b-a)+a;}
static float sat(float x){return std::min(std::max(x,0.0f),1.0f);}
#endif //OPENGLTEST_MATH_H
