/*
 * objdepth.h
 *
 *  Created on: 2012/06/12
 *      Author: ynagai
 */

#ifndef OBJDEPTH_H_
#define OBJDEPTH_H_

#include <string>

namespace ps_objdepth {

class ObjDepth {
public:
  ObjDepth(std::string *objnames, int num);
  virtual ~ObjDepth();
  void CalcDepth(double **light_norms, int intensity);
};

} /* namespace ps_objdepth */
#endif /* OBJDEPTH_H_ */
