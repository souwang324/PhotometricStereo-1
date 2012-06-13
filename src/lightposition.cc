/*
 * lightposition.cc
 *
 *  Created on: 2012/06/12
 *      Author: ynagai
 */

#include "lightposition.h"
#include <iostream>
#include <cmath>
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
string *ballnames_;
int num_;
int *intensities_;
// -----------------
// Private Structure
// -----------------
typedef struct with_double_depth {
  int x;
  int y;
  double z;
}WithDoubleDepth;
// -------------
// Inline Method
// -------------
inline WithDoubleDepth ToSphere(int x, int y, int radius2, int *center) {
  WithDoubleDepth ret;
  ret.x = x;
  ret.y = y;
  ret.z = -sqrt(radius2 - ((x - center[0]) * (x - center[0]) + (y - center[1]) * (y - center[1])));
  return ret;
}
// ---------------------
// Prototype Declaration
// ---------------------
void CalcNormal(double *norm, WithDoubleDepth left, WithDoubleDepth bottom, WithDoubleDepth top);
double CalcRadius2(Mat src_mat);
int GetMax(Mat src_mat);
// --------------
// Private Method
// --------------
void CalcNormal(double *norm, WithDoubleDepth left, WithDoubleDepth bottom, WithDoubleDepth top) {
  double veca[3] = {bottom.x - left.x, bottom.y - left.y, bottom.z - left.z};
  double vecb[3] = {top.x - left.x, top.y - left.y, top.z - left.z};
  norm[0] = veca[1] * vecb[2] - veca[2] * vecb[1];
  norm[1] = veca[2] * vecb[0] - veca[0] * vecb[2];
  norm[2] = veca[0] * vecb[1] - veca[1] * vecb[0];
  double length = std::sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
  norm[0] /= length;
  norm[1] /= length;
  norm[2] /= length;
  return;
}
double CalcRadius2(Mat src_mat) {
  int left = 0;
  int right = 0;
  for (int y = 0; y < src_mat.rows; ++y) {
    bool isZero = true;
    for (int x = 0; x < src_mat.cols; ++x) {
      int pixel = (int)src_mat.at<unsigned char>(y, x);
      if (0 < pixel) {
        // MostLeft
        if (isZero) {
          left = x;
          isZero = false;
        }
      }
      // MostRight
      if (!isZero && x + 1 < src_mat.cols && (int)src_mat.at<unsigned char>(y, x + 1) == 0 ) {
        right = x;
        break;
      }
    } // for x
    if (!isZero) {
      int center[2] = {src_mat.cols / 2, src_mat.rows / 2};
      int middle = (left + right) / 2;
      return (middle - center[0]) * (middle - center[0]) + (y - center[1]) * (y - center[1]);
    }
  } // for y
  return 0.0;
}
int GetMax(Mat src_mat) {
  int max = 0;
  for (int y = 0; y < src_mat.rows; ++y) {
    for (int x = 0; x < src_mat.cols; ++x) {
      int pixel = (int)src_mat.at<unsigned char>(y, x);
      if (max < pixel) {
        max = pixel;
      }
    } // for x
  } // for y
  return max;
}
} // namespace

namespace ps_lp {

LightPosition::LightPosition(string *ballnames, int num) {
  ballnames_ = ballnames;
  num_ = num;
  intensities_ = new int[num_];
}

LightPosition::~LightPosition() {
}

void LightPosition::GetNorms(double **norms) {
  for (int ballindex = 0; ballindex < num_; ++ballindex) {
    // Get file name
    const string path_to_file = "Resources/" + ballnames_[ballindex];
    // Get cv::Mat
    Mat src_mat = cv::imread(path_to_file, 0);
    // Calc radius
    int radius_square = CalcRadius2(src_mat);
    // Get max pixel
    int max = GetMax(src_mat);
    // For push intensity
    intensities_[ballindex] = max;
    int center[2] = {src_mat.cols / 2, src_mat.rows / 2};
    WithDoubleDepth vec_top = {0, 0, 0.0};
    WithDoubleDepth vec_left = {src_mat.cols, 0, 0.0};
    WithDoubleDepth vec_bottom = {0, 0, 0.0};
    for (int y = 0; y < src_mat.rows; ++y) {
      for (int x = 0; x < src_mat.cols; ++x) {
        const int pixel = (int)src_mat.at<unsigned char>(y, x);
        // Max region
        if (pixel == max) {
          // Top Left = First max
          if (vec_top.x == 0) {
            vec_top = ToSphere(x, y, radius_square, center);
          }
          // Most Left
          if (x < vec_left.x) {
            vec_left = ToSphere(x, y, radius_square, center);
          }
          // Bottom right
          vec_bottom = ToSphere(x, y, radius_square, center);
        } // if pixel == max
      } // for x
    } // for y
    //cout << vec_top.x << " " << vec_top.y << " " << vec_top.z << endl;
    CalcNormal(norms[ballindex], vec_left, vec_bottom, vec_top);
    //cout << norms[ballindex][0] << " " << norms[ballindex][1] << " " << norms[ballindex][2] << endl;
  }
  return;
}
int LightPosition::GetIntensity() {
  const int ret = intensities_[0];
  for (int index = 1; index < num_; ++index) {
    if (ret != intensities_[index]) {
      cout << "Invalid lights for balls" << endl;
      exit(-1);
    }
  }
  return ret;
}

} /* namespace ps_lp */
