#ifndef __ACK_H
#define __ACK_H

#include "common.h"

using namespace std;

class Ack {
public:
  bool isValid() {

  }
  char* unserialize() {

  }
  void serialize(char* c) {

  }
  Byte getAck() {

  }

private:
  // return checksum computed based on data
  unsigned int generateChecksum() {

  }
  unsigned int hash(unsigned int input) {
    unsigned int val = 5381;
    val += ((input << 5) ^ ((input & 0xf8000000) >> 27));
    return val;
  }
  Byte value;
  unsigned int frame_number;
  unsigned int checksum;
};

#endif
