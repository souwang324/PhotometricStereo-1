/*
 * objdepth.cc
 *
 *  Created on: 2012/06/12
 *      Author: ynagai
 */

#include "objdepth.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using ::std::cout;
using ::std::endl;
using ::std::string;
using ::cv::Mat;

namespace {
// -----------------
// Private Variables
// -----------------
string *objnames_;
int num_;
// ---------------------
// Prototype Declaration
// ---------------------
Mat GetLightArray(double **light_norms);
// ---------------
// Private Methods
// ---------------
Mat GetLightArray(double **light_norms) {
  Mat ret = cv::Mat_<double>(num_, 3);
  for (int y = 0; y < ret.rows; ++y) {
    for (int x = 0; x < ret.cols; ++x) {
      ret.at<double>(y, x) = light_norms[y][x];
    }
  }
  return ret;
}
} // namespace

namespace ps_objdepth {

ObjDepth::ObjDepth(string *objnames, int num) {
  objnames_ = objnames;
  num_ = num;
}

ObjDepth::~ObjDepth() {
}

void ObjDepth::CalcDepth(double **light_norms, int intensity) {
  GetLightArray(light_norms);
}

} /* namespace ps_objdepth */
