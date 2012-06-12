/*
 * lightposition.h
 *
 *  Created on: 2012/06/12
 *      Author: ynagai
 */

#ifndef LIGHTPOSITION_H_
#define LIGHTPOSITION_H_
#include <string>

namespace ps_lp {

class LightPosition {
public:
  LightPosition(std::string *ballnames, int num);
  virtual ~LightPosition();
  void GetNorms(double **norms);
  int GetIntensity();
};

} /* namespace ps_lp */
#endif /* LIGHTPOSITION_H_ */

