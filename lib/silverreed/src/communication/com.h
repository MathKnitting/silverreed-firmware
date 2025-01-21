#ifndef COM_H_
#define COM_H_

using namespace std;

#include "knitting.h"

class Communicator {
 public:
  virtual ~Communicator() = default;
  void init();
  void update();
};
#endif
