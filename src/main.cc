//============================================================================
// Name        : main.cc
// Author      : ynagai
// Version     :
// Copyright   : ynagai
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "lightposition.h"
#include "objdepth.h"
using namespace std;

int main() {
  string ballnames[4] = {"Ball1.bmp", "Ball2.bmp", "Ball3.bmp", "Ball4.bmp"};
  int ballnum = sizeof(ballnames) / sizeof(ballnames[0]);
  ps_lp::LightPosition lp = ps_lp::LightPosition(ballnames, ballnum);
  double **light_normals = new double*[ballnum];
  for (int index = 0; index < ballnum; ++index) {
    light_normals[index] = new double[3];
  }
  lp.GetNorms(light_normals);
  string objnames[4] = {"Object1.bmp", "Object2.bmp", "Object3.bmp", "Object4.bmp"};
  if (ballnum != sizeof(objnames) / sizeof(objnames[0])) {
    cout << "Should be the same number: Ball and Obj" << endl;
    return -1;
  }
  ps_objdepth::ObjDepth objdepth = ps_objdepth::ObjDepth(objnames, ballnum);
  objdepth.CalcDepth(light_normals, lp.GetIntensity());
  return 0;
}
