/*
 * objdepth.cc
 *
 *  Created on: 2012/06/12
 *      Author: ynagai
 */

#include "objdepth.h"
#include <iostream>
#include <fstream>
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
const double kUndefinedDepth = -100;
// ---------------------
// Prototype Declaration
// ---------------------
Mat GetLightArray(double **light_norms, int num);
// ---------------
// Private Methods
// ---------------
Mat GetLightArray(double **light_norms, int num) {
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
  const Mat light_mat = GetLightArray(light_norms, num_);
  Mat *src_mat = new Mat[num_];
  for (int index = 0; index < num_; ++index) {
    // Get file name
    const string path_to_file = "resources/" + objnames_[index];
    src_mat[index] = cv::imread(path_to_file, 0);
    if (0 < index) {
      if (src_mat[index].size != src_mat[index].size) {
        cout << "Invalid object image size." << endl;
        exit(-1);
      }
    } // if 0 < index
  } // for index
  const int size = src_mat[0].rows * src_mat[0].cols;
  // Depth map: UpperLeft is zero
  double *depth_map = new double[size];
  // UpperLeft is zero
  depth_map[0] = 0;
  // Output file stream
  std::ofstream ofs("resources/output.txt");
  for (int y = 0; y < src_mat[0].rows; ++y) {
    for (int x = 0; x < src_mat[0].cols; ++x) {
      Mat normal_mat = cv::Mat_<double>(3, 1);
      Mat intensity_mat = cv::Mat_<double>(num_, 1);
      for (int index = 0; index < num_; ++index) {
        intensity_mat.at<double>(index, 0) = (double)src_mat[index].at<unsigned char>(y, x);
      }
      normal_mat = light_mat.inv(cv::DECOMP_SVD) * intensity_mat;
      if (0 < x && y == 0) {
        double tmp = normal_mat.at<double>(0, 0) / normal_mat.at<double>(2, 0)
                + depth_map[x - 1 + y * src_mat[0].step];
        depth_map[x + y * src_mat[0].step] = tmp;
        ofs << tmp << endl;
      }
      else if (0 < y){
        if (x == 0) {
//          double tmp_u = normal_mat.at<double>(1, 0) / normal_mat.at<double>(2, 0)
//                                  + depth_map[x + (y - 1) * src_mat[0].step];
          double tmp_ur = (- normal_mat.at<double>(0, 0) + normal_mat.at<double>(1, 0)) / normal_mat.at<double>(2, 0)
                        + depth_map[x + 1 + (y - 1) * src_mat[0].step];
//          double tmp = (tmp_u + tmp_ur) / 2.0;
          double tmp = tmp_ur;
          depth_map[x + y * src_mat[0].step] = tmp;
          ofs << tmp << endl;
        }
        else {
          double tmp_ul = (normal_mat.at<double>(0, 0) + normal_mat.at<double>(1, 0)) / normal_mat.at<double>(2, 0)
                    + depth_map[x - 1 + (y - 1) * src_mat[0].step];
//          double tmp_l = normal_mat.at<double>(0, 0) / normal_mat.at<double>(2, 0)
//                              + depth_map[x - 1 + y * src_mat[0].step];
//          double tmp_u = normal_mat.at<double>(1, 0) / normal_mat.at<double>(2, 0)
//                              + depth_map[x + (y - 1) * src_mat[0].step];
//          double tmp = (tmp_ul + tmp_l + tmp_u) / 3.0;
          double tmp = tmp_ul;
          depth_map[x + y * src_mat[0].step] = tmp;
          ofs << tmp << endl;
        }
      }
      //cout << light_mat.inv(cv::DECOMP_SVD) << endl;
    } // for x
  } // for y
  for (int index = 0; index < size; ++index) {
    cout << depth_map[index] << endl;
  }
}
} /* namespace ps_objdepth */
